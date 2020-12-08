#ifndef SHAFA_UTILS_H
#define SHAFA_UTILS_H

#include <stdio.h>
#include "dados.h"
#include "debug.h"

SINAL loadFile (FICHEIRO* f, char* file_name, unsigned long block_size);
void printFileData (FICHEIRO* f);
void printBlockData (char* block, int block_size);

#endif //SHAFA_UTILS_H
