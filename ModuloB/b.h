#ifndef SHAFA_MODULOB_B_H
#define SHAFA_MODULOB_B_H

#include <stdio.h>
#include "../Common/dados.h"

SINAL moduloB(char* file_name);

void readMetaData (FILE* fp, SYMBSFREQ* symbs);
void readFreq(FILE* fp, int* arr, int SIZE);
SINAL readFreqs(SYMBSFREQ* symbs, char* file_name);


#endif //SHAFA_MODULOB_B_H
