#include <stdio.h>

#include "Common/utils.h"

int main(int argc, char *argv[]) {
    SINAL s = OK;
    FICHEIRO* f = inicializar_ficheiro();
    char* file_name = "";
    char* option = "";
    char* module = "";

    if (argc >= 4)
    {
        file_name = argv[1];
        option = argv[2];
        module = argv[3];

        s = loadFile(f, file_name, 1024);

        printf("Nº total de blocos: %lld\n", f->n_blocks);
        printf("%lld blocos de %lu, sendo o ultimo de %lu dando um total de %llu\n",
                f->n_blocks -1, f->block_size, f->last_block_size, f->total_data_size);

        printFileData(f);
    }

    switch (s)
    {
        case OK:
            break;
        case FICHEIRO_DEMASIADO_PEQUENO:
            printf("O ficheiro de input tem menos de 1KByte, a terminar...\n");
            break;
        case ERRO_IO:
            printf("Erro de IO.\n");
            break;
    }

    printf("\nfn: %s, opt: %s, modulo: %s\n", file_name, option, module);
    printf("Fim da execução\n");
    return 0;
}
