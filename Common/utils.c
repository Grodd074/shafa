#include "utils.h"
#include <string.h>
#include "fsize.h"
#include <math.h>

SINAL loadFileMem(FICHEIRO* f, char* file_name)
{
    SINAL s = OK;
    FILE *fp;
    int cr; // close result
    char c; // caracter de leitura
    double it=0; // iterador

    //Abrir ficheiro
    fp = fopen(file_name, "r");
    if (!fp) s = ERRO_IO;
    else
    {
        PRINT_DEBUG("%s aberto com sucesso\n", file_name);

        // Alloca espaço no array de data para os caracters
        if (!f->ultimo_absorvido) // O ultimo bloco e maior que 1Kb
        {
            f->data = malloc(f->n_blocks * sizeof(char *));
            for (int i=0; i<f->n_blocks; i++)
                f->data[i] = malloc(f->block_size * sizeof(char));
        } else { // Quando o penultimo absorve o ultimo, ficando maior que os outros
            f->data = malloc((f->n_blocks-1) * sizeof(char *));
            int i;
            for (i=0; i<(f->n_blocks-1); i++)
                f->data[i] = malloc(f->block_size * sizeof(char));
            f->data[i] = malloc( (f->block_size+f->last_block_size) * sizeof(char));
            (f->n_blocks)--;
            f->last_block_size += f->block_size;
        }

        // le para dentro do array data
        while ( (c = fgetc(fp)) != EOF)
        {
            int bloco_pertencente = floor(it / f->block_size);
            int pos_no_array = it - (bloco_pertencente*f->block_size);

            if ( (f->ultimo_absorvido) && (bloco_pertencente >= f->n_blocks-1) )
            {
                // no caso de quando o penultimo array absorveu o ultimo por ter menos de 1kb (tornando-se o ultimo)
                // copiar o resto da informaçao ate ao fim do ficheiro para este bloco
                pos_no_array = it - ( (f->n_blocks-1)*(f->block_size) );
                f->data[f->n_blocks-1][pos_no_array] = c;
            } else
                f->data[bloco_pertencente][pos_no_array] = c;
            it++;
        }

        cr = fclose(fp);
        if (cr == 0) PRINT_DEBUG("%s fechado com sucesso\n", file_name);
        else s = ERRO_IO;
    }

    return s;
}

SINAL loadFile (FICHEIRO* f, char* file_name, unsigned long block_size)
{
    SINAL s;
    unsigned long long total;
    long long n_blocks;
    long size_of_last_block;

    if (block_size < FSIZE_MIN_BLOCK_SIZE)
    {
        if (block_size == 0)
            printf("Input block_size: %lu ---> Avançar com default block size = 65536\n", block_size);
        else
            printf("Input block_size: %lu ---> Block Size Minimo de 1024. Avançar com block_size = 1024\n", block_size);
    }

    n_blocks = fsize(NULL, file_name, &block_size, &size_of_last_block);
    total = (n_blocks-1) * block_size + size_of_last_block;

    switch (n_blocks)
    {
        case FSIZE_ERROR_BLOCK_SIZE:
            printf("Error: Block size is larger than max value\n");
            break;
        case FSIZE_ERROR_NUMBER_OF_BLOCKS:
            printf("Error: Number of Blocks exceeds max value permitted\n");
            break;
        case FSIZE_ERROR_IN_FILE:
            printf("Error: Opening or reading file\n");
            break;
        case FSIZE_ERROR_IN_FTELL:
            printf("Error: When using ftell()\n");
            break;
    }

    if (total < MIN_FILE_SIZE) s = FICHEIRO_DEMASIADO_PEQUENO; // Se == 1Kb executa

    f->n_blocks = n_blocks;
    f->block_size = block_size;
    f->last_block_size = size_of_last_block;
    f->total_data_size = total;
    f->ultimo_absorvido = f->last_block_size < FSIZE_MIN_BLOCK_SIZE;

    s = loadFileMem(f, file_name);

    return s;
}

void printBlockData (char* block, int block_size)
{
    for (int i=0; i<block_size; i++)
        printf("%c", block[i]);
}


void printFileData (FICHEIRO* f)
{
    for(int i=0; i<f->n_blocks-1; i++){
        printBlockData(f->data[i], f->block_size);
        printf("\n");
    }

    printBlockData(f->data[f->n_blocks-1], f->last_block_size);
    printf("\n");
}
