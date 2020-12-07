#include "utils.h"
#include <string.h>
#define DEBUG

SINAL loadFicheiro (FICHEIRO* f, char* file_name)
{
    SINAL s = OK;
    FILE *fp;
    int cr; // close result
    char c; // caracter de leitura
    int i=0; // iterador

    //Abrir ficheiro
    fp = fopen(file_name, "r");
    if (!fp) s = ERRO_IO;
    else
    {
        PRINT_DEBUG("%s aberto com sucesso\n", file_name);

        // Le quantos caracters tem o ficheiro
        fseek( fp , 0 , SEEK_END);
        int size = ftell( fp );
        rewind( fp );
        PRINT_DEBUG("file size %d\n", size);

        // Alloca espaço no array de data para os caracters
        f->size = size;
        f->data = malloc(sizeof(char) * size);

        // le para dentro do array data
        while ( (c = fgetc(fp)) != EOF)
        {
            f->data[i] = c;
            i++;
        }

        cr = fclose(fp);
        if (cr == 0) PRINT_DEBUG("%s fechado com sucesso\n", file_name);
        else s = ERRO_IO;
    }

    return s;
}
