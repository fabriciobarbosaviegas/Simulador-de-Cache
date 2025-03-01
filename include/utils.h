#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <cstdint>
#include <string>

bool isValidArg(int argc, char *argv[]);
void calcBits(int bsize, int nsets, int &offset_bits, int &index_bits, int &tag_bits);

#endif // UTILS_H