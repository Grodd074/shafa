#include "c.h"
#include "stdio.h"
SINAL moduloC(char* file_name)
{
    printf("MODULO C\n");

}

int skip_inicial(char *buffer){
    int index=3;
    while(*(buffer+index)!='@'){
        index++;
    }
    return index;
}

int skip_arroba(int nArroba,int index,char *buffer){
    while(nArroba>=0){
        if(*(buffer+index)=='@')nArroba--;
        index++;
    }
    return index;
}

int skip_semicolon(int index,char *buffer){
    while (*(buffer+index)!=';') index++;
    return index;
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

int matrix_code(char *buffer){
    int nblocos,size=0,iLinha=(-1),iColuna,iArray,iTamanhos=0;
    int *tamanhos=malloc(nblocos*sizeof(int));
    sscanf(buffer,"@%*c@%d",&nblocos);
    char ***matrix=malloc(sizeof(char*)*nblocos);
    char *codigo=malloc(256*sizeof(char));
    for(iArray=skip_inicial(buffer);*(buffer+iArray)!='\0';){
        if(*(buffer+iArray)==';'){
            matrix[iLinha][iColuna]=malloc(sizeof(char*));
            matrix[iLinha][iColuna]='\0';
            iArray++;
            iColuna++;
        }
        else if((*(buffer+iArray)=='@')&&(*(buffer+iArray+1)=='0')){
            return 0;
        }
        else if(*(buffer+iArray)=='@'){
            sscanf((buffer+iArray),"@%d@",&size);
            tamanhos[iTamanhos]=size;
            iTamanhos++;
            iArray=skip_arroba(1,iArray,buffer);
            iColuna=0;
            iLinha++;
            matrix[iLinha]=malloc(256*sizeof(char*));
        }
        else{
            sscanf((buffer+iArray),"%[^;]",codigo);
            matrix[iLinha][iColuna]=malloc(sizeof(char)*256);
            matrix[iLinha][iColuna]=codigo;
            iArray=skip_semicolon(iArray,buffer);
            *codigo='\0';
        }
    }
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
