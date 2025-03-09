#include <iostream>
#include <vector>
#include <cctype>
#include "cache.h"
#include "utils.h"

int main(int argc, char* argv[]) {
    if (!isValidArg(argc, argv)) {
        return 1;
    }
    
    int nsets = std::stoi(argv[1]);
    int bsize = std::stoi(argv[2]);
    int assoc = std::stoi(argv[3]);
    char subst = std::toupper(argv[4][0]); // 'F' para FIFO ou 'L' para LRU
    int flag_saida = std::stoi(argv[5]);
    std::string arquivo = argv[6];

    // Cria a cache com a política desejada
    Cache cache(nsets, bsize, assoc, subst);

    // Lê os endereços do arquivo binário
    std::vector<uint32_t> enderecos = readBinFile(arquivo);
    for (uint32_t endereco : enderecos) {
        cache.acessarEndereco(endereco);
    }
    
    // Exibe as estatísticas da simulação
    cache.exibirEstatisticas(flag_saida);
    
    return 0;
}
