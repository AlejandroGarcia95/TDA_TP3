#include "lista.h"
#include "hash.h"
#include "grafo.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "pila.h"
#include "heap.h"

// Prim es O(n a la 2)

#define FALSE 0
#define TRUE 1
#define INFINITO 99500

// Estructura auxiliar para guardar información de los vértices en Prim
struct vert_data{
	char* vertice;
	int distancia;
};

// Destructor de vert_data adaptado para el heap
void vert_data_destruir(void *e){
	struct vert_data* vActual = (struct vert_data*) e;
	free(vActual);
}

// Función de creación de vert_data
struct vert_data* vert_data_crear(char* vert, int dist){
	struct vert_data* vd = malloc(sizeof(struct vert_data));
	if(!vd)
		return NULL;
	vd->vertice = vert;
	vd->distancia = dist;
	return vd;
}

int comparar(const void *a, const void *b){
	struct vert_data* real_a = (struct vert_data*)a;
	struct vert_data* real_b = (struct vert_data*)b;
	return real_b->distancia -real_a->distancia;
}

/* Recibe un grafo y devuelve una lista con el recorrido DFS de ese
grafo, iniciando en el vértice origen (primer elem. de la lista). 
Pre: grafo es válido, origen pertenece al grafo.
Post: se devolvió una lista con los vértices del grafo recorridos en
DFS, o NULL en caso de error.*/
lista_t* recorrido_dfs(grafo_t* grafo, char *origen){
	hash_t* nodos_visitados = hash_crear(NULL);
	if(!nodos_visitados) return NULL;
	// Inicializo todos los visitados como FALSE
	lista_t* vertices = grafo_vertices(grafo);		
	lista_iter_t *l_it = lista_iter_crear(vertices);
	if(!l_it) {
		hash_destruir(nodos_visitados);
		return NULL;
		}
	while(!lista_iter_al_final(l_it)){
		char* v = (char*) lista_iter_ver_actual(l_it);			
		hash_guardar(nodos_visitados, v, (void*)FALSE);
		lista_iter_avanzar(l_it);
		}	
	lista_iter_destruir(l_it);
	lista_destruir(vertices, NULL);
	// Lista de resultados
	lista_t* resultados = lista_crear();
	if(!resultados){
		hash_destruir(nodos_visitados);
		return NULL;
		}
	// Creo la pila para ir apilando los nodos
	pila_t* q = pila_crear();
	if(!q){
		hash_destruir(nodos_visitados);
		lista_destruir(resultados, NULL);
		return NULL;
		}
	pila_apilar(q, origen); // Apilo el nodo inicial
	while(!pila_esta_vacia(q)){
		char* vActual = (char*)pila_desapilar(q);
		hash_guardar(nodos_visitados, vActual, (void*)TRUE);
		// Itero en todos los adyacentes al nodo desapilado
		hash_t* ady = grafo_devolver_adyacentes(grafo, vActual);
		hash_iter_t* h_it = hash_iter_crear(ady);
		if(!h_it){
			pila_destruir(q);	
			hash_destruir(nodos_visitados);
			lista_destruir(resultados, NULL);	
			return FALSE;
			}
		while(!hash_iter_al_final(h_it)){
			char* adyActual = (char*)hash_iter_ver_actual(h_it);
			if(!(int)hash_obtener(nodos_visitados, adyActual))
				pila_apilar(q, adyActual);
			hash_iter_avanzar(h_it);
			}
		hash_iter_destruir(h_it);
		lista_insertar_ultimo(resultados, vActual);
		}
	// Destruyo las estructuras aux. y devuelvo la lista
	hash_destruir(nodos_visitados);
	pila_destruir(q);
	return resultados;
}

/* Función auxiliar usada por el algoritmo de Prim. Convierte el hash
de (nodos, padres) en un grafo nuevo. 
Pre: padres es un hash en formato válido.
Post: se devolvió un grafo siguiendo la estructura del hash recibido o
NULL en caso de error.*/
grafo_t* hash_tree_to_graph(hash_t* padres){
	grafo_t* elGrafo = grafo_crear();
	if(!elGrafo) return NULL;
	// Primero agrego los vértices
	hash_iter_t* h_it = hash_iter_crear(padres);
	if(!h_it){
		grafo_destruir(elGrafo);
		return FALSE;
		}
	while(!hash_iter_al_final(h_it)){
		char* vActual = (char*) hash_iter_ver_actual(h_it);
		grafo_crear_vertice(elGrafo, vActual);
		hash_iter_avanzar(h_it);
		}
	hash_iter_destruir(h_it);
	// Para cada vertice del arbol, lo uno con su padre
	lista_t* vertices = grafo_vertices(elGrafo);		
	lista_iter_t *l_it = lista_iter_crear(vertices);
	if(!l_it) {
		grafo_destruir(elGrafo);		
		return NULL;
		}	
	while(!lista_iter_al_final(l_it)){
		char* hijoAct = (char*) lista_iter_ver_actual(l_it);			
		char* padreAct = (char*) hash_obtener(padres, hijoAct);
		if(hijoAct && padreAct){
			grafo_crear_arista(elGrafo, hijoAct, padreAct, FALSE, 1);
			grafo_crear_arista(elGrafo, padreAct, hijoAct, FALSE, 1);
			}
		lista_iter_avanzar(l_it);
		}
	lista_iter_destruir(l_it);
	lista_destruir(vertices, NULL);
	return elGrafo;
}

