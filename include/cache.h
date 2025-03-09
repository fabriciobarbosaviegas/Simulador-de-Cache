#ifndef CACHE_H
#define CACHE_H

#include <cstdint>
#include <vector>
#include <deque>

class Cache {
public:
    // Construtor:
    // nsets: número de conjuntos;
    // bsize: tamanho do bloco (em bytes);
    // assoc: grau de associatividade;
    // substPolicy: política de substituição ('F' para FIFO, 'L' para LRU)
    Cache(int nsets, int bsize, int assoc, char substPolicy);

    // Processa o acesso a um endereço (32 bits)
    void acessarEndereco(uint32_t endereco);

    // Exibe as estatísticas da simulação (flag_saida: 0 = formato detalhado, 1 = formato compacto)
    void exibirEstatisticas(int flag_saida) const;

private:
    int nsets;   // Número de conjuntos
    int bsize;   // Tamanho do bloco (bytes)
    int assoc;   // Grau de associatividade
    char substitutionPolicy; // 'F' para FIFO ou 'L' para LRU

    int offset_bits; // Bits de deslocamento
    int index_bits;  // Bits para o índice
    int tag_bits;    // Bits para a tag

    int total_acessos;
    int hits;
    int misses_compulsorios;
    int misses_capacidade;
    int misses_conflito;

    int occupiedBlocks; // Quantidade de blocos já ocupados na cache
    int numBlocks;      // Total de blocos (nsets * assoc)

    // Representação de um bloco da cache
    struct Bloco {
        uint32_t tag;
        bool valid;
    };

    // Vetor de conjuntos – cada conjunto é um vetor de blocos
    std::vector< std::vector<Bloco> > conjuntos;
    // Para cada conjunto, um deque que armazena os índices das vias na ordem de acesso.
    // Para FIFO, a ordem é de inserção; para LRU, o bloco mais recentemente usado fica no final.
    std::vector< std::deque<int> > ordem;
};

#endif // CACHE_H
