#include "cache.h"
#include "utils.h"
#include <iostream>
#include <algorithm> // Para std::min_element

Cache::Cache(int nsets, int bsize, int assoc) 
    : nsets(nsets), bsize(bsize), assoc(assoc) {
    // Calcula bits de offset, índice e tag
    calcBits(bsize, nsets, offset_bits, index_bits, tag_bits);

    // Inicializa conjuntos e blocos (todos inválidos)
    conjuntos.resize(nsets);
    for (auto &conjunto : conjuntos) {
        conjunto.resize(assoc, {0, false, 0}); // tag=0, valid=false, fifo_counter=0
    }
}

void Cache::acessarEndereco(uint32_t endereco) {
    total_acessos++;

    // Extrai tag, índice e offset
    uint32_t tag = endereco >> (offset_bits + index_bits);
    uint32_t indice = (endereco >> offset_bits) & ((1 << index_bits) - 1);

    // Verifica se o bloco está no conjunto (HIT)
    bool hit = false;
    for (int via = 0; via < assoc; via++) {
        if (conjuntos[indice][via].valid && conjuntos[indice][via].tag == tag) {
            hits++;
            hit = true;
            break;
        }
    }

    // Trata MISS
    if (!hit) {
        bool substituido = false;

        // Procura por bloco inválido (MISS COMPULSÓRIO)
        for (int via = 0; via < assoc; via++) {
            if (!conjuntos[indice][via].valid) {
                conjuntos[indice][via].tag = tag;
                conjuntos[indice][via].valid = true;
                conjuntos[indice][via].fifo_counter = ++global_counter;
                misses_compulsorios++;
                substituido = true;
                break;
            }
        }

        // Se todos os blocos estão válidos, substitui via FIFO
        if (!substituido) {
            // Encontra a via com o menor fifo_counter (mais antiga)
            int via_substituir = 0;
            uint64_t menor_counter = conjuntos[indice][0].fifo_counter;
            for (int via = 1; via < assoc; via++) {
                if (conjuntos[indice][via].fifo_counter < menor_counter) {
                    menor_counter = conjuntos[indice][via].fifo_counter;
                    via_substituir = via;
                }
            }

            // Substitui e atualiza o contador FIFO
            conjuntos[indice][via_substituir].tag = tag;
            conjuntos[indice][via_substituir].fifo_counter = ++global_counter;

            // Classifica o miss (conflito ou capacidade)
            if (assoc == 1) {
                misses_conflito++;
            } else {
                misses_capacidade++;
            }
        }
    }
}

void Cache::exibirEstatisticas(int flag_saida) const {
    if (flag_saida == 0) {
        std::cout << "Total de acessos: " << total_acessos << "\n";
        std::cout << "Hits: " << hits << " (" << (hits / (double)total_acessos) * 100 << "%)\n";
        std::cout << "Misses: " << (total_acessos - hits) << "\n";
        std::cout << "  - Compulsórios: " << misses_compulsorios << "\n";
        std::cout << "  - Capacidade: " << misses_capacidade << "\n";
        std::cout << "  - Conflito: " << misses_conflito << "\n";
    } else {
        std::cout << total_acessos << " "
                  << (hits / (double)total_acessos) << " "
                  << ((total_acessos - hits) / (double)total_acessos) << " "
                  << (misses_compulsorios / (double)total_acessos) << " "
                  << (misses_capacidade / (double)total_acessos) << " "
                  << (misses_conflito / (double)total_acessos) << "\n";
    }
}