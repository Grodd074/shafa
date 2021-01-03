
#include <a.h>

/* 
Módulo A -- Compressão RLE e Cálculo das Frequências sobre o ficheiro original e comprimido.
Júlio Beites Gonçalves - a93243 - 3/01/2021

Este módulo é chamado, recebendo como inputs, o nome do ficheiro, o tamanho definido para o processamento de blocos e por último um inteiro
indicativo se o comando "-c r" foi introduzido, no caso de o seu valor ser 1 a compressão RLE é forçada, o seu valor é 0 caso contrário.

As duas funções principais deste bloco são: 

normal_compressao {

  - A lógica seguida sempre pelo meu programa é a cópia de um bloco do ficheiro para memória, 
seguido do cálculo das frequências sobre o ficheiro original. De seguida efetua a compressão RLE sobre o bloco 
em memória e escreve diretamente no ficheiro de saida .rle. 
  - Adicionalmente, foram criadas funções idênticas mas que se limitam apenas ao processamento do último bloco 
do ficheiro uma vez que este vai apresentar caracteristicas diferentes dos restantes. Ao longo da execução desta 
função são também guardadas informações que virão a ser utilizadas depois tais como: o tamanho de cada bloco após 
a sua compressão, a taxa de compressão, o tamanho do ficheiro original, numero de blocos a serem processados, entre outros...
  - O cálculo das frequências sobre o ficheiro .rle apenas é efetuada perto do final da execução do programa.
  - Este copia cada bloco já compresso para memória novamente, onde efetua o cálculo das suas frequências.
  - Posteriormente, vai criar um ficheiro com a extensão .freq onde vai inscrever a informação referente ao ficheiro .rle .
  - Por último, é invocada a função imprimeTerminal com a finalidade de reproduzir no terminal um texto informativo referente à execução do módulo A.
}

forca_compressao {
  
  - Esta função é semelhante à anterior, diferenciando-se na ausência das restrições que impediam
   a função de executar a compressão RLE consequente de uma taxa de compressão inferior a 5\%.
   }
*/

// Função para criar uma string com a extensão .rle
char *nomeRLE(char *file_name)
{
    char *nomeFile = malloc(sizeof(char) * 50);
    strcpy(nomeFile, file_name);
    char extensao[] = ".rle";
    unsigned char miyagi;
    strcat(nomeFile, extensao);

    return nomeFile;
}

// Função para criar uma string com a extensao .freq
char *nomeFREQ(char *file_name)
{
    char *nomeFile = malloc(sizeof(char) * 50);
    strcpy(nomeFile, file_name);
    char extensao[] = ".freq";
    unsigned char miyagi;
    strcat(nomeFile, extensao);

    return nomeFile;
}

// Função para calcular o tamanho de um ficheiro
unsigned long tamanhoFicheiro(char *file_name)
{
    unsigned long tamanhoBytes = 0;

    FILE *fp = fopen(file_name, "rb");
    if (fp == NULL)
    {
        printf("Erro a abrir o ficheiro Tamanho\n");
        return 0;
    }

    fseek(fp, 0, SEEK_END);   // percorre o ficheiro todo
    tamanhoBytes = ftell(fp); // calcula o numero de caracteres
    fclose(fp);

    return tamanhoBytes;
}

unsigned char *criarBufferArrayRLE(char *file_name, int *BlocosRLE, int bloco)
{
    int i = 0;
    unsigned char *v = malloc(sizeof(char) * (BlocosRLE[bloco])); // aloca espaço na memória

    FILE *fp = fopen(file_name, "rb");
    if (fp == NULL)
    {
        printf("Erro ao abrir o ficheiro\n");
        return 0;
    }
    while (i < bloco - 1)
    {
        fseek(fp, BlocosRLE[i], SEEK_SET); // Atualiza o apontador dentro do ficheiro
        i++;
    }
    fread(v, BlocosRLE[bloco], 1, fp); // Copia o bloco para memória

    fclose(fp);

    return v;
}

