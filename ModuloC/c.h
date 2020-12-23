#ifndef SHAFA_C_H
#define SHAFA_C_H

#include "../Common/dados.h"

SINAL moduloC(char* file_name);

typedef struct{
    bool modo;
    int n_blocks;
    int *block_size;
    int last_block_size;
    char ** data;
} FICHEIROC;

#endif //SHAFA_C_H
