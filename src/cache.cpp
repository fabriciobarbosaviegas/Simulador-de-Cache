#include "cache.h"
#include "utils.h"
#include <iostream>
#include <cstdio>
#include <cctype>
#include <cstdlib>

using namespace std;

Cache::Cache(int nsets, int bsize, int assoc, char subst)
    : nsets(nsets), bsize(bsize), assoc(assoc),
      politica_substituicao(std::toupper(subst)),
      total_acessos(0), hits(0),
      misses_compulsorios(0), misses_capacidade(0), misses_conflito(0),
      blocos_ocupados(0)
{
    total_blocos = nsets * assoc;
    // Calcula os bits para offset, índice e tag (assume endereços de 32 bits)
   
    calcBits(bsize, nsets, bits_offset, bits_indice, bits_tag);
    // Inicializa cada conjunto: vetor de blocos, inicialmente inválidos
    conjuntos.resize(nsets, vector<Bloco>(assoc, {0, false}));
    // Se a política for FIFO ou LRU, inicializa o deque de ordem para cada conjunto
    if (politica_substituicao == 'F' || politica_substituicao == 'L') {
        ordem.resize(nsets);
    }
}

void Cache::acessarEndereco(uint32_t endereco) {
    total_acessos++;
    
    // Extrai a tag e o índice do endereço
    uint32_t tag = endereco >> (bits_offset + bits_indice);
    uint32_t indice = (endereco >> bits_offset) & ((1 << bits_indice) - 1);
    
    // Procura o bloco no conjunto
    int via_encontrada = -1;
    for (int via = 0; via < assoc; via++) {
        if (conjuntos[indice][via].valid && conjuntos[indice][via].tag == tag) {
            via_encontrada = via;
            break;
        }
    }
    
    if (via_encontrada != -1) { // HIT
        hits++;
        // Se a política for LRU, atualiza a ordem: remove a via e a reinsere no final
        if (politica_substituicao == 'L') {
            auto &fila = ordem[indice];
            for (auto it = fila.begin(); it != fila.end(); ++it) {
                if (*it == via_encontrada) {
                    fila.erase(it);
                    break;
                }
            }
            fila.push_back(via_encontrada);
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
        if (politica_substituicao == 'F' || politica_substituicao == 'L') {
            ordem[indice].push_back(viaVazia);
        }
        blocos_ocupados++;
    } else {
        // Conjunto cheio: substituição necessária
        int via_substituir;
        if (politica_substituicao == 'R') {
            // Substituição RANDOM: seleciona um índice aleatório entre 0 e (assoc-1)
            via_substituir = rand() % assoc;
        } else {
            // Para FIFO ou LRU: seleciona o bloco no início do deque (o mais antigo ou o menos recentemente usado)
            via_substituir = ordem[indice].front();
            ordem[indice].pop_front();
        }
        conjuntos[indice][via_substituir].tag = tag;
        if (politica_substituicao == 'F' || politica_substituicao == 'L') {
            ordem[indice].push_back(via_substituir);
        }
        if (blocos_ocupados == total_blocos)
            misses_capacidade++;
        else
            misses_conflito++;
    }
}

void Cache::exibirEstatisticas(int flag_saida) const {
    int total_misses = misses_compulsorios + misses_capacidade + misses_conflito;
    double taxa_hit = (total_acessos > 0) ? static_cast<double>(hits) / total_acessos : 0.0;
    double taxa_miss = 1.0 - taxa_hit;
    double taxa_miss_compulsorio = (total_misses > 0) ? static_cast<double>(misses_compulsorios) / total_misses : 0.0;
    double taxa_miss_capacidade  = (total_misses > 0) ? static_cast<double>(misses_capacidade) / total_misses : 0.0;
    double taxa_miss_conflito   = (total_misses > 0) ? static_cast<double>(misses_conflito) / total_misses : 0.0;
    
    if (flag_saida == 0) {
        std::cout << "+-------------------------+-----------------+\n";
        std::cout << "| Estatísticas            | Valores         |\n";
        std::cout << "+-------------------------+-----------------+\n";
        std::cout << "| Total de acessos        | " << total_acessos << "\n";
        std::cout << "| Hits                    | " << hits << " (" << taxa_hit* 100 << "%)\n";
        std::cout << "| Misses                  | " << total_misses << "\n";
        std::cout << "| Misses compulsorios     | " << misses_compulsorios << "\n";
        std::cout << "| Misses de capacidade    | " << misses_capacidade << "\n";
        std::cout << "| Misses de conflito      | " << misses_conflito << "\n";
        std::cout << "+-------------------------+-----------------+\n";
    } else {
        // Formatação conforme output esperado
        printf("%d %.4lf %.4lf %.2lf %.2lf %.2lf\n",
               total_acessos, taxa_hit, taxa_miss,
               taxa_miss_compulsorio, taxa_miss_capacidade, taxa_miss_conflito);
    }
}
