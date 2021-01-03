#ifndef SHAFA_DADOS_H
#define SHAFA_DADOS_H

#include <stdlib.h>
#include <stdbool.h>

#define MIN_FILE_SIZE 1024 // 1Kbyte

typedef enum
{
    FICHEIRO_DEMASIADO_PEQUENO = -1,
    OK = 0,
    ERRO_IO = 2
} SINAL;

#endif //SHAFA_DADOS_H
