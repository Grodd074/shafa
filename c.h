#ifndef SHAFA_C_H
#define SHAFA_C_H

#include "dados.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



//Definição de structs


//Struct para guaradar informação sobre o ficheiro cod
typedef struct ficheirocod{
    bool modo;
    int n_blocos;
    int *tamanhos;
    char *** matrix;
    int max_cod_size;
} FICHEIROCOD;

//Struct para guaradar informação sobre o ficheiro original
typedef struct{
    char *nome;
    unsigned char **buffer;
} FICHEIROORIGINAL;



//Funções para o tratamento do ficheiro cod


//Função para aumentar o índice do buffer para ficar a apontar para o início do primeiro bloco de códigos
int skip_inicial(char *buffer);

//Função para aumentar o índice do buffer para passar à frente um certo número de arrobas
int skip_arroba(int nArroba,int index,char *buffer);

//Função para aumentar o índice do buffer para passar à frente um código e parar no próximo ponto e vírgula
int skip_semicolon(int index,char *buffer);

//Função que passa o ficheiro cod para um buffer para poder ser trabalhado na memória
int cod_to_buffer(char**buffer,FILE*cod);

//Função que coloca os códigos correspondentes a cada caracter numa matriz e a devolve numa struct juntamente com outras informções
FICHEIROCOD *matrix_code(int size,char *buffer);



//Funções para o tratamento do ficheiro original


//Função que passa o ficheiro original para varios buffers(por blocos) para poder ser trabalhado na memória
FICHEIROORIGINAL file_to_buffers(FILE *fp, FICHEIROCOD cod, FICHEIROORIGINAL ficheiro);



//Funções para o tratamento do ficheiro final


//Função que devolve o caracter que uma sequencia binária codifica em ASCII
unsigned char char_to_print(int size,char* string);

//Função que recebe uma string de caracteres e os codifica
int newstring(int tam_ant,int nbloco,char*bloco_original,FICHEIROCOD cod,char*output);

//Função que imprime o ficheiro shaf
void printfile(FICHEIROCOD cod,FICHEIROORIGINAL file);



//Funções de ligação à main


//Função que informa o utilizador sobre o desempenho da compressão
int data_console(FICHEIROORIGINAL origin, FICHEIROCOD cod, int *size, float tempo);

//Função que é chamada pela main de modo a executar o Modúlo C
SINAL moduloC(char *file_name);

#endif //SHAFA_C_H