#include "dados.h"

FICHEIRO* inicializar_ficheiro()
{
    FICHEIRO* f = malloc(sizeof(FICHEIRO));
    f->size = 0;
    f->data = NULL;
}