// Função para inicializar o buffer que copia cada bloco para memória
unsigned char *criarBufferArray(char *file_name, int size, int bloco)
{
    int i = 0;

    unsigned char *v = malloc(sizeof(char) * size); // aloca espaço na memória

    FILE *fp = fopen(file_name, "rb");
    if (fp == NULL)
    {
        printf("Erro ao abrir o ficheiro\n");
        return 0;
    }

    fseek(fp, (size * bloco), SEEK_SET); // Atualiza o apontador dentro do ficheiro
    fread(v, size, 1, fp);               // Copia o bloco para memória

    return v;
}

// Função para inicializar o buffer que copia o ultimo bloco para memória
unsigned char *criarBufferArrayFinal(char *file_name, int size, int sizeFinal, int bloco)
{
    int i = 0;
    unsigned long a = tamanhoFicheiro(file_name);
    unsigned char *v = malloc(sizeof(char) * (sizeFinal)); // aloca espaço na memória

    FILE *fp = fopen(file_name, "rb");
    if (fp == NULL)
    {
        printf("Erro ao abrir o ficheiro\n");
        return 0;
    }

    fseek(fp, size * bloco, SEEK_SET); // Atualiza o apontador dentro do ficheiro
    fread(v, sizeFinal, 1, fp);

    fclose(fp);

    return v;
}

// Função que inicializa uma matriz para guardar as frequencias dos diferentes caracteres para cada bloco
int **criarBufferMatriz(int numeroBlocos)
{
    int i, bloco;
    int **j = malloc(sizeof(int *) * numeroBlocos); // aloca memória para os blocos

    for (i = 0; i < numeroBlocos; i++) // para cada bloco, aloca memória para os 256 caracteres possiveis
        j[i] = malloc(sizeof(int *) * 256);

    for (bloco = 0; bloco < numeroBlocos; bloco++)
    {
        for (i = 0; i < 256; i++)
        {
            j[bloco][i] = 0; // percorre toda a matriz e inicializa-a a 0
        }
    }

    return j;
}

// Função para comprimir o ficheiro original
int compressaoRLE(char *file_name, unsigned char *v, int size, int bloco)
{
    unsigned char atual;
    unsigned char previous;
    int contador = 1;
    int i = 0;
    int caracters = 0;
    unsigned char miyagi;

    char *nomeFile = malloc(sizeof(char) * 50);
    strcpy(nomeFile, nomeRLE(file_name)); // Cria o nome do ficheiro de escrita .rle

    FILE *ft = fopen(nomeFile, "ab");
    if (ft == NULL)
    {
        printf("Erro a abrir ficheiro de escrita\n"); // Acabar de fazer a funcao compressao com base num array
        return 0;
    }
    fseek(ft, (size * bloco), SEEK_SET); // Altera o apontador dentro do ficheiro
    atual = v[i];
    while (i < size) // Percorre o bloco lido para memória
    {
        previous = atual;
        i++;
        atual = v[i];

        if (v[i] == 0)
        {
            previous = atual;
            i++;
            atual = v[i];
            while (atual == previous && contador < 255 && i < size)
            {
                contador++;
                previous = atual;
                i++;
                atual = v[i];
            }
            fputc('\0', ft);
            fputc('\0', ft);
            miyagi = contador;
            fputc(miyagi, ft);
            caracters += 3;
            contador = 1;
        }
        else if (atual == previous) // Enquanto o caracter atual for igual ao anterior incrementa o contador
            contador++;

        else if (atual != previous)
        {
            if (contador <= 3) // Caso o contador seja <= 3 não compensa fazer a compressão
            {
                while (contador != 0)
                {
                    fputc(previous, ft);
                    contador--;
                    caracters++;
                }
                contador = 1;
            }
            else // Caso contrário, contador > 3, vai fazer a compressão rle
            {
                fputc('\0', ft);
                fprintf(ft, "%c", previous);
                miyagi = contador;
                fputc(miyagi, ft);
                caracters += 3;
                contador = 1;
            }
        }
    }

    fclose(ft);

    return caracters; // retorna o numero de caracteres após executada a compressão
}