/* Ejecuta el algoritmo de Prim en el grafo recibido, usando como
raíz para el árbol devuelto la recibida. Devuelve un grafo que
representa el árbol de tendido mínimo. En caso de error, devuelve
NULL. se ejecuta en O(n^2) donde n = |V(G)|.
Pre: grafo es válido, raiz pertenece a grafo.
Post: se devuelve un árbol de tendido mínimo en foma de grafo, o NULL
en caso de error.*/
grafo_t* ejecutar_prim(grafo_t* grafo, char *raiz){
	// Primero creo las 3 estructuras auxiliares necesarias:
	// distancias, nodos_visitados y padres, todas como diccionarios
	hash_t* distancias = hash_crear(NULL);
	if(!distancias)
		return NULL;
	hash_t* nodos_visitados = hash_crear(NULL);
	if(!nodos_visitados){
		hash_destruir(distancias);
		return NULL;
		}
	hash_t* padres = hash_crear(free);
	if(!nodos_visitados){
		hash_destruir(distancias);
		hash_destruir(nodos_visitados);
		return NULL;
		}
	// Inicializo distancias, padres y nodos_visitados
	lista_t* vertices = grafo_vertices(grafo);		
	lista_iter_t *l_it = lista_iter_crear(vertices);
	if(!l_it) {
		hash_destruir(distancias);
		hash_destruir(padres);	
		hash_destruir(nodos_visitados);		
		return NULL;
		}	
	while(!lista_iter_al_final(l_it)){
		char* v = (char*) lista_iter_ver_actual(l_it);			
		hash_guardar(nodos_visitados, v, (void*)FALSE);
		hash_guardar(distancias, v, (void*)INFINITO);
		hash_guardar(padres, v, NULL);
		lista_iter_avanzar(l_it);
		}
	lista_iter_destruir(l_it);
	// Inicializo la distancia para el origen
	hash_guardar(distancias, raiz, 0);
	int cantVisitados = 0;
	heap_t* q = heap_crear(comparar);
	if(!q){
		hash_destruir(distancias);
		hash_destruir(padres);	
		hash_destruir(nodos_visitados);	
		lista_destruir(vertices, NULL);
		return NULL;
		}
	struct vert_data* inicial = vert_data_crear(raiz, 0);
	if(!inicial){
		lista_destruir(vertices, NULL);
		heap_destruir(q, NULL);
		hash_destruir(distancias);
		hash_destruir(padres);	
		hash_destruir(nodos_visitados);	
		return NULL;
		}
	// Encolo nodo inicial
	heap_encolar(q, (void*) inicial);
	// Bucle principal del algoritmo
	while((!heap_esta_vacio(q)) && (cantVisitados < lista_largo(vertices))){
		struct vert_data* vActual = heap_desencolar(q);
		// Si el nodo no fue visitado, lo visito
		if(!(int)hash_obtener(nodos_visitados, vActual->vertice)){
			cantVisitados++;
			hash_guardar(nodos_visitados, vActual->vertice, (void*)TRUE);
			}
		// Itero en todos los adyacentes al nodo desencolado
		hash_t* ady = grafo_devolver_adyacentes(grafo, vActual->vertice);
		hash_iter_t* h_it = hash_iter_crear(ady);
		if(!h_it){
			heap_destruir(q, vert_data_destruir);
			lista_destruir(vertices, NULL);
			hash_destruir(distancias);
			hash_destruir(padres);	
			hash_destruir(nodos_visitados);	
			return FALSE;
			}
		while(!hash_iter_al_final(h_it)){ // for w adyacente a vActual
			char* w = (char*) hash_iter_ver_actual(h_it); 
			if((!(int)hash_obtener(nodos_visitados,w)) && ((int)hash_obtener(distancias,w) > grafo_devolver_peso_arista(grafo,vActual->vertice,w))){
				hash_guardar(distancias, w, (void*)grafo_devolver_peso_arista(grafo,vActual->vertice,w));
				char* cadenita = (char*)malloc(sizeof(char)*(strlen(vActual->vertice)+1));
				strcpy(cadenita, vActual->vertice);
				hash_guardar(padres, w, cadenita);
				struct vert_data* wAdy = vert_data_crear(w,grafo_devolver_peso_arista(grafo,vActual->vertice,w)); 
				if(!wAdy){
					lista_destruir(vertices, NULL);
					heap_destruir(q, free);
					hash_destruir(distancias);
					hash_destruir(padres);	
					hash_destruir(nodos_visitados);	
					hash_iter_destruir(h_it);
					return NULL;
					}					
				// Encolo w adyacente
				heap_encolar(q, (void*) wAdy);				
				}
			hash_iter_avanzar(h_it);
			}
		hash_iter_destruir(h_it);
		vert_data_destruir((void*)vActual);
		}
	
	heap_destruir(q, vert_data_destruir);	
	lista_destruir(vertices, NULL);
	
	grafo_t* arbolito = hash_tree_to_graph(padres);
	// Destruyo las estructuras de visitados, padres y distancias
	hash_destruir(distancias);
	hash_destruir(nodos_visitados);
	hash_destruir(padres);
	return arbolito;
}

