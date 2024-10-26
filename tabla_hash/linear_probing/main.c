#include<stdio.h>
#include<stdlib.h>
#include "tablahash.h"

void *copia_entero(void *n) {
    int *m = malloc(sizeof(int));
    *m = *(int*)n;
    return m;
}

int comparar(void *a, void *b) {
    if (*(int*)a < *(int*)b) return -1;
    if (*(int*)a > *(int*)b) return 1;
    return 0;
}

void destruir(void *n) {
    free((int*)n);
}

unsigned hash(void *n) {
    return *(int*)n;
}

int main() {

    TablaHash tablahash = tablahash_crear(5, copia_entero, comparar, destruir, hash);
    int numeros[] = { 70, 60, 40, 7, 80, 90, 93 };
    tablahash_insertar(tablahash, numeros);
    tablahash_insertar(tablahash, &numeros[1]);
    tablahash_insertar(tablahash, &numeros[2]);
    tablahash_insertar(tablahash, &numeros[3]);
    tablahash_insertar(tablahash, &numeros[4]);
    imprimir_tabla(tablahash);
    printf("---------- cantidad de elementos: %d\n", tablahash_nelems(tablahash));
    tablahash_eliminar(tablahash, &numeros[1]);
    imprimir_tabla(tablahash);
    printf("---------- cantidad de elementos: %d\n", tablahash_nelems(tablahash));
    tablahash_insertar(tablahash, &numeros[5]);
    imprimir_tabla(tablahash);
    printf("---------- cantidad de elementos: %d\n", tablahash_nelems(tablahash));
    tablahash_insertar(tablahash, &numeros[5]);
    imprimir_tabla(tablahash);
    printf("---------- cantidad de elementos: %d\n", tablahash_nelems(tablahash));
    tablahash_destruir(tablahash);

    return 0;
}