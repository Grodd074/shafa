#ifndef SHAFA_C_H
#define SHAFA_C_H

#include "../Common/dados.h"

typedef struct ficheirocod{
    bool modo;
    int n_blocos;
    int *tamanhos;
    char *** matrix;
    int max_cod_size;
} FICHEIROCOD;

typedef struct{
    char *nome;
    unsigned char **buffer;
} FICHEIROORIGINAL;

//Tratamento ficheiro cod

//Função para aumentar o índice do buffer para ficar a apontar para o início do primeiro bloco de códigos
int skip_inicial(char *buffer);

//Função para aumentar o índice do buffer para passar à frente um certo número de arrobas
int skip_arroba(int nArroba,int index,char *buffer);

//Função para aumentar o índice do buffer para passar à frente um código e parar no próximo ponto e vírgula
int skip_semicolon(int index,char *buffer);
int cod_to_buffer(char**buffer,FILE*cod);
FICHEIROCOD *matrix_code(int size,char *buffer);
FICHEIROORIGINAL file_to_buffers(FILE *fp, FICHEIROCOD cod, FICHEIROORIGINAL ficheiro);
unsigned char char_to_print(int size,char* string);
int newstring(int tam_ant,int nbloco,char*bloco_original,FICHEIROCOD cod,char*output);
void printfile(FICHEIROCOD cod,FICHEIROORIGINAL file);
SINAL moduloC(char *file_name);

#endif //SHAFA_C_H