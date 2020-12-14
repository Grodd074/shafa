#include "b.h"
#include "stdio.h"
#include "../Common/utils.h"
SINAL moduloB(char* file_name)
{
    SINAL s;
    SYMBSFREQ* symbs = NULL;
    symbs = inicializarSymbFreq();
    s = readFreqs(symbs, file_name);
    printSymbsFreq(symbs);
    return s;
}

void readMetaData (FILE* fp, SYMBSFREQ* symbs)
{
    char rle;
    long long n_blocks;
    unsigned long block_size;

    fscanf(fp, "@%c@%lld@%lu@", &rle, &n_blocks, &block_size);
    symbs->rle_compression = rle=='R'? 1:0;
    symbs->n_blocks = n_blocks;
    symbs->block_size = block_size;
    // calcular last_block_size?
}


SINAL readFreqs(SYMBSFREQ* symbs, char* file_name)
{
    SINAL s;
    FILE* fp = fopen(file_name, "r");
    if(!fp){
        printf("Failed to open file %s", file_name);
        s = ERRO_IO;
    }

    readMetaData(fp, symbs);
    allocateFreqsBlocks(symbs, symbs->n_blocks);
    for(int i=0; i<symbs->n_blocks; i++){
        readFreq(fp, symbs->freqs[i], 256);
    }

    return s;
}


/* assume que o proximo elemento a ler e a frequencia do primeiro simbolo
 * assume que fp está aberto
 */
void readFreq(FILE* fp, int* arr, int SIZE)
{
    int r;
    int n;
    char c;
    for(int i=0; i<SIZE; i++)
    {
        // 1. se a leitura for de um numero:
        //	  da scanf do nº de freq
        //	  passa o ponto e virgula a frente
        //	  mete no array o nº lido
        // 2. se a leitura der erro (não for um nº)
        //	  passa o ponto e virgula a frente
        //	  mete o nº anterior nesta posicao do array
        r = fscanf(fp, "%d", &n);
        c = fgetc(fp);
        arr[i] = n;
    }

    printf("%c\n", c);
    // prepara o fp para que as precondiçoes desta funçao sejam satisfeitas
    int lixo;
    fscanf(fp, "%d@", &lixo);
}
