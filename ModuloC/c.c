#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "c.h"


//Tratamento ficheiro cod

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

int cod_to_buffer(char**buffer,FILE*cod){
    int size_cod;
    fseek(cod,0L,SEEK_END);
    size_cod=ftell(cod);
    *buffer=malloc(size_cod);
    fseek(cod,0L,SEEK_SET);
    fread(*buffer,size_cod,1,cod);
    return size_cod;
}

FICHEIROCOD *matrix_code(int size,char *buffer){
    FICHEIROCOD *ficheiro=malloc(sizeof(FICHEIROCOD));
    char modo;
    int iLinha=(-1),iColuna,iArray,iTamanhos=0,n_blocos=0,cod_size=0,max=0,bloco_size=0;
    sscanf(buffer,"@%c@%d",&modo,&(n_blocos));
    ficheiro->tamanhos=malloc(sizeof(int)*n_blocos);
    ficheiro->n_blocos=n_blocos;
    ficheiro->modo = (modo=='R') ? true : false; 
    ficheiro->matrix=malloc(sizeof(char*)*(ficheiro->n_blocos));
    char *codigo=malloc(256*sizeof(char));
    for(iArray=skip_inicial(buffer);iArray<size;){
        if(*(buffer+iArray)==';'){
            iArray++;
            iColuna++;
            ficheiro->matrix[iLinha][iColuna]="";
        }
        else if((*(buffer+iArray)=='@')&&(*(buffer+iArray+1)=='0')){
            ficheiro->max_cod_size=max;
            return ficheiro;
        }
          else if(*(buffer+iArray)=='@'){
            iColuna=0;
            iLinha++;
            ficheiro->matrix[iLinha]=malloc(256*sizeof(char*));
            ficheiro->matrix[iLinha][iColuna]="";
            sscanf((buffer+iArray),"@%d@",&bloco_size);
            ficheiro->tamanhos[iTamanhos]=bloco_size;
            iTamanhos++;
            iArray=skip_arroba(1,iArray,buffer);
        }
        else{
            sscanf((buffer+iArray),"%[^;]",codigo);
            cod_size=strlen(codigo);
            if(cod_size>max) max=cod_size;
            ficheiro->matrix[iLinha][iColuna]=malloc(sizeof(char)*cod_size);
            strcpy(ficheiro->matrix[iLinha][iColuna],codigo);
            iArray=skip_semicolon(iArray,buffer);
            codigo[0]='\0';
        }
    }
    ficheiro->max_cod_size=max;
    return ficheiro;
    
}



//Tratamento ficheiro original

FICHEIROORIGINAL file_to_buffers(FILE *fp, FICHEIROCOD cod, FICHEIROORIGINAL ficheiro){
    int nblocos = cod.n_blocos, sizeblocos;
    bool modo = cod.modo;
    fseek(fp, 0L, SEEK_SET);
    unsigned char **buffer_file = malloc(sizeof(char *) * nblocos);
    for (int i = 0; i < nblocos; i++){
        sizeblocos = cod.tamanhos[i];
        buffer_file[i] = malloc(sizeof(char) * sizeblocos);
        fread(buffer_file[i], sizeblocos, 1, fp);
    }
    ficheiro.buffer = buffer_file;
    return ficheiro;
}



//Tratamento ficheiro final

unsigned char char_to_print(int size,char* string){
    int c=0,i=0,k=7,n=0;
    unsigned char print;
    for(;i<size;i++){
        n=string[i]-'0';
        c+=n<<k;
        k--;
    }
    print=c;
    return print;
}

int newstring(int tam_ant,int nbloco,char*bloco_original,FICHEIROCOD cod,char*output){
    int index,tam_dep=0,size_cod=0,new_string_index=0,counter=0,max=cod.max_cod_size;
    unsigned char car;
    char *codigo=malloc(sizeof(char)*(max+1));
    int i=0;
    for(index=0;index<tam_ant;index++){
        car=bloco_original[index];
        size_cod=strlen(cod.matrix[nbloco][car]);
        codigo=cod.matrix[nbloco][car];
        for(counter=0;counter<size_cod;counter++){
            output[new_string_index+counter]=codigo[counter];
        }
        codigo[counter]='\0';
        new_string_index+=counter;
    }
    return new_string_index;
}

void printfile(FICHEIROCOD cod,FICHEIROORIGINAL file){
    int i=0,tam,max=0,new_size=0,tamres=0,allocsize=0;
    max=cod.max_cod_size;
    unsigned char c;
    unsigned char*str=malloc(sizeof(char)*256);
    strcat(str,file.nome);
    strcat(str,".shaf");
    FILE *fp;
    fp=fopen(str,"wb");
    fprintf(fp,"@%d",cod.n_blocos);
    unsigned char*output;
    for(i=0;i<cod.n_blocos;i++){
        tam=cod.tamanhos[i];
        allocsize=sizeof(char)*cod.tamanhos[i]*max;
        output=malloc(allocsize);
        new_size=newstring(tam,i,file.buffer[i],cod,output);

        if(new_size%8) fprintf(fp,"@%d@",(new_size/8)+1);
        else fprintf(fp,"@%d@",new_size/8);

        for(int counter=0;counter<new_size;){
            tamres=new_size-counter;
            if(tamres<8){
                c=char_to_print(tamres,output+counter);
                fputc(c,fp);
            }
            else{
                c=char_to_print(8,output+counter);
                fputc(c,fp);
            }
            counter+=8;
        }
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


SINAL moduloC(char *file_name){
    clock_t tempoOrd;
    tempoOrd = clock();

    printf("MODULO C\n\n");

    int i = strlen(file_name);
    char *str = malloc(i + 4);
    strcat(str, file_name);
    strcat(str, ".cod");

    FILE *file;
    FILE *cod;
    file = fopen(file_name, "rb");
    cod = fopen(str, "rb");

    if (cod != NULL){
        FICHEIROCOD codfile;
        FICHEIROORIGINAL originfile;
        originfile.nome = file_name;
        char *buffer = '\0';
        int size = cod_to_buffer(&buffer, cod);
        codfile = *matrix_code(size, buffer);
        int *tamanhos = malloc(sizeof(int) * (codfile.n_blocos));
        for (int j = 0; j < codfile.n_blocos; j++)
            tamanhos[j] = codfile.tamanhos[j];
        originfile = file_to_buffers(file, codfile, originfile);
        printfile(codfile, originfile);
        fclose(cod);
        fclose(file);
        data_console(originfile, codfile, tamanhos, ((clock() - tempoOrd) / (double)CLOCKS_PER_SEC));
    }
    else{
        printf("Não existe o ficheiro %s\n", str);
    }
}
