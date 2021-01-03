#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "a.h"
//#include "ModuloB/b.h"
#include "c.h"
#include "d.h"
#include "dados.h"
#include "utils.h"

int main () {

    int argc = 4;
    char* argv[] = {"./shafa","aaa.txt","-m","c"};

    int default_size = 65536;
    int force_RLE = 0; // Se estiver a 0, a compressão RLE não é forçada. Se estiver a 1, a compressão RLE é forçada.
    int descm_RLE = 0; // Se estiver a 1, apenas realiza a descompressão RLE.
    int descm_SF = 0;  // Se estiver a 1, apenas realiza a descompressão SF.
    char* file_name = argv[1];

    if (argc == 1) {
        printf("Tem que especificar pelo menos o nome do ficheiro de input.\n");
        return 0;
    }
    if (argc == 2) { // executamos os módulos todos
        moduloA(file_name, default_size, force_RLE);
     //   moduloB(file_name);
        moduloC(file_name);
        moduloD(file_name, descm_RLE, descm_SF);
        return 0;
    }

    if (argc > 2) { // Ciclo para ativar as flags que vão alterar os tamanhos dos blocos ou alterar o funcionamento normal do programa.
        int i = 2;
        while (i < argc) {
            if (argv[i][1] == 'c') { 
                force_RLE = 1; 
                i++;
            }
            else if (argv[i][1] == 'b') {
                 i++;
                 if (argv[i][0] == 'K') default_size = 655360;
                 else if (argv[i][0] == 'm') default_size = 8388608;
                 else if (argv[i][0] == 'M') default_size = 67108864;
                 i++;
            }
            else if (argv[i][1] == 'd') {
                 i++;
                 if (argv[i][0] == 'r') descm_RLE = 1;
                 else if (argv[i][0] == 's') descm_SF = 1;
                 i++;
            }
        i++;
        }
    }

    if (argv[2][1] == 'm') { // Condição para testar se foi chamado apenas um módulo.
        if (argv[3][0] == 'f') moduloA(file_name, default_size, force_RLE);
    //    else if (argv[3][0] == 't') moduloB(file_name);
        else if (argv[3][0] == 'c') moduloC(file_name);
        else if (argv[3][0] == 'd') moduloD(file_name, descm_RLE, descm_SF);
    }
    else { // Caso contrário, executa o programa todo.
        moduloA(file_name, default_size, force_RLE);
    //  moduloB(file_name);
        moduloC(file_name);
        moduloD(file_name, descm_RLE, descm_SF);
    }
}
