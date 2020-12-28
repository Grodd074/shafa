#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

FICHEIROCOD *matrix_code(char *buffer){
    FICHEIROCOD *ficheiro=malloc(sizeof(FICHEIROCOD));
    char modo;
    int size=0,iLinha=(-1),iColuna,iArray,iTamanhos=0;
    ficheiro->tamanhos=malloc(sizeof(int)*(ficheiro->n_blocos));
    sscanf(buffer,"@%c@%d",&modo,&(ficheiro->n_blocos));
    ficheiro->modo = (modo=='R') ? true : false; 
    ficheiro->matrix=malloc(sizeof(char*)*ficheiro->n_blocos);
    char *codigo=malloc(256*sizeof(char));
    for(iArray=skip_inicial(buffer);*(buffer+iArray)!='\0';){
        if(*(buffer+iArray)==';'){
            iArray++;
            iColuna++;
            ficheiro->matrix[iLinha][iColuna]="";
        }
        else if((*(buffer+iArray)=='@')&&(*(buffer+iArray+1)=='0')){
            return ficheiro;
        }
          else if(*(buffer+iArray)=='@'){
            sscanf((buffer+iArray),"@%d@",&size);
            ficheiro->tamanhos[iTamanhos]=size;
            iTamanhos++;
            iArray=skip_arroba(1,iArray,buffer);
            iColuna=0;
            iLinha++;
            ficheiro->matrix[iLinha]=malloc(256*sizeof(char*));
            ficheiro->matrix[iLinha][iColuna]="";
        }
        else{
            sscanf((buffer+iArray),"%[^;]",codigo);
            ficheiro->matrix[iLinha][iColuna]=malloc(sizeof(char)*256);
            strcpy(ficheiro->matrix[iLinha][iColuna],codigo);
            iArray=skip_semicolon(iArray,buffer);
            codigo[0]='\0';
        }
    }
    return ficheiro;
    
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
    for(i=0;i<256;i++){
        if(strlen(blocomatrix[i])>max) max=strlen(blocomatrix[i]);
    }
    return max;
}

int max_size_matriz(char***matrix){
    int i,j,max=0;
    for(i=0;matrix[i];i++){
        if(max_size(matrix[i])>max) max = max_size(matrix[i]);
    }
    return max;
}

char *string_to_print(int bloco,char**buffer,char***matrix){
    int i=0,tam=2048;
    char *string=malloc(sizeof(char)*max_size(matrix[bloco])*tam);
    string[0]='\0';
    char c;
    for(;(i<tam)&&(buffer[bloco][i]!='\0');i++){
        c=buffer[bloco][i];
        strcat(string,matrix[bloco][(int)c]);
    }
    return string;
}

char char_to_print(char* string){
    int c=0,i=0,k=7,n=0,size=0;
    size=strlen(string);
    char print;
    for(;i<size;i++){
        n=string[i]-'0';
        c+=n<<k;
        k--;
    }
    print=(char)c;
    return print;
}

char *pad(char*array,int n, int size){
    for(;n;n--){
        array[size]='0';
        size++;
    }
    return array;
}

void printfile(FICHEIROCOD cod,FICHEIROORIGINAL file){
    int i=0,k=0,n=0,tam=cod.tamanhos[0],nblocos=cod.n_blocos,max=0,size=0,offset=0;
    max=max_size_matriz(cod.matrix);
    char*temp=malloc(sizeof(char)*max*tam);
    char*test=malloc(sizeof(char)*max*tam);
    temp='\0';
    char c;
    char*str=malloc(sizeof(char)*256);
    strcat(str,file.nome);
    strcat(str,".shaf");
    FILE *fp;
    fp=fopen(str,"w");
    fprintf(fp,"@%d",nblocos);
    for(;i<(cod.n_blocos);i++){
        temp=string_to_print(i,file.buffer,cod.matrix);
        size=strlen(temp);
        if((size%8))size=(size/8)+1;
        fprintf(fp,"@%d@",size);
        cod.tamanhos[i] = size;
        for(;temp[k];k+=8){
            c=char_to_print(temp+k);
            fputc(c,fp);
            test=temp+k;
        }
        temp='\0';
        k=0;
    }
    fclose(fp);
}

int data_console(FICHEIROORIGINAL origin, FICHEIROCOD cod, int *size, float tempo){
    int j = 1, n = 0, antes, depois, nbloco = cod.n_blocos;
    float taxa, taxa_global;
    printf("Daniel Constantino Faria, a93187/João Augusto Macedo Moreira, a93326, MIEI/CD, 1-jan-2021\n"
           "Módulo: c (codificação dum ficheiro de símbolos)\n"
           "Número de blocos: %d\n",nbloco);
    for (; j <= nbloco; j++, n++){
        antes = size[n];
        depois = cod.tamanhos[n];
        taxa = 100 - ((float)depois / antes * 100);
        printf("Tamanho antes/depois & taxa de compressão (bloco %d): %d/%d || %.2f%%\n", j, antes, depois, taxa);
        taxa_global += taxa;
    }
    taxa_global /= cod.n_blocos;
    printf("Taxa de compressão global: %.2f%%\n"
           "Tempo de execução do módulo (milissegundos): %f\n"
           "Ficheiro gerado: %s.shaf\n",taxa_global, tempo, origin.nome);
}