// Função para calcular a taxa de compressão
double taxaCompressao(int cZipped, unsigned char *v)
{
    int i = 0;
    double cUnzipped = 0;
    double taxaC;

    while (v[i] != '\0') // calcula o numero de caracteres no bloco copiado do ficheiro original
    {
        i++;
        cUnzipped++;
    }

    taxaC = (((abs(cUnzipped - cZipped)) / cUnzipped) * 100); // calcula a taxa de compressão

    return taxaC;
}

// Função para retornar em quantos blocos o ficheiro vai ser dividido
int quantidadeBlocos(unsigned long tamanhoFicheiro, int size)
{

    int blocos;
    int resto;
    blocos = tamanhoFicheiro / size; // calcula a parte inteira da divisão do tamanho do Ficheiro / size
    resto = tamanhoFicheiro % size;  // calcula o resto * size

    if ((resto) >= 1024)
        return blocos += 1; // caso o resto >= 1024, vai criar um bloco adicional
    else
        return blocos;
}

// Função para imprimir o tamanho de cada bloco
int imprimeTamanhoBlocos(unsigned long tamanhoFicheiro, int size)
{
    double blocos;
    double resto;
    blocos = tamanhoFicheiro / size; // calcula a parte inteira da divisão do tamanho do Ficheiro / size
    resto = tamanhoFicheiro % size;  // calcula o resto * size

    if (blocos == 0)
        printf("%ld bytes\n", tamanhoFicheiro);
    else if (tamanhoFicheiro == size)
        printf("%ld bytes\n", tamanhoFicheiro);
    else if ((resto) >= 1024) // caso o resto >= 1024, vai imprimir o tamanho do bloco adicional
    {
        while (blocos > 0)
        {
            printf("%d | ", size);
            blocos--;
        }
        printf("%.0lf\n", resto); // imprime o tamanho do último blobco
        return (resto);
    }
    else // caso contrário, o último bloco vai ser absorvido
    {
        blocos--;
        while (blocos > 0)
        {
            while (blocos > 0)
            {
                printf("%d | ", size);
                blocos--;
            }
            printf("%.0lf bytes\n", (size + resto)); // imprime o tamanho do ultimo bloco absorvido
        }
        return (size + resto);
    }
}

// Função para calcular o tamanho do ultimo bloco
int tamanhoUltimoBloco(unsigned long tamanhoFicheiro, int size)
{
    double blocos;
    double resto;
    blocos = tamanhoFicheiro / size; // calcula a parte inteira da divisão do tamanho do Ficheiro / size
    resto = tamanhoFicheiro % size;  // calcula o resto * size

    if (blocos == 0)
        return tamanhoFicheiro;
    else if ((resto) >= 1024)
        return (resto);
    else
        return (size + resto); // caso o ultimo bloco seja inferior a 1024, o ultimo bloco agrega-se ao penúltimo
}

// Função para guardar em memória, a frequencia dos simbolos para cada bloco
int **frequenciaCalculo(unsigned char *v, int size, int bloco, int **j)
{
    int i = 0;  

    while (i < size)
    {
        j[bloco][v[i]]++;
        i++;
    }
    return j;
}

// Função com a finalidade de guardar num buffer as frequencias do ficheiro .rle
int **calculoFrequenciaRLE(char *file_name, int *blocosRLE, int BlocosLength)
{

    int bloco = 0;
    int **k;
    unsigned char *v;

    k = criarBufferMatriz(BlocosLength);    

    while (bloco < BlocosLength)
    {
        v = criarBufferArrayRLE(file_name, blocosRLE, bloco);       // copia para memória uma lista de caracteres provenientes do ficheiro .rle
        int size = blocosRLE[bloco];                                // armazena o tamanho dos blocos após a compressão
        k = frequenciaCalculo(v, size, bloco, k);                   // efetua o cálculo das frequências
        bloco++;
    }
    return k;
}

