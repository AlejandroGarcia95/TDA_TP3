
#include "pila.h"
#include <stdio.h>
#include <stdlib.h>


#define TAM_INIC 4
#define PASO 2

//Estructura pila_t

struct _pila {
    void** datos;
    size_t tamanio;
    size_t cantidad;
};

//typedef struct _pila pila_t;

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/
// Crea una pila.
// Post: devuelve una nueva pila vacía.
pila_t* pila_crear() {
	pila_t* p_pila= malloc(sizeof(pila_t));
	if(p_pila==NULL)
		fputs("Error, no hay memoria", stdout);
	p_pila->datos=malloc(sizeof(void*)*TAM_INIC);
	if(p_pila->datos==NULL) {
		fputs("Error, no hay memoria", stdout);
		free(p_pila);
		}
	p_pila->cantidad=0;
	p_pila->tamanio=TAM_INIC;
	return p_pila;
}

// Destruye la pila.
// Pre: la pila fue creada.
// Post: se eliminaron todos los elementos de la pila.
void pila_destruir(pila_t *pila)  {
	free(pila->datos);
	free(pila);
}

// Devuelve verdadero o falso, según si la pila tiene o no elementos apilados.
// Pre: la pila fue creada.
bool pila_esta_vacia(const pila_t *pila) {
	if(pila->cantidad==0)
		return true;
	else
		return false;
}

// Agrega un nuevo elemento a la pila. Devuelve falso en caso de error.
// Pre: la pila fue creada.
// Post: se agregó un nuevo elemento a la pila, valor es el nuevo tope.
bool pila_apilar(pila_t *pila, void *valor) {
	pila->datos[pila->cantidad]=valor;
	pila->cantidad+=1;
	if((pila->cantidad)>((pila->tamanio)/PASO)) {
		void **datos_nuevos;
		size_t tam_novo=PASO*(pila->tamanio);
		//LA LINEA CONFLICTIVA:
		datos_nuevos=realloc(pila->datos, sizeof(void*)*tam_novo);
		if(datos_nuevos==NULL) 
			return false;
		pila->datos=datos_nuevos;
		pila->tamanio=tam_novo;
		}	
	return true;
}

// Obtiene el valor del tope de la pila. Si la pila tiene elementos,
// se devuelve el valor del tope. Si está vacía devuelve NULL.
// Pre: la pila fue creada.
// Post: se devolvió el valor del tope de la pila, cuando la pila no está
// vacía, NULL en caso contrario.
void* pila_ver_tope(const pila_t *pila) {
	if(pila->cantidad==0)
		return NULL;
	void *eltope=pila->datos[pila->cantidad-1];
	return eltope;
}

// Saca el elemento tope de la pila. Si la pila tiene elementos, se quita el
// tope de la pila, y se devuelve ese valor. Si la pila está vacía, devuelve 
// NULL.
// Pre: la pila fue creada.
// Post: si la pila no estaba vacía, se devuelve el valor del tope anterior 
// y la pila contiene un elemento menos.
void* pila_desapilar(pila_t *pila) {
	if(pila->cantidad==0)
		return NULL;
	void *eltope=pila_ver_tope(pila);
	pila->cantidad-=1;
	
	if(((pila->cantidad)<=((pila->tamanio)/(2*PASO)))&&(pila->tamanio>TAM_INIC)) {
		void **datos_nuevos;
		size_t tam_novo=(pila->tamanio)/PASO;
		datos_nuevos=realloc(pila->datos, sizeof(void*)*tam_novo);
		if(datos_nuevos==NULL) 
			return NULL;
		pila->datos=datos_nuevos;
		pila->tamanio=tam_novo;
		}	
	return eltope;
}







