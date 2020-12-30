#include "b.h"
#include "stdio.h"
#include "ctype.h"
#include "../Common/utils.h"
#include "../Common/dados.h"
#include "stdlib.h"
#include "string.h"
SINAL moduloB(char* file_name)
{
    SINAL s = OK;
    SYMBSFREQ* symbs = inicializarSymbFreq();
    s = readFreqs(symbs, file_name);

    LSYMBFREQ* l = llSymbFreq(symbs); // array de lligada (todos os blocos)
    for(int i=0; i<symbs->n_blocks; i++)
    {
        int* arr = NULL; // arr com ordem decrescente das frequencias dos symbs de um bloco
        int tamanho = llParaArray(l[i], &arr); // nº de symbs com freq>0
        LBINSTR* codigos = NULL; // codigos de um bloco (array de lligadas(strings), cada ll liga os caracters da string binaria)
        codigos = malloc(tamanho * sizeof(LBINSTR));
        calcularCodigosSF(arr, codigos, 0, tamanho-1);
        fillLSymbFreqWithBin( &(l[i]), codigos);

    }


 /*
    for (int i=0; i<tamanho; i++)
        printf("%d\n", arr[i]);

    printf("\n");



    for (int i=0;i<tamanho;i++){
        while(codigos[i] != NULL){
            printf("%c", codigos[i]->c);
            codigos[i] = codigos[i]->prox;
        }
        printf("\n");
    }
*/
    /*LSYMBFREQ* arrll = passo1(symbs);
    for(int i=0; i<symbs->n_blocks; i++) {
        while (arrll[i] != NULL)
        {
            printf("b%d %c => %d\n",i, arrll[i]->symb, arrll[i]->freq);
            arrll[i] = arrll[i]->prox;
        }
    }*/
    //sortLLexiographically(l);
    /*for(int i=0; i<symbs->n_blocks; i++) {
        while (l[i] != NULL) {
            printf("b%d %c(%d) => %d ", i, l[i]->symb, l[i]->symb, l[i]->freq);
            while (l[i]->bin_str != NULL){
                printf("%c", l[i]->bin_str->c);
                l[i]->bin_str = l[i]->bin_str->prox;
            }
            printf("\n");
            l[i] = l[i]->prox;
        }
    }*/
    escreveCod(file_name, symbs, l);

    return s;
}

void escreveMetadata (FILE* fp, SYMBSFREQ* symbs)
{
    fprintf(fp, "@%c@%lld",
            symbs->rle_compression?'R':'N',
            symbs->n_blocks);
}

void escreveCod (char* file_name, SYMBSFREQ* symbs, LSYMBFREQ* l)
{
    strcat(file_name, ".cod");
    FILE* fp = fopen(file_name, "w");
    if (!fp) printf("failed to open file: %s", file_name);
    escreveMetadata(fp, symbs);

    for (int i=0;i<symbs->n_blocks; i++)
        sortLLexiographically(&(l[i]));

    for(int i=0; i<symbs->n_blocks; i++)
    {
        int c_ant = 0;
        int c;

        if (i == symbs->n_blocks-1)
            fprintf(fp, "@%ld@", symbs->last_block_size);
        else
            fprintf(fp, "@%lu@", symbs->block_size);

        while (l[i] != NULL)
        {
            c = l[i]->symb;
            for(int i=abs(c-c_ant); i>0; i--) fputc(';', fp); // poe ';' ate chegar ao proxino sinbolo com codigo
            c_ant = c;

            while (l[i]->bin_str != NULL){ // escreve o codigo correspondente ao caracter
                fprintf(fp,"%c", l[i]->bin_str->c);
                l[i]->bin_str = l[i]->bin_str->prox;
            }

            l[i] = l[i]->prox;
        }
        for(int i=abs(255-c_ant); i>0; i--) fputc(';', fp);

    }

    fprintf(fp, "@0");
}


