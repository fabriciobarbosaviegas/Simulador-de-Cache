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

    // Extrai tag, índice e offset do endereço
    uint32_t tag = (endereco >> (offset_bits + index_bits));
    uint32_t indice = (endereco >> offset_bits) & ((1 << index_bits) - 1);

    // Verifica se o bloco está no conjunto
    bool hit = false;
    for (int via = 0; via < assoc; via++) {
        if (valid[indice][via] && tags[indice][via] == tag) {
            hits++;
            hit = true;
            break;
        }
    }

    // Trata miss
    if (!hit) {
        bool encontrou_invalido = false;
        // Procura por um bloco inválido para substituir (miss compulsório)
        for (int via = 0; via < assoc; via++) {
            if (!valid[indice][via]) {
                tags[indice][via] = tag;
                valid[indice][via] = true;
                misses_compulsorios++;
                encontrou_invalido = true;
                break;
            }
        }

        // Se não encontrou inválido, classifica como conflito/capacidade
        if (!encontrou_invalido) {
            if (assoc == 1) { // Mapeamento direto -> miss de conflito
                misses_conflito++;
            } else { // Associativo -> miss de capacidade
                misses_capacidade++;
            }
        }
    }
}

// Rascunho
void Cache::exibirEstatisticas(int flag_saida) const {
    if (flag_saida == 0) {
        std::cout << "Total de acessos: " << total_acessos << "\n";
        std::cout << "Hits: " << hits << " (" << (hits / (double)total_acessos) * 100 << "%)\n";
        std::cout << "Misses: " << (total_acessos - hits) << "\n";
        std::cout << "  - Compulsórios: " << misses_compulsorios << "\n";
        std::cout << "  - Capacidade: " << misses_capacidade << "\n";
        std::cout << "  - Conflito: " << misses_conflito << "\n";
    } else {
        std::cout << total_acessos << ", "
                  << (hits / (double)total_acessos) << ", "
                  << ((total_acessos - hits) / (double)total_acessos) << ", "
                  << (misses_compulsorios / (double)total_acessos) << ", "
                  << (misses_capacidade / (double)total_acessos) << ", "
                  << (misses_conflito / (double)total_acessos) << "\n";
    }
}