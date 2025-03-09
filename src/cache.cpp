#include "cache.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <deque>
#include <cstdint>
#include <cstdio>

using namespace std;

// Construtor
Cache::Cache(int nsets, int bsize, int assoc)
    : nsets(nsets), bsize(bsize), assoc(assoc),
      total_acessos(0), hits(0),
      misses_compulsorios(0), misses_capacidade(0), misses_conflito(0),
      occupiedBlocks(0)
{
    numBlocks = nsets * assoc;
    // Calcula os bits de offset, índice e tag (endereço de 32 bits)
    calcBits(bsize, nsets, offset_bits, index_bits, tag_bits);
    // Inicializa cada conjunto com "assoc" blocos (inicialmente inválidos)
    conjuntos.resize(nsets, vector<Bloco>(assoc, {0, false}));
    // Inicializa as filas FIFO para cada conjunto
    filas_fifo.resize(nsets);
}

void Cache::acessarEndereco(uint32_t endereco) {
    total_acessos++;
    
    // Extrai tag e índice
    uint32_t tag = endereco >> (offset_bits + index_bits);
    uint32_t indice = (endereco >> offset_bits) & ((1 << index_bits) - 1);

    // Verifica se há HIT
    bool hitFound = false;
    for (int via = 0; via < assoc; via++) {
        if (conjuntos[indice][via].valid && (conjuntos[indice][via].tag == tag)) {
            hits++;
            hitFound = true;
            break;
        }
    }
    if (hitFound)
        return;  // Fim: bloco já presente

    // Miss: procura via vazia no conjunto
    int emptyVia = -1;
    for (int via = 0; via < assoc; via++) {
        if (!conjuntos[indice][via].valid) {
            emptyVia = via;
            break;
        }
    }
    if (emptyVia != -1) {
        // Miss compulsório: via ainda não preenchida
        misses_compulsorios++;
        conjuntos[indice][emptyVia].tag = tag;
        conjuntos[indice][emptyVia].valid = true;
        filas_fifo[indice].push_back(emptyVia);
        occupiedBlocks++;
    } else {
        // Conjunto cheio: substituição FIFO
        int via_substituir = filas_fifo[indice].front();
        filas_fifo[indice].pop_front();
        conjuntos[indice][via_substituir].tag = tag;
        filas_fifo[indice].push_back(via_substituir);
        
        // Classifica o miss: se a cache inteira já estiver cheia, é de capacidade; caso contrário, é de conflito
        if (occupiedBlocks == numBlocks)
            misses_capacidade++;
        else
            misses_conflito++;
    }
}

void Cache::exibirEstatisticas(int flag_saida) const {
    int totalMisses = misses_compulsorios + misses_capacidade + misses_conflito;
    double hitRatio = (total_acessos > 0) ? (double)hits / total_acessos : 0.0;
    double missRatio = 1.0 - hitRatio;
    double compulsoryMissRatio = (totalMisses > 0) ? (double)misses_compulsorios / totalMisses : 0.0;
    double capacityMissRatio   = (totalMisses > 0) ? (double)misses_capacidade / totalMisses : 0.0;
    double conflictMissRatio   = (totalMisses > 0) ? (double)misses_conflito / totalMisses : 0.0;
    
    if (flag_saida == 0) {
        std::cout << "Total de acessos: " << total_acessos << "\n";
        std::cout << "Hits: " << hits << " (" << hitRatio * 100 << "%)\n";
        std::cout << "Misses: " << totalMisses << "\n";
        std::cout << "  - Misses compulsorios: " << misses_compulsorios << "\n";
        std::cout << "  - Misses de capacidade: " << misses_capacidade << "\n";
        std::cout << "  - Misses de conflito: " << misses_conflito << "\n";
    } else {
        // Formatação conforme output esperado
        printf("%d %.4lf %.4lf %.2lf %.2lf %.2lf\n",
               total_acessos, hitRatio, missRatio,
               compulsoryMissRatio, capacityMissRatio, conflictMissRatio);
    }
}
