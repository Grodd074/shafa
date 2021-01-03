#ifndef SHAFA_MODULOB_B_H
#define SHAFA_MODULOB_B_H

#include <stdio.h>
#include "dados.h"


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

typedef struct BINSTRNODE
{
    char c;
    struct BINSTRNODE* prox;
} *LBINSTR;

typedef struct SYMBFREQNODE
{
    unsigned char symb;
    int freq;
    LBINSTR bin_str;
    struct SYMBFREQNODE* prox;
} *LSYMBFREQ;

FICHEIRO* inicializarFicheiro();
SYMBSFREQ* inicializarSymbFreq();
SYMBSFREQ* allocateFreqsBlocks(SYMBSFREQ* s, long long n);


SINAL moduloB(char* file_name);

void readMetaData (FILE* fp, char* file_name, SYMBSFREQ* symbs);
void readFreq(FILE* fp, int* arr, int SIZE);
SINAL readFreqs(SYMBSFREQ* symbs, char* file_name);

long findLastBlockSize(char* file_name);

void criarL(LSYMBFREQ* l, int* block); // cria lligada (symb, freqs) do bloco
void sortL (LSYMBFREQ* l); // organiza a lligada por ordem decrescente de freq
void sortLDecFreq (LSYMBFREQ* l); // organiza a lligada por ordem decrescente de freq
void sortLLexiographically (LSYMBFREQ* l); // organiza a lligada por ordem decrescente de freq

LSYMBFREQ* llSymbFreq(SYMBSFREQ* symbs); // devolve array de lligada, cada posicao no array e um bloco cada lligada
                                        // contem um par (simbolo,frequencia)
int llParaArray(LSYMBFREQ l, int ** arr);
int calcularMelhorDivisao (int * freqs, int i, int j);
void addBitToCodes(char bit, LBINSTR* codes, int start, int end);
void calcularCodigosSF (int* freqs, LBINSTR* codes, int start, int end); // para um bloco
void fillLSymbFreqWithBin (LSYMBFREQ* l, LBINSTR* codigos);

void escreveMetadata (FILE* fp, SYMBSFREQ* symbs);
void escreveCod (char* file_name, SYMBSFREQ* symbs, LSYMBFREQ* l);
char* nomeDoFicheiro(char *file_name);
void printInfoModulo (char *file_name, SYMBSFREQ* symbs, double time_spent);

int fileSize(char* file_name);
int soma(int* arr, int i, int j);

#endif //SHAFA_MODULOB_B_H
