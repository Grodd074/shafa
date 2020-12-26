#ifndef SHAFA_C_H
#define SHAFA_C_H

#include "../Common/dados.h"

typedef struct{
    bool modo;
    int n_blocos;
    int *tamanhos;
    char *** matrix;
} FICHEIROCOD;

typedef struct{
    char *nome;
    char **buffer;
} FICHEIROORIGINAL;


SINAL moduloC(char* file_name);
int skip_inicial(char *buffer);
int skip_arroba(int nArroba,int index,char *buffer);
int skip_semicolon(int index,char *buffer);
char *cod_to_buffer(FILE*cod);
FICHEIROCOD matrix_code(char *buffer);
FICHEIROORIGINAL file_to_buffers(FILE *fp,FICHEIROCOD cod,FICHEIROORIGINAL ficheiro);

#endif //SHAFA_C_H
