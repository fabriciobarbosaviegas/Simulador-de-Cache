#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <cstdint>

// Estrutura de um bloco da cache
struct Bloco {
    uint32_t tag;        // Tag do bloco
    bool valid;          // Bit de validade
    uint64_t fifo_counter; // Contador FIFO (ordem de inserção)
};

class Cache {
private:
    int nsets;              // Número de conjuntos
    int bsize;              // Tamanho do bloco (bytes)
    int assoc;              // Associatividade
    int offset_bits;        // Bits de offset
    int index_bits;         // Bits de índice
    int tag_bits;           // Bits de tag

    std::vector<std::vector<Bloco>> conjuntos; // conjuntos[índice][via]
    uint64_t global_counter = 0; // Contador global para FIFO

    // Estatísticas
    uint64_t total_acessos = 0;
    uint64_t hits = 0;
    uint64_t misses_compulsorios = 0;
    uint64_t misses_capacidade = 0;
    uint64_t misses_conflito = 0;

public:
    Cache(int nsets, int bsize, int assoc);
    void acessarEndereco(uint32_t endereco);
    void exibirEstatisticas(int flag_saida) const;
};

#endif // CACHE_H