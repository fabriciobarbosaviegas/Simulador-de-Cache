#include <iostream>
#include <vector>
#include "cache.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    if (!isValidArg(argc, argv)) {
        return 1;
    }

    int nsets = std::stoi(argv[1]);
    int bsize = std::stoi(argv[2]);
    int assoc = std::stoi(argv[3]);
    std::string arquivo_entrada = argv[6];

    // Cria cache
    Cache cache(nsets, bsize, assoc);

    // Lê endereços do arquivo
    std::vector<uint32_t> enderecos = readBinFile(arquivo_entrada);

    // Processa cada endereço
    for (uint32_t endereco : enderecos) {
        cache.acessarEndereco(endereco);
    }

    // Exibe estatísticas
    int flag_saida = std::stoi(argv[5]);
    cache.exibirEstatisticas(flag_saida);

    return 0;
}