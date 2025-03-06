#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <cstdint>

class Cache {
private:
    int nsets;              // Número de conjuntos
    int bsize;              // Tamanho do bloco (bytes)
    int assoc;              // Associatividade
    int offset_bits;        // Bits de offset
    int index_bits;         // Bits de índice
    int tag_bits;           // Bits de tag

    // Estrutura da cache: sets[conjunto][via]
    std::vector<std::vector<uint32_t>> tags;  // Tags dos blocos
    std::vector<std::vector<bool>> valid;     // Bits de validade

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