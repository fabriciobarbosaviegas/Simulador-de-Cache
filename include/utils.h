#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <cstdint>
#include <string>

bool isValidArg(int argc, char *argv[]);
void calcBits(int bsize, int nsets, int &bits_offset, int &bits_indice, int &bits_tag);
std::vector<uint32_t> readBinFile(const std::string &nome_arquivo);

#endif // UTILS_H