// Função com a finalidade de guardar num buffer as frequencias do ficheiro .rle
int funcaoFrequenciaEscritaRLE(char *file_name, char k, int BlocosLength, int *blocosRLE)
{

    int **j = calculoFrequenciaRLE(file_name, blocosRLE, BlocosLength); // Funcao auxiliar para armazenar num buffer as frequencias

    char *nomeFile = malloc(sizeof(char) * 50);
    strcpy(nomeFile, nomeFREQ(file_name));                              // cria  nome do ficheiro a criar .freq

    FILE *fp = fopen(nomeFile, "wb");

    if (fp == NULL)
    {
        printf("Erro a abrir Ficheiro de escrita das Frequências\n");
        return 0;
    }

    fprintf(fp, "@%c", k);            // Indicativo da ocorrência da compressão RLE
    fprintf(fp, "@%d", BlocosLength); // Numero de Blocos Processados

    if (BlocosLength == 1) // Caso apenas tenha sido processado 1 bloco
    {
        fprintf(fp, "@%d@", blocosRLE[0]);
        for (int k = 0; k < 255; k++)
        {
            if (j[0][k] == j[0][k - 1])
            {
                fprintf(fp, ";"); // Frequencia igual ao caracter anterior
            }
            else
            {
                fprintf(fp, "%d;", j[0][k]); // Frequencia igual ao caracter anterior
            }
        }
    }
    else // Caso contrário, 1 <
    {
        int c, b;

        for (b = 0; b < BlocosLength; b++)
        {

            fprintf(fp, "@%d@", blocosRLE[b]); // Indicativo de um novo bloco(tamanho)
            for (c = 0; c < 255; c++)
            {
                if (c == 0)
                    fprintf(fp, "%d;", j[b][0]);
                else if (j[b][c] == j[b][c - 1])
                {
                    fprintf(fp, ";"); // Frequencia igual ao caracter anterior
                }
                else
                {
                    fprintf(fp, "%d;", j[b][c]); // Frequencia diferente do caracter anterior
                }
            }
        }
        fprintf(fp, "@0"); // Final do ficheiro
        fclose(fp);

        return 0;
    }
}

// Função para escrever as frequencias de cada simbolo para cada bloco
int funcaoFrequenciaEscrita(char *file_name, int **j, char k, unsigned long tamanhoFicheiro, int size, int numeroblocos)
{
    int a = tamanhoUltimoBloco(tamanhoFicheiro, size); // Função para calcular o tamanho do último bloco

    char *nomeFile = malloc(sizeof(char) * 50);
    strcpy(nomeFile, nomeFREQ(file_name)); // Função para criar o nome do ficheiro .fre30
    FILE *fp = fopen(nomeFile, "wb");
    if (fp == NULL)
    {
        printf("Erro a abrir Ficheiro de escrita das Frequências\n");
        return 0;
    }

    fprintf(fp, "@%c", k);            // Indicativo da ocorrência da compressão RLE
    fprintf(fp, "@%d", numeroblocos); // Numero de Blocos Processados

    if (numeroblocos == 1) // Caso apenas tenha sido processado 1 bloco
    {
        fprintf(fp, "@%ld@", tamanhoFicheiro);
        for (int k = 0; k < 255; k++)
        {
            if (j[0][k] == j[0][k - 1])
            {
                fprintf(fp, ";"); // Frequencia igual ao caracter anterior
            }
            else
            {
                fprintf(fp, "%d;", j[0][k]); // Frequencia igual ao caracter anterior
            }
        }
    }
    else // Caso contrário, 1 <
    {
        int c, b;

        for (b = 0; b < numeroblocos - 1; b++)
        {

            fprintf(fp, "@%d@", size); // Indicativo de um novo bloco(tamanho)
            for (c = 0; c < 255; c++)
            {
                if (c == 0)
                    fprintf(fp, "%d;", j[b][0]);
                else if (j[b][c] == j[b][c - 1])
                {
                    fprintf(fp, ";"); // Frequencia igual ao caracter anterior
                }
                else
                {
                    fprintf(fp, "%d;", j[b][c]); // Frequencia diferente do caracter anterior
                }
            }
        }

        fprintf(fp, "@%d@", a); // Tamanho do ultimo bloco

        for (c = 0; c < 255; c++)
        {
            if (c == 0)
                fprintf(fp, "%d;", j[b][0]);
            if (j[b][c] == j[b][c - 1])
                fprintf(fp, ";"); // Frequencia igual ao caracter anterior
            else
                fprintf(fp, "%d;", j[b][c]); // Frequencia diferente do caracter anterior
        }
    }

    fprintf(fp, "@0"); // Final do ficheiro
    fclose(fp);

    return 0;
}

