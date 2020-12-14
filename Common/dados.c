#include "dados.h"

FICHEIRO* inicializaFicheiro()
{
    FICHEIRO* f = malloc(sizeof(FICHEIRO));
    f->total_data_size = 0;
    f->block_size = 65536; // Default de 64 Kbytes
    f->last_block_size = 0;
    f->n_blocks = 0;
    f->data = NULL;
    f->ultimo_absorvido = false;
}

SYMBSFREQ* inicializarSymbFreq()
{
    SYMBSFREQ* s = malloc(sizeof(SYMBSFREQ));
    s->rle_compression = false;
    s->n_blocks = 0;
    s->block_size = 65536; // Default de 64 Kbytes
    s->last_block_size = 0;
    s->freqs = NULL;

    return s;
}

SYMBSFREQ* allocateFreqsBlocks(SYMBSFREQ* s, long long n)
{
    s->freqs = malloc(n * sizeof(int*));
    for(int i=0;i<n;i++){
        s->freqs[i] = malloc(sizeof(int)*256);
    }
}
