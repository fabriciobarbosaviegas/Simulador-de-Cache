#include <iostream>
#include "utils.h"
#include <cmath>
#include <fstream>
#include <string>
#include <stdexcept> // Para std::invalid_argument

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

// Calcula log2
int log2(int n) {
    if (n <= 0 || (n & (n - 1)) != 0) { // Garante que n é potência de 2 e positivo
        throw std::invalid_argument("log2: n deve ser uma potência de 2 e positivo.");
    }
    int bits = 0;
    while (n > 1) {
        n >>= 1; // Desloca bits para a direita (equivalente a dividir por 2)
        bits++;
    }
    return bits;
}

void calcBits(int bsize, int nsets, int &offset_bits, int &index_bits, int &tag_bits) {
    try {
        offset_bits = log2(bsize);   // Já implementado
        index_bits = log2(nsets);    // Novo cálculo
        tag_bits = 32 - offset_bits - index_bits; // Novo cálculo
    } catch (const std::invalid_argument& e) {
        throw std::invalid_argument("Erro em calcularBits: " + std::string(e.what()));
    }
}
