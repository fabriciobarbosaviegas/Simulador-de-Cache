#include "cache.h"
#include "utils.h"
#include <iostream>

Cache::Cache(int nsets, int bsize, int assoc) 
    : nsets(nsets), bsize(bsize), assoc(assoc) {
    // Calcula bits de offset, índice e tag
    calcBits(bsize, nsets, offset_bits, index_bits, tag_bits);

    // Inicializa conjuntos e blocos (todos inválidos inicialmente)
    tags.resize(nsets, std::vector<uint32_t>(assoc, 0));
    valid.resize(nsets, std::vector<bool>(assoc, false));
}

void Cache::acessarEndereco(uint32_t endereco) {
    total_acessos++;

    // Extrai tag, índice e offset
    uint32_t tag = endereco >> (offset_bits + index_bits);
    uint32_t indice = (endereco >> offset_bits) & ((1 << index_bits) - 1);

    // Verifica se o bloco está no conjunto (HIT)
    bool hit = false;
    for (int via = 0; via < assoc; via++) {
        if (valid[indice][via] && tags[indice][via] == tag) {
            hits++;
            hit = true;
            break;
        }
    }

    // Se MISS, substitui o primeiro bloco inválido ou o primeiro válido (simplificado)
    if (!hit) {
        misses++;
        bool substituido = false;

        // Procura por um bloco inválido para substituir
        for (int via = 0; via < assoc; via++) {
            if (!valid[indice][via]) {
                tags[indice][via] = tag;
                valid[indice][via] = true;
                substituido = true;
                break;
            }
        }

        // Se todos os blocos estão válidos, substitui o primeiro (sem política)
        if (!substituido) {
            tags[indice][0] = tag; // Substituição simplificada (apenas para funcionar)
        }
    }
}

void Cache::exibirEstatisticas(int flag_saida) const {
    if (flag_saida == 0) {
        std::cout << "Total de acessos: " << total_acessos << "\n";
        std::cout << "Hits: " << hits << " (" << (hits / (double)total_acessos) * 100 << "%)\n";
        std::cout << "Misses: " << misses << " (" << (misses / (double)total_acessos) * 100 << "%)\n";
    } else {
        std::cout << total_acessos << ", " 
                  << (hits / (double)total_acessos) << ", " 
                  << (misses / (double)total_acessos) << "\n";
    }
}