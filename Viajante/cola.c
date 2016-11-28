#include "cola.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La cola está planteada como una cola de punteros genéricos. */

struct _nodo{
	void *dato;
	struct _nodo *puntero;
};

typedef struct _nodo nodo_t;

struct cola {
	nodo_t *primero, *ultimo;
};


/* ******************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

// Crea una cola.
// Post: devuelve una nueva cola vacía.
cola_t* cola_crear() {
	cola_t *p_cola=malloc(sizeof(cola_t));
	if(p_cola==NULL)
		fputs("Error, no hay memoria", stdout);
	p_cola->ultimo=NULL;	
	return p_cola;
	
}

// Destruye la cola. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la cola llama a destruir_dato.
// Pre: la cola fue creada. destruir_dato es una función capaz de destruir
// los datos de la cola, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la cola.
void cola_destruir(cola_t *cola, void destruir_dato(void*)) {
	while(!cola_esta_vacia(cola)) {
		if(destruir_dato) 
			destruir_dato(cola->primero->dato);
		cola_desencolar(cola);
	}
	free(cola);	
}


// Devuelve verdadero o falso, según si la cola tiene o no elementos encolados.
// Pre: la cola fue creada.
bool cola_esta_vacia(const cola_t *cola) {
	return cola->ultimo==NULL;
}

// Agrega un nuevo elemento a la cola. Devuelve falso en caso de error.
// Pre: la cola fue creada.
// Post: se agregó un nuevo elemento a la cola, valor se encuentra al final
// de la cola.
bool cola_encolar(cola_t *cola, void* valor) {
	nodo_t *minodo=malloc(sizeof(nodo_t));
	if(minodo==NULL) {
		fputs("Error, no hay memoria", stdout);
		return false;
		}	
	minodo->dato=valor;
	if(cola_esta_vacia(cola)) {
		cola->primero=minodo;
		cola->ultimo=cola->primero;
		return true;
		}
	else {
		cola->ultimo->puntero=minodo;
		cola->ultimo=minodo;
		return true;
	}
}

// Obtiene el valor del primer elemento de la cola. Si la cola tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la cola fue creada.
// Post: se devolvió el primer elemento de la cola, cuando no está vacía.
void* cola_ver_primero(const cola_t *cola) {
	return cola_esta_vacia(cola) ? NULL : cola->primero->dato;
}

// Saca el primer elemento de la cola. Si la cola tiene elementos, se quita el
// primero de la cola, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la cola fue creada.
// Post: se devolvió el valor del primer elemento anterior, la cola
// contiene un elemento menos, si la cola no estaba vacía.
void* cola_desencolar(cola_t *cola) {
	void *valor;
	if(cola_esta_vacia(cola))
		return NULL;
	else if(cola->primero==cola->ultimo) {
		cola->ultimo=NULL;
		valor=cola->primero->dato;
		free(cola->primero);
		}
	else {
		valor=cola_ver_primero(cola);
		nodo_t *punt_aux=cola->primero;
		cola->primero=cola->primero->puntero;
		free(punt_aux);
		}
	return valor;
}