void fillLSymbFreqWithBin (LSYMBFREQ* l, LBINSTR* codigos)
{
    int i=0;
    while ( (*l) != NULL )
    {
        (*l)->bin_str = codigos[i];
        l = &((*l)->prox);
        i++;
    }
}

long findLastBlockSize(char* file_name)
{
    FILE* fp = fopen(file_name, "r");
    if (!fp) printf("Failed to open file %s at findLastBlockSize\n", file_name);

    int fs = fileSize(file_name);
    char source[fs + 1];

    int newLen = fread(source, sizeof(char), fs, fp);
    if ( ferror( fp ) != 0 ) {
        fputs("Error reading file", stderr);
    } else {
        source[newLen++] = '\0'; /* Just to be safe. */
    }

    // source tem o texto do ficheiro freq
    // procurar o penultimo arroba
    int arroba_count = 0;
    int multiplier = 1;
    long last_block_size = 0;
    for(int i=fs; i>0 && (arroba_count <= 2); i--)
    {
        if (source[i] == '@') arroba_count++;
        if (arroba_count == 2)
        {
            i--;
            while( (isdigit(source[i])) )
            {
                last_block_size += (source[i]-'0')*multiplier;
                multiplier*=10;
                i--;
            }
            break;
        }
    }
    fclose(fp);
    return last_block_size;
}

void readMetaData (FILE* fp, char* file_name, SYMBSFREQ* symbs)
{
    char rle;
    long long n_blocks;
    unsigned long block_size;
    long last_block_size;

    last_block_size = findLastBlockSize(file_name);

    fscanf(fp, "@%c@%lld@%lu@", &rle, &n_blocks, &block_size);
    symbs->rle_compression = rle=='R'? 1:0;
    symbs->n_blocks = n_blocks;
    symbs->block_size = block_size;
    symbs->last_block_size = last_block_size;
}


SINAL readFreqs(SYMBSFREQ* symbs, char* file_name)
{
    SINAL s;
    FILE* fp = fopen(file_name, "r");
    if(!fp){
        printf("Failed to open file %s", file_name);
        s = ERRO_IO;
    }

    readMetaData(fp, file_name, symbs);
    allocateFreqsBlocks(symbs, symbs->n_blocks);
    for(int i=0; i<symbs->n_blocks; i++){
        readFreq(fp, symbs->freqs[i], 256);
    }

    return s;
}

/* assume que o proximo elemento a ler e a frequencia do primeiro simbolo
 * assume que fp está aberto
 */
void readFreq(FILE* fp, int* arr, int SIZE)
{
    int r;
    int n;
    char c;
    for(int i=0; i<SIZE; i++)
    {
        // 1. se a leitura for de um numero:
        //	  da scanf do nº de freq
        //	  passa o ponto e virgula a frente
        //	  mete no array o nº lido
        // 2. se a leitura der erro (não for um nº)
        //	  passa o ponto e virgula a frente
        //	  mete o nº anterior nesta posicao do array
        r = fscanf(fp, "%d", &n);
        c = fgetc(fp);
        arr[i] = n;
    }

    // prepara o fp para que as precondiçoes desta funçao sejam satisfeitas
    int lixo;
    fscanf(fp, "%d@", &lixo);
}

void criarL(LSYMBFREQ* l, int* block)
{
    (*l) = NULL;

    for(int i=0; i<256; i++)
    {
        if (block[i] != 0) // criar lligada para todos os simbolos que aparecem pelo menos 1 vez
        {
            LSYMBFREQ nodo = malloc(sizeof(struct SYMBFREQNODE));
            nodo->freq = block[i];
            nodo->symb = i;
            nodo->prox = NULL;
            nodo->bin_str = NULL;
            *l = nodo;
            l = &((*l)->prox);
        }
    }
}