// Função para imprimir o tamanho de cada bloco após a compressão
void imprimeTamanhoBlocosRLE(int *blocosRLE, int BlocosLength)
{
    int r = 0;

    while (blocosRLE[r] && r < BlocosLength - 1)
    {
        printf(" %d | ", blocosRLE[r]);
        r++;
    }

    printf("%d bytes\n", blocosRLE[r]); // Caso do Ultimo bloco
}

// Função para imprimir texto informativo na consola
void imprimeTerminal(char *file_name, char m, int BlocosLength, int FileLength, int size, int taxa, int *blocosRle)
{
    char *nomeFile = malloc(sizeof(char) * 50);
    strcpy(nomeFile, nomeRLE(file_name)); // Função para criar o nome do ficheiro .rle

    printf("Júlio Gonçalves, A93243, MIEI/CD, 03/01/2021\n");
    printf("Módulo: A (Compressão e Cálculo da Frequência)\n");
    printf("Número de Blocos: %d\n", BlocosLength);
    printf("Tamanho dos blocos do Ficheiro Original: ");
    imprimeTamanhoBlocos(FileLength, size);

    if (m == 'R') // Caso o ficheiro tenha sido comprimido
    {
        char *nomeFile2 = malloc(sizeof(char) * 50);
        strcpy(nomeFile2, nomeFile);
        char extensao2[] = ".freq";
        strcat(nomeFile2, extensao2); // Função para criar o nome do ficheiro .rle.freq

        char *nomeFile3 = malloc(sizeof(char) * 50);
        strcpy(nomeFile3, nomeFREQ(file_name)); // Função para criar o nome do ficheiro .freq
                                                // o file_name recebido ja é suposto ser .rle
        printf("Compressão RLE: %s (%d%c compressão)\n", nomeFile, taxa, '%');
        printf("Tamanho dos blocos do Ficheiro RLE: ");
        imprimeTamanhoBlocosRLE(blocosRle, BlocosLength);
        printf("Ficheiros Gerados: %s, %s, %s\n", nomeFile, nomeFile2, nomeFile3);
    }
    else // Caso o ficheiro não tenha sido comprimido
    {
        char *nomeFile3 = malloc(sizeof(char) * 50);
        strcpy(nomeFile3, nomeFREQ(file_name));

        printf("Ficheiros Gerados: %s\n", nomeFile3);
    }
}

