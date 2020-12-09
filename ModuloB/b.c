#include "b.h"
#include "stdio.h"
#include "../Common/utils.h"
SINAL moduloB(char* file_name)
{
    SINAL s;
    printf("MODULO B\n");
    FICHEIRO* f = inicializar_ficheiro();
    s = loadFile(f, file_name, 0);
    printFileData(f);
    printf("Nº total de blocos: %lld\n", f->n_blocks);
    printf("%lld blocos de %lu, sendo o ultimo de %lu dando um total de %llu\n",
           f->n_blocks -1, f->block_size, f->last_block_size, f->total_data_size);
    return s;
}