void swap(struct SYMBFREQNODE *a, struct SYMBFREQNODE *b)
{
    LSYMBFREQ temp = malloc((sizeof(struct SYMBFREQNODE)));
    temp->freq = a->freq;
    temp->symb = a->symb;
    temp->bin_str = a->bin_str;
    a->freq = b->freq;
    a->symb = b->symb;
    a->bin_str = b->bin_str;
    b->freq = temp->freq;
    b->symb = temp->symb;
    b->bin_str = temp->bin_str;
}

void sortLLexiographically (LSYMBFREQ* l)
{
    int swapped, i;
    LSYMBFREQ ptr1;
    LSYMBFREQ lptr = NULL;

    /* Checking for empty list */
    if (l == NULL)
        return;

    do
    {
        swapped = 0;
        ptr1 = *l;

        while (ptr1->prox != lptr)
        {
            if ( ptr1->symb > ptr1->prox->symb )
            {
                swap(ptr1, ptr1->prox);
                swapped = 1;
            }
            ptr1 = ptr1->prox;
        }
        lptr = ptr1;
    }
    while (swapped);
}

void sortLDecFreq (LSYMBFREQ* l)
{
    int swapped, i;
    LSYMBFREQ ptr1;
    LSYMBFREQ lptr = NULL;

    /* Checking for empty list */
    if (l == NULL)
        return;

    do
    {
        swapped = 0;
        ptr1 = *l;

        while (ptr1->prox != lptr)
        {
            if (ptr1->freq < ptr1->prox->freq)
            {
                swap(ptr1, ptr1->prox);
                swapped = 1;
            }
            ptr1 = ptr1->prox;
        }
        lptr = ptr1;
    }
    while (swapped);
}

LSYMBFREQ* llSymbFreq(SYMBSFREQ* symbs)
{
    // criar uma array de lligada organizada por ordem decrescente de freq
    LSYMBFREQ* arrll = NULL;
    arrll = malloc(symbs->n_blocks * sizeof(struct SYMBFREQNODE)); // duvida sizeof(struct SYMBFREQNODE*)

    for(int i=0; i<symbs->n_blocks; i++) {
        LSYMBFREQ l = NULL;
        criarL(&l, symbs->freqs[i]);
        sortLDecFreq(&l);
        arrll[i] = l;
    }
    return arrll;
}

int calcularMelhorDivisao (int * freqs, int i, int j)
{
    int div = i;
    int sum = soma(freqs, i, j);
    int total=sum,
        mindif=sum,
        dif=sum;
    int g1 = 0;
    do
    {
        g1 += freqs[div];
        dif = abs(2*g1-total);
        if (dif < mindif) {
            div++;
            mindif = dif;
        } else {
            break;
        }
    } while (true);

    return div-1;
}

int llParaArray(LSYMBFREQ l, int ** arr)
{
    int tamanho = 0;
    LSYMBFREQ pt = l;
    while (pt != NULL) { // calcula tamanho da lligada
        tamanho++;
        pt = pt->prox;
    }

    (*arr) = malloc(tamanho * sizeof(int));
    for(int i=0; l != NULL; i++){
        (*arr)[i] = l->freq;
        l = l->prox;
    }
    return tamanho;
}


void addBitToCode(char bit, LBINSTR* code)
{
    while(*code != NULL)
        code = &((*code)->prox);

    LBINSTR nodo = malloc(sizeof(struct BINSTRNODE));
    nodo->c = bit;
    nodo->prox = NULL;

    (*code) = nodo;
}

void addBitToCodes(char bit, LBINSTR* codes, int start, int end)
{
    for (int i=start; i<=end; i++)
        addBitToCode(bit, &codes[i]);
}

void calcularCodigosSF (int* freqs, LBINSTR* codes, int start, int end)
{
    int div;
    if (start!=end)
    {
        div = calcularMelhorDivisao(freqs, start, end);
        addBitToCodes('0', codes, start, div);
        addBitToCodes('1', codes, div+1, end);
        calcularCodigosSF(freqs, codes, start, div);
        calcularCodigosSF(freqs, codes, div+1, end);
    }
}