// Função executada através do comando "-c r"
int forca_compressao(char *file_name, int size)
{
    unsigned char *v;
    int **j;
    int bloco = 0;
    int caracterRLE = 0;
    int RleB = 0;
    int taxa;
    unsigned long FileLength = tamanhoFicheiro(file_name); // Calcula o tamanho do ficheiro .txt
    int BlocosLength = quantidadeBlocos(FileLength, size); // Calcula em quantos blocos vai ser divido o ficheiro original
    int *blocosRLE = malloc(sizeof(int) * BlocosLength);   // Guarda o tamanho de cada bloco depois de comprimido

    if (BlocosLength > 1) // Condição de teste
    {
        j = criarBufferMatriz(BlocosLength); // Cria uma matriz
        while (bloco < BlocosLength - 1)     // Condição para saltar no ultimo bloco
        {
            v = criarBufferArray(file_name, size, bloco);           // Copia um bloco para memória
            j = frequenciaCalculo(v, size, bloco, j);               // Guarda em memória as frequencias de cada simbolo para cada bloco
            caracterRLE = compressaoRLE(file_name, v, size, bloco); // Comprime e escreve no ficheiro .rle bloco a bloco && retorna o numero de caracteres final.

            if (bloco == 0) // Calcula a taxa de compressão para o primeiro bloco
                taxa = taxaCompressao(caracterRLE, v);

            blocosRLE[RleB] = caracterRLE; // Guarda o tamanho de cada bloco depois de comprimido
            RleB++;                        // Atualiza o bloco
            bloco++;                       // Atualiza o bloco
            free(v);                       // Liberta espaço alocado na memória
        }

        int a = tamanhoUltimoBloco(FileLength, size);            // Calcula o tamanho do ultimo blocos
        v = criarBufferArrayFinal(file_name, size, a, bloco);    // Copia o ultimo bloco para memória
        j = frequenciaCalculo(v, a, bloco, j);                   // Guarda em memória as frequencias de cada simbolo para o ultimo bloco
        blocosRLE[RleB] = compressaoRLE(file_name, v, a, bloco); // Guarda o tamanho do último bloco depois de comprimido
    }
    else // Caso o ficheiro seja todo processado num unico bloco
    {
        int tamanho;
        if (FileLength > size)
            tamanho = size;
        else
            tamanho = FileLength;

        v = criarBufferArray (file_name, tamanho, bloco);          // Copia um bloco para memória
        j = criarBufferMatriz(BlocosLength);                       // Cria uma matriz
        j = frequenciaCalculo(v, tamanho, bloco, j);               // Guarda em memória as frequencias de cada simbolo para cada bloco
        caracterRLE = compressaoRLE(file_name, v, tamanho, bloco); // Comprime e escreve no ficheiro .rle bloco a bloco && retorna o numero de caracteres final.

        taxa = taxaCompressao(caracterRLE, v); // Calcula a taxa de compressão
        blocosRLE[RleB] = caracterRLE;         // Guarda o tamanho do bloco depois de comprimido
    }

    char *nomeFile = malloc(sizeof(char) * 50);
    strcpy(nomeFile, nomeRLE(file_name));

    funcaoFrequenciaEscrita(file_name, j, 'R', FileLength, size, BlocosLength); // Função que cria e cálcula as frequencias de cada simboolo em cada bloco
    funcaoFrequenciaEscritaRLE(nomeFile, 'R', BlocosLength, blocosRLE);
    imprimeTerminal(file_name, 'R', BlocosLength, FileLength, size, taxa, blocosRLE); // Função para escrever texto informativo no terminal

    return 0;
}

