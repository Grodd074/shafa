#include <stdio.h>

#include "Common/utils.h"

int main(int argc, char *argv[]) {
    SINAL s;
    //ESTADO *e = inicializar_estado();
    FICHEIRO* f = inicializar_ficheiro();
    char* file_name = "";
    char* option = "";
    char* module = "";

    if (argc >= 4)
    {
        file_name = argv[1];
        option = argv[2];
        module = argv[3];

        s = loadFicheiro(f, file_name);
    }

    switch (s)
    {
        case OK:
            break;
        case COMANDO_INEXISTENTE:
            printf("Comando não existente ou input desformatado.\n");
            break;
        case ERRO_IO:
            printf("Erro de IO.\n");
            break;
    }

    printf("Conteudo do ficheiro:\n");
    for(int i=0; i<f->size; i++){printf("%c", f->data[i]);}
    printf("\nfn: %s, opt: %s, modulo: %s\n", file_name, option, module);
    printf("Fim da execução\n");
    return 0;
}
