#ifndef SHAFA_DADOS_H
#define SHAFA_DADOS_H

#include <stdlib.h>
#include <stdbool.h>
#include "debug.h"

#define MIN_FILE_SIZE 1024 // 1Kbyte

typedef enum
{
    FICHEIRO_DEMASIADO_PEQUENO = -1,
    OK = 0,
    ERRO_IO = 2
} SINAL;

typedef struct
{
    bool ultimo_absorvido;
    unsigned long long total_data_size;
    long long n_blocks;
    unsigned long block_size;
    long last_block_size;
    char ** data;
} FICHEIRO;

typedef struct
{
    bool rle_compression;
    long long n_blocks;
    unsigned long block_size;
    long last_block_size;
    int** freqs;
} SYMBSFREQ;

FICHEIRO* inicializarFicheiro();
SYMBSFREQ* inicializarSymbFreq();
SYMBSFREQ* allocateFreqsBlocks(SYMBSFREQ* s, long long n);

#endif //SHAFA_DADOS_H