// Função que executa o módulo A normalmente
int normal_compressao(char *file_name, int size)
{
    unsigned char *v;
    int **j;
    int bloco = 0;
    int caracterRLE = 0;
    int RleB = 0;
    char m = 'N';
    int taxa;
    unsigned long FileLength = tamanhoFicheiro(file_name); // Calcula o tamanho do ficheiro .txt
    int BlocosLength = quantidadeBlocos(FileLength, size); // Calcula em quantos blocos vai ser divido o ficheiro original
    int *blocosRLE = malloc(sizeof(int) * BlocosLength);   // Guarda o tamanho de cada bloco depois de comprimido

    int tamanho;
    if (FileLength > size)
        tamanho = size;
    else
        tamanho = FileLength;

    v = criarBufferArray(file_name, tamanho, bloco); // Copia um bloco para memória
    j = criarBufferMatriz(BlocosLength);             // Cria uma matriz
    j = frequenciaCalculo(v, tamanho, bloco, j);     // Guarda em memória as frequencias de cada simbolo para cada bloco

    caracterRLE = compressaoRLE(file_name, v, tamanho, bloco); // Comprime e escreve no ficheiro .rle bloco a bloco && retorna o numero de caracteres final.
    taxa = taxaCompressao(caracterRLE, v);                     // Calcula a taxa de compressão
    blocosRLE[RleB] = caracterRLE;                             // Guarda o tamanho de cada bloco depois de comprimido
    RleB++;                                                    // Atualiza o bloco
    bloco++;                                                   // Atualiza o bloco

    if (taxa >= 5 && BlocosLength > 1) // Condição de teste
    {
        while (bloco < BlocosLength - 1) // Condição para saltar no ultimo bloco
        {
            v = criarBufferArray(file_name, size, bloco);               // Copia um bloco para memória
            j = frequenciaCalculo(v, size, bloco, j);                   // Guarda em memória as frequencias de cada simbolo para cada bloco
            blocosRLE[RleB] = compressaoRLE(file_name, v, size, bloco); // Guarda o tamanho de cada bloco depois de comprimido
            RleB++;                                                     // Atualiza o bloco
            bloco++;                                                    // Atualiza o bloco
            free(v);                                                    // Liberta espaço alocado na memória
        }

        int a = tamanhoUltimoBloco(FileLength, size);            // Calcula o tamanho do ultimo blocos
        v = criarBufferArrayFinal(file_name, size, a, bloco);    // Copia o ultimo bloco para memória
        j = frequenciaCalculo(v, size, bloco, j);                // Guarda em memória as frequencias de cada simbolo para o ultimo bloco
        blocosRLE[RleB] = compressaoRLE(file_name, v, a, bloco); // Guarda o tamanho do último bloco depois de comprimido
    }
    else if (taxa < 5 && BlocosLength > 1)
    {
        while (bloco < BlocosLength - 1) // Condição para saltar no ultimo bloco
        {
            v = criarBufferArray(file_name, size, bloco); // Copia um bloco para memória
            j = frequenciaCalculo(v, size, bloco, j);     // Guarda em memória as frequencias de cada simbolo para cada bloco
            bloco++;                                      // Atualiza o bloco
            free(v);                                      // Liberta espaço alocado na memória
        }

        int a = tamanhoUltimoBloco(FileLength, size);         // Calcula o tamanho do ultimo blocos
        v = criarBufferArrayFinal(file_name, size, a, bloco); // Copia o ultimo bloco para memória
        j = frequenciaCalculo(v, a, bloco, j);                // Guarda em memória as frequencias de cada simbolo para o ultimo bloco
    }

    if (taxa >= 5)
    { // Altera o char indicativo da compressão
        m = 'R';
        char *nomeFile = malloc(sizeof(char) * 50);
        strcpy(nomeFile, nomeRLE(file_name));

        funcaoFrequenciaEscrita(file_name, j, m, FileLength, size, BlocosLength); // Função que cria e cálcula as frequencias de cada simboolo em cada bloco
        funcaoFrequenciaEscritaRLE(nomeFile, m, BlocosLength, blocosRLE);
        imprimeTerminal(file_name, m, BlocosLength, FileLength, size, taxa, blocosRLE); // Função para escrever texto informativo no terminal
    }

    if (taxa < 5) // No caso de não ter feito  a compressão, vai eliminar o ficheiro criado para a compressão do primeiro bloco
    {
        char *nomeFile = malloc(sizeof(char) * 50);
        strcpy(nomeFile, nomeRLE(file_name));
        remove(nomeFile);
        funcaoFrequenciaEscrita(file_name, j, m, FileLength, size, BlocosLength);       // Função que cria e cálcula as frequencias de cada simboolo em cada bloco
        imprimeTerminal(file_name, m, BlocosLength, FileLength, size, taxa, blocosRLE); // Função para escrever texto informativo no terminal
    }
    return 0;
}

// Funcao Principal Modulo A
int moduloA(char *file_name, int default_size, int force_RLE)
{
    unsigned long FileLength = tamanhoFicheiro(file_name); // calcula o tamanho do ficheiro original
    clock_t tempo_i, tempo_f;
    double tempo;

    if (FileLength <= 1024)
    {
        printf("O Tamanho do Ficheiro é demasiado pequeno");
        return 0;
    }

    if (force_RLE == 1) // caso em que foi executado o comando -c r
    {
        tempo_i = clock();
        forca_compressao(file_name, default_size);
        tempo_f = clock();
    }
    else if (force_RLE == 0) // caso em que o módulo é chamado normalmente
    {
        tempo_i = clock();
        normal_compressao(file_name, default_size);
        tempo_f = clock();
    }

    tempo = (double)(tempo_f - tempo_i) / (CLOCKS_PER_SEC / 1000); // calculo do tempo de execução do módulo
    printf("Tempo de execução do módulo A (milissegundos): %0.3lf ms\n", tempo);

    return 0;
}