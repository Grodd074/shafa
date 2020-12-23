#ifndef SHAFA_C_H
#define SHAFA_C_H

#include "../Common/dados.h"

typedef struct{
    bool modo;
    int n_blocks;
    int *blocks_sizes;
    char *** data;
} FICHEIROC;
SINAL moduloC(char* file_name);
int skip_inicial(char *buffer);
int skip_arroba(int nArroba,int index,char *buffer);
int skip_semicolon(int index,char *buffer);
char *cod_to_buffer(FILE*cod);
int matrix_code(char *buffer);
char** file_to_buffers(FILE *fp,char*buffer_cod);

#endif //SHAFA_C_H
