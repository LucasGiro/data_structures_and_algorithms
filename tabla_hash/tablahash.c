#include "tablahash.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Casillas en la que almacenaremos los datos de la tabla hash.
 */
typedef struct
{
  void *dato;
  int deleted;
} CasillaHash;

/**
 * Estructura principal que representa la tabla hash.
 */
struct _TablaHash
{
  CasillaHash *elems;
  unsigned numElems;
  unsigned capacidad;
  FuncionCopiadora copia;
  FuncionComparadora comp;
  FuncionDestructora destr;
  FuncionHash hash;
};

/**
 * Crea una nueva tabla hash vacia, con la capacidad dada.
 */
TablaHash tablahash_crear(unsigned capacidad, FuncionCopiadora copia,
                          FuncionComparadora comp, FuncionDestructora destr,
                          FuncionHash hash)
{

  // Pedimos memoria para la estructura principal y las casillas.
  TablaHash tabla = malloc(sizeof(struct _TablaHash));
  assert(tabla != NULL);
  tabla->elems = malloc(sizeof(CasillaHash) * capacidad);
  assert(tabla->elems != NULL);
  tabla->numElems = 0;
  tabla->capacidad = capacidad;
  tabla->copia = copia;
  tabla->comp = comp;
  tabla->destr = destr;
  tabla->hash = hash;

  // Inicializamos las casillas con datos nulos.
  for (unsigned idx = 0; idx < capacidad; ++idx)
  {
    tabla->elems[idx].dato = NULL;
    tabla->elems[idx].deleted = 0;
  }

  return tabla;
}

/**
 * Retorna el numero de elementos de la tabla.
 */
int tablahash_nelems(TablaHash tabla) { return tabla->numElems; }

/**
 * Retorna la capacidad de la tabla.
 */
int tablahash_capacidad(TablaHash tabla) { return tabla->capacidad; }

/**
 * Destruye la tabla.
 */
void tablahash_destruir(TablaHash tabla)
{

  // Destruir cada uno de los datos.
  for (unsigned idx = 0; idx < tabla->capacidad; ++idx)
    if (tabla->elems[idx].dato != NULL)
      tabla->destr(tabla->elems[idx].dato);

  // Liberar el arreglo de casillas y la tabla.
  free(tabla->elems);
  free(tabla);
  return;
}

/**
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 */
void tablahash_insertar(TablaHash tabla, void *dato)
{

  unsigned k = 0;
  unsigned capacidad = tabla->capacidad;
  int insertado = 0;
  while (k < capacidad && !insertado)
  {
    unsigned idx = (tabla->hash(dato) + k) % capacidad;

    if (tabla->elems[idx].dato == NULL)
    {
      tabla->numElems++;
      tabla->elems[idx].dato = tabla->copia(dato);
      insertado = 1;
    }
    else if (tabla->elems[idx].deleted)
    {
      tabla->numElems++;
      tabla->elems[idx].dato = tabla->copia(dato);
      tabla->elems[idx].deleted = 0;
      insertado = 1;
      // Sobrescribir el dato si el mismo ya se encontraba en la tabla.
    }
    else if (tabla->comp(tabla->elems[idx].dato, dato) == 0)
    {
      tabla->destr(tabla->elems[idx].dato);
      tabla->elems[idx].dato = tabla->copia(dato);
      insertado = 1;
    }
    k++;
  }
}

/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
void *tablahash_buscar(TablaHash tabla, void *dato)
{

  unsigned k = 0;
  unsigned capacidad = tabla->capacidad;
  int encontrado = 0;
  void *elem = NULL;

  while (k < capacidad && !encontrado)
  {

    // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
    unsigned idx = (tabla->hash(dato) + k) % tabla->capacidad;

    // Retornar NULL si la casilla estaba vacia.
    if (tabla->elems[idx].dato == NULL)
    {
      elem = NULL;
      encontrado = 1;
    }
    // Retornar el dato de la casilla si hay concidencia.
    else if (tabla->comp(tabla->elems[idx].dato, dato) == 0 && !tabla->elems[idx].deleted)
    {
      elem = tabla->elems[idx].dato;
      encontrado = 1;
    }
    k++;
  }

  return elem;
}

/**
 * Elimina el dato de la tabla que coincida con el dato dado.
 */
void tablahash_eliminar(TablaHash tabla, void *dato)
{
  unsigned k = 0;
  unsigned capacidad = tabla->capacidad;
  unsigned eliminado = 0;

  while (k < capacidad && !eliminado)
  {

    // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
    unsigned idx =  (tabla->hash(dato) + k) % tabla->capacidad;

    // Retornar si la casilla estaba vacia.
    if (tabla->elems[idx].dato == NULL)
      eliminado = 1;
    // Vaciar la casilla si hay coincidencia.
    else if (tabla->comp(tabla->elems[idx].dato, dato) == 0 && !tabla->elems[idx].deleted)
    {
      tabla->numElems--;
      tabla->destr(tabla->elems[idx].dato);
      tabla->elems[idx].deleted = 1;
      eliminado = 1;
    }

    k++;

  }
}

void imprimir_tabla(TablaHash tabla)
{

  for (int i = 0; i < tabla->capacidad; i++)
  {
    printf("[ %d ]\n", (tabla->elems[i].dato != NULL) ? *(int *)tabla->elems[i].dato : -1);
  }
}
