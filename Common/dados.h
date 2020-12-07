#ifndef SHAFA_DADOS_H
#define SHAFA_DADOS_H

#include <stdlib.h>
#include <stdbool.h>
#include "dados.h"
#include "debug.h"

typedef enum
{
    OK = 0,
    COMANDO_INEXISTENTE = 1,
    ERRO_IO = 2
} SINAL;

typedef struct
{
    int size;
    char * data;
} FICHEIRO;

FICHEIRO* inicializar_ficheiro();

#endif //SHAFA_DADOS_H
