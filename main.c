#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ModuloA/a.h"
#include "ModuloB/b.h"
#include "ModuloC/c.h"
#include "ModuloD/d.h"
#include "Common/dados.h"
#include "Common/utils.h"

int main (int argc, char* argv[]) {
    int default_size = 65536;
    int force_RLE = 0; // Se estiver a 0, a compressão RLE não é forçada. Se estiver a 1, a compressão RLE é forçada.
    int descm_RLE = 0; // Se estiver a 1, apenas realiza a descompressão RLE.
    int descm_SF = 0;  // Se estiver a 1, apenas realiza a descompressão SF.
    char* file_name = argv[1];

    if (argc == 1) printf("Tem que especificar pelo menos o nome do ficheiro de input.\n");
    if (argc == 2) { // executamos os módulos todos
        moduloA(file_name, default_size, force_RLE);
        moduloB(file_name);
        moduloC(file_name);
        moduloD(file_name, descm_RLE, descm_SF);
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
        else if (argv[3][0] == 't') moduloB(file_name);
        else if (argv[3][0] == 'c') moduloC(file_name);
        else if (argv[3][0] == 'd') moduloD(file_name, descm_RLE, descm_SF);
    }
    else { // Caso contrário, executa o programa todo.
        moduloA(file_name, default_size, force_RLE);
        moduloB(file_name);
        moduloC(file_name);
        moduloD(file_name, descm_RLE, descm_SF);
    }
}


/*
     * Input
     *       shafa <filename> [...]
     *
     * A
     *   -m f      Indicativo do módulo A - Frequências dos símbolos e Compressão RLE.
     *   -c r      Forçar compressão RLE.
     *   -b K|m|M  Indica tamanho dos blocos (640 KBytes, 8 MBytes, 64 MBytes, respetivamente).
     *               Tamanho default por bloco 64 KBytes.
     *
     * B
     *   -m t      Indicativo do módulo B - Tabelas de codificação
     *
     *             Input   .freq
     *             Output  .cod
     *
     * C
     *   -m c      Indicativo do módulo C - Codificação do ficheiro original/RLE
     *
     *             Input   original ou .rle
     *             Output  .shaf
     *
     * D
     *   -m d      Indicativo do módulo D - Descodificação do ficheiro comprimido.
     *   -d s      Apenas realiza a descompressão SF.
     *               Input   .rle.shaf ou .shaf
     *               Output  .rle ou original.
     *   -d r      Apenas realiza a descompressão RLE.
     *               Input   .rle
     *               Output  original
     *
    */

   /*
     * O argumento argv[0] é "lixo".
     * Os argumentos aparecem a partir de argv[2], inclusive.
     * Caso argc seja igual a 2, p.ex "shafa <filename>", vamos correr o programa todo.
     * Caso não hajam opções a referir um módulo em particular, vamos correr o programa todo. Atenção que
     *  isto pode acontecer caso hajam 2 argumentos ou mais.
     *   p.ex "shafa <filename> -c r", forçar compressão RLE.
     *        "shafa <filename> -b K|m|M -c r", tamanho dos blocos & força compressão RLE.
     *        "shafa <filename> -d s -b K|m|M", apenas faz descompressão SF & tamanho blocos
     *        "shafa <filename> -c r -d r", força a compressão RLE e apenas faz descompressão RLE.
     *d
    */
