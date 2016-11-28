// heap.c
// Alumnos: Fresia Juan Manuel (Padron: 96632)
// 	        Garcia Alejandro Martin (Padron: 96661)

#include <stdbool.h>  /* bool */
#include <stddef.h>	  /* size_t */
#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

#define TAM_INIC 20
#define PASO 2

// Estructura del heap

struct heap {
	void **datos;
	size_t cantidad;
	size_t tamanio;
	cmp_func_f heap_cmp;
	};

// Funciones auxiliares internas

/* Crea un heap con el tamanio y la función de comparación recibidos.
Devuelve un puntero al heap, o NULL en caso contrario
Pre: tam es valido, y la funcion de comparacion tambien.
Post: se creo el heap o se devolvio NULL en caso de error.*/
heap_t* heap_crear_con_tam(cmp_func_f cmp, size_t tam) {
	heap_t *miheap = malloc(sizeof(heap_t));
	if(!miheap)
		return NULL;
	miheap->datos = calloc(tam, sizeof(void*));
	if(!miheap->datos) {
		free(miheap);
		return NULL;
		}
	miheap->tamanio = tam;
	miheap->cantidad = 0;
	miheap->heap_cmp = cmp;
	return miheap;
}

/* Funcion que intercambia los datos de dos punteros genericos.
Post: Los punteros apuntan cada uno al dato que antes apuntaba el otro*/
void swapear_punteros(void **x, void **y) {
	void *aux = *x;
	*x = *y;
	*y = aux;
}

/* Funcion de up-heap, encargada de colocar al ultimo elemento insertado
en su posicion actual.
Pre: el heap fue creado, el ultimo elemento insertado se encuentra en
la posicion heap->cantidad.
Post: el ultimo elemento insertado se encuentra en su posicion correcta,
haciendo que el heap sea un max-heap.*/
void up_heap(heap_t *heap) {
	size_t pos_actual = heap->cantidad, pos_padre = 0;
	if(pos_actual>0)
		pos_padre =  (pos_actual-1)/2;
	// Si el elemento es mayor a su padre, hay que swapearlo
	while(pos_padre >= 0 && heap->heap_cmp(heap->datos[pos_actual], heap->datos[pos_padre]) > 0) {
		swapear_punteros(&heap->datos[pos_actual], &heap->datos[pos_padre]);
		pos_actual = pos_padre;
		if(pos_actual>0)
			pos_padre = (pos_actual - 1)/2;
		} 
}

/* Funcion que recibe un heap y una posicion de hijo, y compara a ese
hijo con su hermano a su derecha. Devuelve la posicion del hermano mayor.
Pre: el heap fue creado, la posicion hijo es valida y tiene un hermano.
Post: se devolvio la posicion del hermano con mayor prioridad.*/
size_t posicion_mayor_hijo(heap_t *heap, size_t hijo) {
	if(heap->heap_cmp(heap->datos[hijo], heap->datos[hijo + 1]) < 0)
		// Si el primer hijo es menor al segundo, devuelvo al segundo
		hijo++;
	return hijo;
}

/* Funcion de down-heap, encargada de colocar los elementos del heap en
su posicion correspondiente luego de un desencolado.
Pre: el heap fue creado, el ultimo elemento insertado se encuentra en
la posicion pos_actual.
Post: los elementos estan en la posicion correcta, haciendo que el heap 
sea un max-heap.*/
void down_heap(heap_t *heap, size_t pos_actual) {
	size_t pos_hijo = 0; 
	if((2 * pos_actual + 1) < heap->cantidad)
		pos_hijo = 2 * pos_actual + 1;
	while(pos_actual < pos_hijo) {
		if((pos_hijo + 1) < heap->cantidad) // Si tiene 2 hijos...
			pos_hijo = posicion_mayor_hijo(heap, pos_hijo);
		if(heap->heap_cmp(heap->datos[pos_actual], heap->datos[pos_hijo])<0)
			// Si el actual es menor que el hijo menor, los swapeo
			swapear_punteros(&heap->datos[pos_actual], &heap->datos[pos_hijo]);
		pos_actual = pos_hijo;
		if((2 * pos_actual + 1) < heap->cantidad)
			pos_hijo = 2 * pos_actual + 1;
		}
}

/* Funcion encargada del redimensionamiento del heap. Si el heap recibido
debe agrandarse o achicarse, lo hace y devuelve el heap modificado. Si no
pudo hacer esa redimension, devuelve NULL. Si el heap no debia ser modificado
devuelve el heap que ya se tenia.
Pre: el heap fue creado.
Post: se devuelve el heap con el tamanio adecuado, o bien NULL si la 
redimension falla.*/
heap_t* redimensionar_heap(heap_t* heap) {
	size_t tam_novo;
	if(heap->cantidad > (heap->tamanio/PASO)) 
		tam_novo = PASO * heap->tamanio; //Agrandar heap
	else if(heap->cantidad <= (heap->tamanio/(2*PASO)) && (heap->tamanio > TAM_INIC))
		tam_novo = (heap->tamanio)/PASO; //Achicar heap
	else
		return heap;
	//Cambio el tamanio del arreglo del heap y lo devuelvo
	void **datos_nuevos = realloc(heap->datos, sizeof(void*) * tam_novo);
	if(!datos_nuevos) 
		return false;
	heap->datos = datos_nuevos;
	heap->tamanio = tam_novo;
	return heap;
}

