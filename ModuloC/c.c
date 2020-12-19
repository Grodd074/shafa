#include "c.h"
#include "stdio.h"
SINAL moduloC(char* file_name)
{
    printf("MODULO C\n");

}

SINAL matrix_code(char *nome){

    int qtlidas, n_blocos, size, x = 0, y = 0;
    
    char c, modo;
    char *binstr;
    FILE *fp = fopen(nome,"r");
    qtlidas = fscanf(fp,"@%c@%lld@%lu@",&modo,&n_blocos,&size);
    unsigned long arrblocos[n_blocos];
    arrblocos[0] = size;

    char *matrix[n_blocos][256];

    char *temp = binstr;
    int index = 1;
    for(int i = n_blocos;((c = fget(fp)) != EOF) && i > 0;i--){
        if(c == ';'){
            binstr = temp;
            matrix[x][y] = binstr;
            y++;
        }
        else if(c == '@'){
            qtlidas = fscanf(fp,"@%lu@",&size);
            arrblocos[index] = size;
            index++;
            y = 0;
            x++;
        }
        else{
            (*binstr) = c;
            binstr++;
        }
    }    
}

