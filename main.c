#include <stdio.h>


#include "string.h"

#include "ModuloA/a.h"
#include "ModuloB/b.h"
#include "ModuloC/c.h"
#include "ModuloD/d.h"
#include "Common/dados.h"

int main(int argc, char *argv[]) {
    SINAL s = OK;
    char* file_name = "";
    char* option = "";
    char module;

    if (argc == 1) printf("Tem que especificar pelo menos o nome do ficheiro de input.\n");
    if (argc > 1 && ( !strcmp(argv[1], "-m") || !strcmp(argv[1], "-c") || !strcmp(argv[1], "-b")))
    {
        printf("usage: %s <nomeficheiro> -m {a,b,c,d} -[c|b] ...\n", argv[0]);
        exit(0);
    }
    if (argc > 1)
    {
        file_name = argv[1];
        FILE* fp = fopen(file_name, "r");
        if (!fp) { // testa a abertura do ficheiro dado
            printf("Incapaz de abrir o ficheiro %s\n A terminar...\n", file_name);
            exit(0);
        }

        if (argc == 2) moduloA(file_name); // executa sempre primeiro o modulo A se não especificado
        if (argc > 2){
            option = argv[2]; // == "-m"
            module = *argv[3]; // {f,t,c,d}
            if ( module == 'f' || module == 't' || module == 'c' || module == 'd' )
            {
                if (module == 'f')
                {
                    // todo: captar argumentos como [-c r] e [-b K|m|M]
                    s = moduloA(file_name);
                }
                if (module == 't') s = moduloB(file_name);
                if (module == 'c') s = moduloC(file_name);
                if (module == 'd') s = moduloD(file_name);
            } else {
                printf("Modulo tem que ser um dos seguintes: {f,t,c,d}\n");
                exit(0);
            }

        }
    }

    /*if (argc >= 4)
    {

        printf("Nº total de blocos: %lld\n", f->n_blocks);
        printf("%lld blocos de %lu, sendo o ultimo de %lu dando um total de %llu\n",
                f->n_blocks -1, f->block_size, f->last_block_size, f->total_data_size);
    }*/

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

    //printf("\nfn: %s, opt: %s, modulo: %s\n", file_name, option, module);
    printf("Fim da execução\n");
    return 0;
}
