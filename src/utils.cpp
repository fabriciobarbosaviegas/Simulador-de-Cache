#include <iostream>
#include "utils.h"
#include <cmath>
#include <fstream>
#include <string>
#include <stdexcept> // Para std::invalid_argument
#include <vector>
#include <cstdint>

// Função auxiliar para verificar se um número é potência de 2
bool isPowerOfTwo(int n) {
    return (n > 0) && ((n & (n - 1)) == 0);
}

// Valida os argumentos da linha de comando
bool isValidArg(int argc, char* argv[]) {
    int nsets, bsize, assoc, flag;
    char subst;
    std::string arquivo;

    // Verifica número de argumentos
    if (argc != 7) {
        std::cerr << "Erro: Número incorreto de argumentos.\n";
        std::cerr << "Uso: " << argv[0] << " <nsets> <bsize> <assoc> <substituição> <flag_saida> <arquivo_entrada>\n";
        return false;
    }

    // Converte argumentos para inteiros
    try {
        nsets = std::stoi(argv[1]);
        bsize = std::stoi(argv[2]);
        assoc = std::stoi(argv[3]);
        subst = toupper(argv[4][0]); // Aceita 'r', 'f', 'l' minúsculos
        flag = std::stoi(argv[5]);
        arquivo = argv[6];
    } catch (...) {
        std::cerr << "Erro: Argumentos inválidos (não são números inteiros).\n";
        return false;
    }

    // Valida nsets, bsize, assoc
    if (nsets <= 0 || bsize <= 0 || assoc <= 0) {
        std::cerr << "Erro: nsets, bsize e assoc devem ser maiores que zero.\n";
        return false;
    }

    // Valida potências de 2 (exceto assoc=1 para mapeamento direto)
    if (!isPowerOfTwo(nsets) || !isPowerOfTwo(bsize)) {
        std::cerr << "Erro: nsets e bsize devem ser potências de 2.\n";
        return false;
    }

    // Valida política de substituição
    if (subst != 'R' && subst != 'F' && subst != 'L') {
        std::cerr << "Erro: Política de substituição inválida. Use R (Random), F (FIFO) ou L (LRU).\n";
        return false;
    }

    // Valida flag_saida
    if (flag != 0 && flag != 1) {
        std::cerr << "Erro: flag_saida deve ser 0 ou 1.\n";
        return false;
    }

    // Valida arquivo de entrada
    std::ifstream file(arquivo, std::ios::binary);
    if (!file.good()) {
        std::cerr << "Erro: Arquivo de entrada '" << arquivo << "' não encontrado ou inválido.\n";
        return false;
    }
    file.close();

    return true;
}

// Faz os calculos necessarios para usar os bits
void calcBits(int bsize, int nsets, int &offset_bits, int &index_bits, int &tag_bits) {
    try {
        offset_bits = log2(bsize); 
        index_bits = log2(nsets);
        tag_bits = 32 - offset_bits - index_bits;
    } catch (const std::invalid_argument& e) {
        throw std::invalid_argument("Erro em calcularBits: " + std::string(e.what()));
    }
}

std::vector<uint32_t> readBinFile(const std::string &nome_arquivo) {
    std::ifstream file(nome_arquivo, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Erro ao abrir o arquivo: " + nome_arquivo);
    }

    std::vector<uint32_t> enderecos;
    uint32_t endereco;

    // Lê 4 bytes (32 bits) por vez
    while (file.read(reinterpret_cast<char*>(&endereco), sizeof(uint32_t))) {
        // Converte de big-endian para little-endian (se necessário)
        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            endereco = __builtin_bswap32(endereco);
        #endif
        enderecos.push_back(endereco);
    }

    return enderecos;
}