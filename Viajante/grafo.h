#ifndef _GRAFO_H
#define _GRAFO_H

#include <stdbool.h> 
#include <stddef.h>
#include "lista.h"
#include "hash.h"
#include <string.h>
#include "heap.h"


// Estructura del grafo
typedef struct _grafo grafo_t;

// Funcion de destruccion
typedef void (*destruir_dato_f)(void *);

// -------- PRIMITIVAS DEL GRAFO -------

// Crea un grafo. Devuelve NULL si no fue posible.
// Post: el grafo se creo o se devolvio NULL.
grafo_t *grafo_crear();

// Destruye el grafo recibido.
// Pre: el grafo fue creado.
// Post: la memoria del grafo se libero.
void grafo_destruir(grafo_t *grafo);

// Agrega un vertice al grafo con la clave recibida, e inicializa
// su lista de caminos de forma vacia. Devuelve true o false si
// pudo o no llevarse a cabo respectivamente.
// Pre: el grafo fue creado.
// Post: se agrego el vertice con la clave, y se devolvio true
// o false en caso contrario.
bool grafo_crear_vertice(grafo_t *grafo, char *clave);

// Crea una arista uniendo dos vertices origen -> destino. Si
// idayvuelta es true, ademas crea la arista en el otro
// sentido (origen <- destino). Devuelve true o false segun
// la funcion haya funcionado o no.
// Pre: el grafo fue creado.
// Post: se creo el camino y se devolvio true o false.
bool grafo_crear_arista(grafo_t *grafo, char *origen, char *destino, bool idayvuelta, int peso);

// Devuelve true si existe un vertice con la clave recibida.
// Pre: el grafo fue creado, la clave es valida.
// Post: se devolvio true o false segun corresponda.
bool grafo_pertenece(grafo_t *grafo, char* clave);

// Devuelve true si hay una arista desde nodo origen a destino.
// Pre: el grafo fue creado.
// Post: se devolvio true o false segund corresponda.
bool grafo_son_adyacentes(grafo_t *grafo, char *origen, char *destino);

// Devuelve true si el grafo esta vacio o false en caso contrario.
// Pre: el grafo fue creado.
bool grafo_esta_vacio(grafo_t *grafo);

// Devuelve la cantidad de elementos guardados en el grafo.
// Pre: el grafo fue creado.
// Post: se devolvio la cant. de elem. del grafo.
size_t grafo_cantidad(grafo_t *grafo);

// Devuelve todos los vertices que son adyacentes al
// vertice con la clave recibida.
// Pre: el grafo fue creado.
// Post: se devolvio un hash con todos los adyacentes.
hash_t *grafo_devolver_adyacentes(grafo_t *grafo, char *clave);

// Borra un vertice del grafo con todos sus caminos asociados.
// Pre: el grafo fue creado.
// Post: se borro el vertice que tenia a clave.
void grafo_borrar_vertice(grafo_t *grafo, char *clave);

// Borra la arista que va de origen a destino.
// Pre: el grafo fue creado.
// Post: se borro el camino.
void grafo_borrar_arista(grafo_t *grafo, char *origen, char *destino);

// Devuelve el hash con todas las conexiones del grafo.
// Pre: el grafo fue creado.
// Post: se devolvio el hash de todas las conexiones del grafo.
hash_t* grafo_devolver_conexiones(grafo_t* grafo);

// Devuelve el peso de una arista que una origen con destino
// Pre: origen y destino estan conectados
// Post: se devolvio el peso de dicha conexion
int grafo_devolver_peso_arista(grafo_t *grafo, char *origen, char *destino);

// Devuelve una lista con las claves de todos los vértices 
// del grafo. Si el grafo está vacío, devuelve una lista vacía.
// Pre: el grafo fue creado.
// Post: se devolvió la lista de vértices del grafo, o 	NULL en
// caso de error.
// NOTA: la lista se devuelve dinámicamente, por lo que debe ser
// borrada por el usuario.
lista_t* grafo_vertices(grafo_t* grafo);


#endif // GRAFO_H
