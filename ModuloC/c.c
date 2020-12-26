#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "c.h"
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

FICHEIROCOD matrix_code(char *buffer){
    FICHEIROCOD ficheiro;
    char modo;
    int nblocos,size=0,iLinha=(-1),iColuna,iArray,iTamanhos=0;
    int *tamanhos=malloc(nblocos*sizeof(int));
    sscanf(buffer,"@%c@%d",&modo,&nblocos);
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
            ficheiro.matrix=matrix;
            ficheiro.n_blocos=nblocos;
            ficheiro.tamanhos=tamanhos;
            ficheiro.modo = (modo=='R') ? true : false; 
            return ficheiro;
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

FICHEIROORIGINAL file_to_buffers(FILE *fp,FICHEIROCOD cod,FICHEIROORIGINAL ficheiro){
    int nblocos=cod.n_blocos,sizeblocos=cod.tamanhos[0];
    bool modo=cod.modo;
    fseek(fp,0L,SEEK_SET);
    char**buffer_file=malloc(sizeof(char*)*nblocos);
    for (int i=0; i < nblocos; i++){
        buffer_file[i]=malloc(sizeof(char)*sizeblocos);
        fread(buffer_file[i],sizeblocos,1,fp);
    }
    ficheiro.buffer=buffer_file;
    return ficheiro;
}

int max_size(char**blocomatrix){
    int max=0,i;
    for(i=0;blocomatrix[i];i++){
        if(strlen(blocomatrix[i])>max) max=strlen(blocomatrix[i]);
    }
    return max;
}

int max_size_matriz(char***matrix){
    int max=0,i,j;
    for(i=0;matrix[i];i++){
        for(j=0;matrix[i][j];j++){
            if(strlen(matrix[i][j])>max){
                max=strlen(matrix[i][j]);
            }
        }
    }
    return max;
}

char *string_to_print(int bloco,char**buffer,char***matrix){
    int i=0,tam=2048;
    char *string=malloc(sizeof(char)*max_size(matrix[bloco])*tam);
    string='\0';
    char c;
    for(;(i<tam)&&(buffer[bloco][i]!='\0');i++){
        c=buffer[bloco][i];
        strcat(string,matrix[bloco][(int)c]);
    }
    return string;
}

char char_to_print(char* string){
    int c=0,i=0,k=7,n=0;
    char print;
    for(;i<8;i++){
        n=string[i]-'0';
        c+=n<<k;
        k--;
    }
    print=(char)c;
    return print;
}

void printfile(FICHEIROCOD cod,FICHEIROORIGINAL file){
    int i=0,k=0,tam=cod.tamanhos[0],nblocos=cod.n_blocos;
    char*temp=malloc(sizeof(char)*max_size_matriz(cod.matrix)*tam);
    temp='\0';
    char c;
    char*str=malloc(sizeof(char)*256);
    int *tamanhos=malloc(sizeof(int)*nblocos);
    strcpy(str,file.nome);
    strcat(str,".shaf");
    FILE *fp;
    fp=fopen(str,"w");
    fprintf(fp,"@%d",&nblocos);
    for(;i<(cod.n_blocos);i++){
        fprintf(fp,"@%d@",tamanhos[i]);
        temp=string_to_print(i,file.buffer,cod.matrix);
        for(;temp[k];k+=8){
            c=char_to_print((temp+k));
            fputc(c,fp);
        }
        temp='\0';
    }
    fclose(fp);
}