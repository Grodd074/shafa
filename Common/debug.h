#ifndef SHAFA_DEBUG_H
#define SHAFA_DEBUG_H

void print_nada(const char* f, ...);

#ifdef DEBUG
#define PRINT_DEBUG printf
#else
#define PRINT_DEBUG print_nada
#endif

#endif //SHAFA_DEBUG_H