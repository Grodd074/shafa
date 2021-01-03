#ifndef SHAFA_MODULOB_B_H
#define SHAFA_MODULOB_B_H

#include <stdio.h>
#include "dados.h"

/* SYMBSFREQ
 * Estrutura que contem toda a raw data numa matriz em que cada array dessa matriz tem 256
 * inteiros que correspondem as frequencias lidas do ficheiro .freq, por ordem de leitura
 * assim como toda a outra informacao a retirar do ficheiro .freq
*/
typedef struct
{
    bool rle_compression;
    long long n_blocks;
    unsigned long block_size;
    long last_block_size;
    int** freqs;
} SYMBSFREQ;

/* Estrutura do tipo lista ligada que serve para armazenar as strings
 * de caracteres '0' e '1' dos codigos shafa
*/
typedef struct BINSTRNODE
{
    char c;
    struct BINSTRNODE* prox;
} *LBINSTR;

/* Estrutura do tipo lista ligada onde cada nodo contem
 * o simbolo e a sua respetiva frequencia, assim como mais tarde o seu respetivo
 * codigo shannon-fannon (armazenado na forma de uma lista ligada)
*/
typedef struct SYMBFREQNODE
{
    unsigned char symb;
    int freq;
    LBINSTR bin_str;
    struct SYMBFREQNODE* prox;
} *LSYMBFREQ;

SYMBSFREQ* inicializarSymbFreq();
SYMBSFREQ* allocateFreqsBlocks(SYMBSFREQ* s, long long n);

// Funcao que executa todos os passos para executar este modulo do programa
SINAL moduloB(char* file_name);

// Passo 1: Funcoes de leitura
// readMetaData: funcao que le a primeira metadata do ficheiro freq e move o "cursor" para o inicio do bloco
void readMetaData (FILE* fp, char* file_name, SYMBSFREQ* symbs);
// readFreq: funcao que le para "arr" as 256 frequencias de um bloco
void readFreq(FILE* fp, int* arr, int SIZE);
// readFreqs: funcao que utiliza num loop a readFreq para ler todos os blocos colocando essa informacao na estrutura symbs
SINAL readFreqs(SYMBSFREQ* symbs, char* file_name);
// findLastBlockSize: funcao que encontra o tamanho do ultimo bloco, procurando por o penultimo '@' do ficheiro .freq
long findLastBlockSize(char* file_name);

// Passo 2: Tratamento dos dados
// criarL: Para uma array de 256 frequencias, transforma-a numa lligada
void criarL(LSYMBFREQ* l, int* block);
// sortLDecFreq: Organiza a lligada por ordem decrescente de freq de forma a poder ser utilizada no algoritmo shafa
void sortLDecFreq (LSYMBFREQ* l);
// sortLLexiographically: organiza a lligada por lexiografica para simplificar a escrita no ficheiro
void sortLLexiographically (LSYMBFREQ* l);
// llSymbFreq: devolve array de lligadas com todos os blocos, cada posicao no array e um bloco de frequencias armazenado numa lligada
LSYMBFREQ* llSymbFreq(SYMBSFREQ* symbs);
// llParaArray: converte de lligada de volta para um array
int llParaArray(LSYMBFREQ l, int ** arr);
// fillLSymbFreqWithBin: preenche a estrutura LSYMBFREQ com os codigos shafo correspondentes a cada simbolo
void fillLSymbFreqWithBin (LSYMBFREQ* l, LBINSTR* codigos);

int calcularMelhorDivisao (int * freqs, int i, int j);
void addBitToCodes(char bit, LBINSTR* codes, int start, int end);
void calcularCodigosSF (int* freqs, LBINSTR* codes, int start, int end); // para um bloco

// Passo 3: Escrita do ficheiro .cod
void escreveMetadata (FILE* fp, SYMBSFREQ* symbs);
void escreveCod (char* file_name, SYMBSFREQ* symbs, LSYMBFREQ* l);
char* nomeDoFicheiro(char *file_name);

// Funcoes auxiliares
int fileSize(char* file_name);
int soma(int* arr, int i, int j);
void printInfoModulo (char *file_name, SYMBSFREQ* symbs, double time_spent);

#endif //SHAFA_MODULOB_B_H