// Función auxiliar para crear un grafo de ejemplo. Matar luego
grafo_t* crear_ejemplo(){
	grafo_t* grafo_ej = grafo_crear();
	if(!grafo_ej) return NULL;
	grafo_crear_vertice(grafo_ej, "a");
	grafo_crear_vertice(grafo_ej, "b");
	grafo_crear_vertice(grafo_ej, "c");
	grafo_crear_vertice(grafo_ej, "d");
	grafo_crear_vertice(grafo_ej, "e");
	
	grafo_crear_arista(grafo_ej, "a", "b", FALSE, 100);
	grafo_crear_arista(grafo_ej, "b", "c", FALSE, 50);
	grafo_crear_arista(grafo_ej, "c", "d", FALSE, 100);	
	grafo_crear_arista(grafo_ej, "d", "e", FALSE, 50);
	grafo_crear_arista(grafo_ej, "e", "a", FALSE, 75);
	grafo_crear_arista(grafo_ej, "a", "d", FALSE, 100);
	grafo_crear_arista(grafo_ej, "a", "c", FALSE, 300);
	grafo_crear_arista(grafo_ej, "d", "b", FALSE, 75);
	grafo_crear_arista(grafo_ej, "b", "e", FALSE, 125);
	grafo_crear_arista(grafo_ej, "c", "e", FALSE, 125);	 
	
	grafo_crear_arista(grafo_ej, "b", "a", FALSE, 100);
	grafo_crear_arista(grafo_ej, "c", "b", FALSE, 50);
	grafo_crear_arista(grafo_ej, "d", "c", FALSE, 100);	
	grafo_crear_arista(grafo_ej, "e", "d", FALSE, 50);
	grafo_crear_arista(grafo_ej, "a", "e", FALSE, 75);
	grafo_crear_arista(grafo_ej, "d", "a", FALSE, 100);
	grafo_crear_arista(grafo_ej, "c", "a", FALSE, 300);
	grafo_crear_arista(grafo_ej, "b", "d", FALSE, 75);
	grafo_crear_arista(grafo_ej, "e", "b", FALSE, 125);
	grafo_crear_arista(grafo_ej, "e", "c", FALSE, 125);	 
	
	return grafo_ej;
	
}

int main(){
	grafo_t* ng = crear_ejemplo();
	char* vertInicial = "a";
	grafo_t* arbolito = ejecutar_prim(ng, vertInicial);
	lista_t* lista_dfs = recorrido_dfs(arbolito, vertInicial);
	
	// Muestro los resultados:
	int pesoAcum = 0;
	char* vAnterior = NULL;
	lista_iter_t* lit = lista_iter_crear(lista_dfs);
	printf("Recorrido final obtenido: ");
	while(!lista_iter_al_final(lit)){
		char* vAct = (char*) lista_iter_ver_actual(lit);
		printf("%s ", vAct);
		if(vAnterior)
			pesoAcum += grafo_devolver_peso_arista(ng, vAnterior, vAct);
		vAnterior = vAct;
		lista_iter_avanzar(lit);
		}
	lista_iter_destruir(lit);
	pesoAcum += grafo_devolver_peso_arista(ng, vAnterior, vertInicial);
	printf("\r\nPeso final del tour: %d\r\n", pesoAcum);
	lista_destruir(lista_dfs, NULL);

	grafo_destruir(arbolito);
	grafo_destruir(ng);
	return 0;
}

