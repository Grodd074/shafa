#ifndef SHAFA_A_H
#define SHAFA_A_H

#include "../Common/dados.h"

/**
 * @char *file_name - Nome do Ficheiro Original
 * return Nome do Ficheiro.rle
*/
char *nomeRLE(char *file_name);

unsigned char *criarBufferArrayRLE(char *file_name, int *BlocosRLE, int bloco);

/**
 * @char *file_name - Nome do Ficheiro Original
 * return Nome do Ficheiro.freq
*/
char *nomeFREQ(char *file_name);

/**
 * @char *file_name - Nome do Ficheiro
 * return numero de caracteres
*/
int tamanhoFicheiro(char *file_name);

/**
 * @char *file_name - Nome do Ficheiro
 * int *blocosRLE - Buffer para guardar o tamanho de cada bloco após efetuada a Compresão RLE
 * int bloco - Numero do Bloco que está a ler
 * return numero de caracteres
*/
unsigned char *criarBufferArrayRLE(char *file_name, int *BlocosRLE, int bloco);
/**
 * @char *file_name - Nome do Ficheiro
 * int size - Tamanho dos Blocos
 * int bloco - Numero do Bloco que está a ler
 * return buffer 
*/
unsigned char *criarBufferArray(char *file_name, int size, int bloco);

/**
 * @char *file_name - Nome do Ficheiro
 * int size - Tamanho dos Blocos
 * int sizeFinal - Tamanho do Ultimo Bloco
 * int bloco - Numero do Bloco que está a ler
 * return buffer 
*/
unsigned char *criarBufferArrayFinal(char *file_name, int size, int sizeFinal, int bloco);

/**
 * @char *file_name - Nome do Ficheiro
 * return buffer 
*/
int **criarBufferMatriz(int numeroBlocos);

/**
 * @char *file_name - Nome do Ficheiro
 * unsigned char *v - Buffer que contem o bloco em memória
 * int size - Tamanho dos Blocos
 * int bloco - Numero do Bloco que está a ler
 * return Numero de caracteres após compressão do bloco
*/
int compressaoRLE(char *file_name, unsigned char *v, int size, int bloco);

/**
 * int cZipped - Numero de Caracteres do Bloco após Compressão RLE
 * unsigned char *v - Buffer que contem o bloco em memória
 * return Taxa de Compressão
*/
double taxaCompressao(int cZipped, unsigned char *v);

/**
 * int tamanhoFicheiro - Tamanho do ficheiro original
 * int size - Tamanho definido dos blocos
 * return Numero de Blocos
*/
int quantidadeBlocos(int tamanhoFicheiro, int size);

/**
 * int tamanhoFicheiro - Tamanho do ficheiro original
 * int size - Tamanho definido dos blocos
 * return Tamanho do Ultimo Bloco
*/
int imprimeTamanhoBlocos(int tamanhoFicheiro, int size);

/**
 * int tamanhoFicheiro - Tamanho do ficheiro original
 * int size - Tamanho definido dos blocos
 * return Tamanho do Ultimo Bloco
*/
int tamanhoUltimoBloco(int tamanhoFicheiro, int size);

/**
 * unsigned char *v - Buffer que contem o bloco em memória
 * int bloco - Numero do Bloco que está a ler
 * int **j - Buffer que contem as frequencias de cada caracter para cada bloco
 * return Buffer
*/
int **frequenciaCalculo(unsigned char *v, int bloco, int **j);

/**
 * @char *file_name - Nome do Ficheiro
 * int *blocosRLE - Buffer para guardar o tamanho de cada bloco após efetuada a Compresão RLE
 * int BlocosLength - Número de Blocos em que o ficheiro foi dividido
 * return numero de caracteres
*/
int **calculoFrequenciaRLE(char *file_name, int *blocosRLE, int BlocosLength);

/**
 * @char *file_name - Nome do Ficheiro
 * char k - char que indica se foi efetua a compressão RLE
 * int *blocosRLE - Buffer para guardar o tamanho de cada bloco após efetuada a Compresão RLE
 * int bloco - Numero do Bloco que está a ler
 * return numero de caracteres
*/
int funcaoFrequenciaEscritaRLE(char *file_name, char k, int BlocosLength, int *blocosRLE);

/**
 * @char *file_name - Nome do Ficheiro
 * int **j - Buffer que contem as frequencias de cada caracter para cada bloco
 * char k - char que indica se foi efetua a compressão RLE
 * int tamanhoFicheiro - Tamanho do ficheiro original
 * int size - Tamanho definido dos blocos
 * int numeroblocos - Indicativo de em quantos blocos o ficheiro original foi dividido
 * return Buffer
 * return 0
*/
int funcaoFrequenciaEscrita(char *file_name, int **j, char k, int tamanhoFicheiro, int size, int numeroblocos);

/**
 * int *blocosRLE - Buffer para guardar o tamanho de cada bloco após efetuada a Compresão RLE
 * int BlocosLength - Indicativo de em quantos blocos o ficheiro original foi dividido
*/
void imprimeTamanhoBlocosRLE(int *blocosRLE, int BlocosLength);

/**
 * @char *file_name - Nome do Ficheiro
 * char m - char que indica se foi efetua a compressão RLE
 * int BlocosLength - Indicativo de em quantos blocos o ficheiro original foi dividido
 * int FileLength - Indicativo do Tamanho do ficheiro original
 * int size - Tamanho definido dos blocos
 * int taxa - Indicativo da taxa de compressão efetuada no primeiro bloco
 * int *blocosRLE - Buffer para guardar o tamanho de cada bloco após efetuada a Compresão RLE
*/
void imprimeTerminal(char *file_name, char m, int BlocosLength, int FileLength, int size, int taxa, int *blocosRle);

/**
 * @char *file_name - Nome do Ficheiro
 * int size - Tamanho definido dos blocos
 * return 0
*/
int forca_compressao(char *file_name, int size);

/**
 * @char *file_name - Nome do Ficheiro
 * int size - Tamanho definido dos blocos
 * return 0
*/
int normal_compressao(char *file_name, int size);

/**
 * @char *file_name - Nome do Ficheiro
 * int default_size - Tamanho definido dos blocos
 * int force_RLE - Indicativo se foi forçada a compressão RLE
*/
int moduloA(char *file_name, int default_size, int force_RLE);

#endif //SHAFA_A_H
