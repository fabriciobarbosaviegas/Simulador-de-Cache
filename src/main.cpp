#include <iostream>
#include "cache.h"
#include "utils.h"

int main(int argc, char *argv[]){
    if(isValidArg(argc, argv)){
        std::vector<uint32_t> enderecos = readBinFile(argv[6]);
        return 0;
    }
    else{
        return 1;
    }
}