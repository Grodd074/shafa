#include "c.h"
#include "stdio.h"
SINAL moduloC(char* file_name)
{
    printf("MODULO C\n");

}

char *cod_to_buffer(FILE*cod){
    int size_cod;
    fseek(cod,0L,SEEK_END);
    size_cod=ftell(cod);
    char*buffer_cod=malloc(size_cod);
    fseek(cod,0L,SEEK_SET);
    fread(buffer_cod,size_cod,1,cod);
    return buffer_cod;
}


char** file_to_buffers(FILE *fp,char*buffer_cod){
    int nblocos,sizeblocos;
    char modo;
    sscanf(buffer_cod,"@%c@%d@%d",&modo,&nblocos,&sizeblocos);
    fseek(fp,0L,SEEK_SET);
    char**buffer_file=malloc(sizeof(char*)*nblocos);
    for (int i=0; i < nblocos; i++){
        buffer_file[i]=malloc(sizeblocos);
        fread(buffer_file[i],sizeblocos,1,fp);
    }
    return buffer_file;
}



/*SINAL matrix_code(char *nome){

    int qtlidas, x = 0, y = 0;
    long long int n_blocos;
    unsigned long size;
    char c, modo;
    char *binstr;
    FILE *fp = fopen(nome,"r");
    qtlidas = fscanf(fp,"@%c@%lld@%lu@",&modo,&n_blocos,&size);
    unsigned long arrblocos[n_blocos];
    arrblocos[0] = size;

    char *matrix[n_blocos][256];

    char *temp = binstr;
    int index = 1;
    for(int i = n_blocos;((c = fgetc(fp)) != EOF) && i > 0;i--){
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
}*/

