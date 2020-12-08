#include "dados.h"

FICHEIRO* inicializar_ficheiro()
{
    FICHEIRO* f = malloc(sizeof(FICHEIRO));
    f->total_data_size = 0;
    f->block_size = 65536; // Default de 64 Kbytes
    f->last_block_size = 0;
    f->n_blocks = 0;
    f->data = NULL;
    f->ultimo_absorvido = false;
}
