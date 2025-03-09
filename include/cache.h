#ifndef CACHE_H
#define CACHE_H

#include <cstdint>
#include <vector>
#include <deque>

class Cache {
public:
    // Construtor: nsets = número de conjuntos, bsize = tamanho do bloco (em bytes), assoc = grau de associatividade
    Cache(int nsets, int bsize, int assoc);

    // Processa o acesso ao endereço (32 bits)
    void acessarEndereco(uint32_t endereco);

    // Exibe as estatísticas da simulação; flag_saida define o formato da saída
    void exibirEstatisticas(int flag_saida) const;

private:
    int nsets;   // Número de conjuntos
    int bsize;   // Tamanho do bloco (bytes)
    int assoc;   // Grau de associatividade

    int offset_bits; // Número de bits de deslocamento
    int index_bits;  // Número de bits para o índice do conjunto
    int tag_bits;    // Número de bits para a tag

    int total_acessos;
    int hits;
    int misses_compulsorios;
    int misses_capacidade;
    int misses_conflito;

    int occupiedBlocks; // Quantidade de blocos já ocupados na cache
    int numBlocks;      // Total de blocos (nsets * assoc)

    // Estrutura que representa um bloco da cache
    struct Bloco {
        uint32_t tag;
        bool valid;
    };

    // Vetor de conjuntos, onde cada conjunto é um vetor de blocos
    std::vector< std::vector<Bloco> > conjuntos;

    // Para cada conjunto, uma fila FIFO armazenando os índices das vias (para substituição)
    std::vector< std::deque<int> > filas_fifo;
};

#endif // CACHE_H
