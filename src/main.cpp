#include <iostream>
#include "cache.h"
#include "utils.h"

int main(int argc, char *argv[]){
    if(isValidArg(argc, argv)){
        return 0;
    }
    else{
        return 1;
    }
    return 0;
}