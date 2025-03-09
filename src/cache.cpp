#include "cache.h"
#include "utils.h"
#include <iostream>
#include <cstdio>
#include <cctype>
#include <cstdlib>

using namespace std;

Cache::Cache(int nsets, int bsize, int assoc, char subst)
    : nsets(nsets), bsize(bsize), assoc(assoc),
      substitutionPolicy(std::toupper(subst)),
      total_acessos(0), hits(0),
      misses_compulsorios(0), misses_capacidade(0), misses_conflito(0),
      occupiedBlocks(0)
{
    numBlocks = nsets * assoc;
    // Calcula os bits para offset, índice e tag (assume endereços de 32 bits)
    calcBits(bsize, nsets, offset_bits, index_bits, tag_bits);
    // Inicializa cada conjunto: vetor de blocos, inicialmente inválidos
    conjuntos.resize(nsets, vector<Bloco>(assoc, {0, false}));
    // Se a política for FIFO ou LRU, inicializa o deque de ordem para cada conjunto
    if (substitutionPolicy == 'F' || substitutionPolicy == 'L') {
        ordem.resize(nsets);
    }
}

void Cache::acessarEndereco(uint32_t endereco) {
    total_acessos++;
    
    // Extrai a tag e o índice do endereço
    uint32_t tag = endereco >> (offset_bits + index_bits);
    uint32_t indice = (endereco >> offset_bits) & ((1 << index_bits) - 1);
    
    // Procura o bloco no conjunto
    int viaEncontrada = -1;
    for (int via = 0; via < assoc; via++) {
        if (conjuntos[indice][via].valid && conjuntos[indice][via].tag == tag) {
            viaEncontrada = via;
            break;
        }
    }
    
    if (viaEncontrada != -1) { // HIT
        hits++;
        // Se a política for LRU, atualiza a ordem: remove a via e a reinsere no final
        if (substitutionPolicy == 'L') {
            auto &fila = ordem[indice];
            for (auto it = fila.begin(); it != fila.end(); ++it) {
                if (*it == viaEncontrada) {
                    fila.erase(it);
                    break;
                }
            }
            fila.push_back(viaEncontrada);
        }
        return;
    }
    
    // MISS: procura uma via vazia no conjunto
    int viaVazia = -1;
    for (int via = 0; via < assoc; via++) {
        if (!conjuntos[indice][via].valid) {
            viaVazia = via;
            break;
        }
    }
    
    if (viaVazia != -1) {
        // Miss compulsório: há via não preenchida
        misses_compulsorios++;
        conjuntos[indice][viaVazia].tag = tag;
        conjuntos[indice][viaVazia].valid = true;
        if (substitutionPolicy == 'F' || substitutionPolicy == 'L') {
            ordem[indice].push_back(viaVazia);
        }
        occupiedBlocks++;
    } else {
        // Conjunto cheio: substituição necessária
        int viaSubstituir;
        if (substitutionPolicy == 'R') {
            // Substituição RANDOM: seleciona um índice aleatório entre 0 e (assoc-1)
            viaSubstituir = rand() % assoc;
        } else {
            // Para FIFO ou LRU: seleciona o bloco no início do deque (o mais antigo ou o menos recentemente usado)
            viaSubstituir = ordem[indice].front();
            ordem[indice].pop_front();
        }
        conjuntos[indice][viaSubstituir].tag = tag;
        if (substitutionPolicy == 'F' || substitutionPolicy == 'L') {
            ordem[indice].push_back(viaSubstituir);
        }
        if (occupiedBlocks == numBlocks)
            misses_capacidade++;
        else
            misses_conflito++;
    }
}

void Cache::exibirEstatisticas(int flag_saida) const {
    int totalMisses = misses_compulsorios + misses_capacidade + misses_conflito;
    double hitRatio = (total_acessos > 0) ? static_cast<double>(hits) / total_acessos : 0.0;
    double missRatio = 1.0 - hitRatio;
    double compulsoryMissRatio = (totalMisses > 0) ? static_cast<double>(misses_compulsorios) / totalMisses : 0.0;
    double capacityMissRatio   = (totalMisses > 0) ? static_cast<double>(misses_capacidade) / totalMisses : 0.0;
    double conflictMissRatio   = (totalMisses > 0) ? static_cast<double>(misses_conflito) / totalMisses : 0.0;
    
    if (flag_saida == 0) {
        std::cout << "+-------------------------+-----------------+\n";
        std::cout << "| Estatísticas            | Valores         |\n";
        std::cout << "+-------------------------+-----------------+\n";
        std::cout << "| Total de acessos        | " << total_acessos << "\n";
        std::cout << "| Hits                    | " << hits << " (" << hitRatio * 100 << "%)\n";
        std::cout << "| Misses                  | " << totalMisses << "\n";
        std::cout << "| Misses compulsorios     | " << misses_compulsorios << "\n";
        std::cout << "| Misses de capacidade    | " << misses_capacidade << "\n";
        std::cout << "| Misses de conflito      | " << misses_conflito << "\n";
        std::cout << "+-------------------------+-----------------+\n";
    } else {
        // Formatação conforme output esperado
        printf("%d %.4lf %.4lf %.2lf %.2lf %.2lf\n",
               total_acessos, hitRatio, missRatio,
               compulsoryMissRatio, capacityMissRatio, conflictMissRatio);
    }
}
