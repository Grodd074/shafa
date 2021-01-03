#ifndef SHAFA_UTILS_H
#define SHAFA_UTILS_H

#include <stdio.h>
#include "dados.h"

SINAL loadFile (FICHEIRO* f, char* file_name, unsigned long block_size);

void printFileData (FICHEIRO* f);
void printBlockData (char* block, int block_size);
void printSymbsFreq (SYMBSFREQ* s);

int fileSize(char* file_name);

int soma(int* arr, int i, int j);


#endif //SHAFA_UTILS_H