/* Funcion de heapify que recibe un vector de void* y crea un max-heap
con sus elementos para despues poder ordenarlo con heap_sort.
Pre: el vector fue creado.
Post: se devuelve un max-heap con los elementos del vector o NULL si no
es esto posible. */
heap_t *heapify_vector(void *elementos[], size_t cant, cmp_func_f cmp) {
	heap_t *heap_aux = heap_crear_con_tam(cmp, cant);
	if(!heap_aux)
		return NULL;
	int i;
	heap_aux->cantidad = cant;
	for(i = cant - 1; i >= cant/2; i--) 
		heap_aux->datos[i] = elementos[i];
	for(i = (int) (cant/2 - 1); i >= 0; i--) {
		heap_aux->datos[i] = elementos[i];
		down_heap(heap_aux, i);
		}
	return heap_aux;
}

/*---------------------------------------------
 * 	PRIMITIVAS DE LA COLA DE PRIORIDADES
 *--------------------------------------------- */
 
 /* Función de heapsort genérica. Esta función ordena mediante heap_sort
 * un arreglo de punteros opacos, para lo cual requiere que se
 * le pase una función de comparación. Modifica el arreglo "in-place".
 * Notar que esta función NO es formalmente parte del TAD Heap.*/
void heap_sort(void *elementos[], size_t cant, cmp_func_f cmp) {
	heap_t * heap_aux = heapify_vector(elementos, cant, cmp);
	if(!heap_aux) 
		return;
	int i;
	for(i = cant-1; i >= 0; i--) 
		elementos[i] = heap_desencolar(heap_aux);
	
	heap_destruir(heap_aux, NULL);
}

/* Crea un heap. Recibe como único parámetro la función de comparación a
 * utilizar. Devuelve un puntero al heap, el cual debe ser destruido con
 * heap_destruir(). */
heap_t *heap_crear(cmp_func_f cmp) {
	return heap_crear_con_tam(cmp, TAM_INIC);
}

/* Elimina el heap, llamando a la función dada para cada elemento del mismo.
 * El puntero a la función puede ser NULL, en cuyo caso no se llamará.
 * Post: se llamó a la función indicada con cada elemento del heap. El heap
 * dejó de ser válido. */
void heap_destruir(heap_t *heap, void destruir_elemento(void *e)) {
	if(destruir_elemento) 
		while(!heap_esta_vacio(heap))
			destruir_elemento(heap_desencolar(heap));
	free(heap->datos);
	free(heap);
}

/* Devuelve la cantidad de elementos que hay en el heap. */
size_t heap_cantidad(const heap_t *heap) {
	return heap->cantidad;
}

/* Devuelve true si la cantidad de elementos que hay en el heap es 0, 
false en caso contrario. */
bool heap_esta_vacio(const heap_t *heap) {
	return !heap_cantidad(heap);
}

/* Agrega un elemento al heap. El elemento no puede ser NULL.
 * Devuelve true si fue una operación exitosa, o false en caso de error. 
 * Pre: el heap fue creado.
 * Post: se agregó un nuevo elemento al heap.*/
bool heap_encolar(heap_t *heap, void *elem) {
	heap->datos[heap->cantidad] = elem;
	up_heap(heap);
	heap->cantidad++;
	// Redimensionar heap checkea si hay que agrandar el heap
	heap = redimensionar_heap(heap);
	return heap ? true : false;
}

/* Devuelve el elemento con máxima prioridad. Si el heap esta vacío, 
 * devuelve NULL. 
 * Pre: el heap fue creado.*/
void *heap_ver_max(const heap_t *heap) {
	return heap_esta_vacio(heap) ? NULL : heap->datos[0];
}

/* Elimina el elemento con máxima prioridad, y lo devuelve.
 * Si el heap esta vacío, devuelve NULL.
 * Pre: el heap fue creado.
 * Post: el elemento desencolado ya no se encuentra en el heap. */
void *heap_desencolar(heap_t *heap) {
	void *elmayor = heap_ver_max((const heap_t*) heap);
	if(!elmayor)
		return NULL;
	if(heap_cantidad((const heap_t*) heap) > 1) {
		heap->datos[0] = heap->datos[heap->cantidad-1];
		down_heap(heap, 0);
		}
	else
		heap->datos[0] = NULL; // Si tenia un elemento, ahora lo vacie
	heap->cantidad--;
	// Redimensionar heap checkea si hay que achicar el heap
	heap = redimensionar_heap(heap);	
	return heap ? elmayor : NULL;
}
