#include "lista.h"
#include "hash.h"
#include "grafo.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "heap.h"

// Prim es O(n a la 2)

#define FALSE 0
#define TRUE 1
#define INFINITO 99500

// Estructura auxiliar para guardar información de los vértices en
// cada uno de los recorridos
struct vert_data{
	char* vertice;
	int distancia;
};

// Destructor de vert_data adaptado para el heap
void vert_data_destruir(void *e){
	struct vert_data* vActual = (struct vert_data*) e;
	free(vActual);
}

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



hash_t* ejecutar_prim(grafo_t* grafo, char *raiz){
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
	// Destruyo las estructuras de visitados y distancias
	hash_destruir(distancias);
	hash_destruir(nodos_visitados);
	return padres;
}

grafo_t* crear_ejemplo(){
	grafo_t* grafo_ej = grafo_crear();
	if(!grafo_ej) return NULL;
	grafo_crear_vertice(grafo_ej, "a");
	grafo_crear_vertice(grafo_ej, "b");
	grafo_crear_vertice(grafo_ej, "c");
	grafo_crear_vertice(grafo_ej, "d");
	grafo_crear_vertice(grafo_ej, "e");
	grafo_crear_vertice(grafo_ej, "f");
	grafo_crear_vertice(grafo_ej, "g");
	grafo_crear_vertice(grafo_ej, "h");
	grafo_crear_vertice(grafo_ej, "i");
	
	grafo_crear_arista(grafo_ej, "a", "b", FALSE, 4);
	grafo_crear_arista(grafo_ej, "b", "c", FALSE, 8);
	grafo_crear_arista(grafo_ej, "c", "d", FALSE, 7);	
	grafo_crear_arista(grafo_ej, "d", "e", FALSE, 9);
	grafo_crear_arista(grafo_ej, "e", "f", FALSE, 10);
	grafo_crear_arista(grafo_ej, "f", "g", FALSE, 2);
	grafo_crear_arista(grafo_ej, "g", "h", FALSE, 1);
	grafo_crear_arista(grafo_ej, "h", "a", FALSE, 8);
	grafo_crear_arista(grafo_ej, "h", "b", FALSE, 11);
	grafo_crear_arista(grafo_ej, "h", "i", FALSE, 7);	
	grafo_crear_arista(grafo_ej, "c", "i", FALSE, 2);
	grafo_crear_arista(grafo_ej, "i", "g", FALSE, 6);
	grafo_crear_arista(grafo_ej, "c", "f", FALSE, 4);	
	grafo_crear_arista(grafo_ej, "f", "d", FALSE, 14);
	
	grafo_crear_arista(grafo_ej, "b", "a", FALSE, 4);
	grafo_crear_arista(grafo_ej, "c", "b", FALSE, 8);
	grafo_crear_arista(grafo_ej, "d", "c", FALSE, 7);	
	grafo_crear_arista(grafo_ej, "e", "d", FALSE, 9);
	grafo_crear_arista(grafo_ej, "f", "e", FALSE, 10);
	grafo_crear_arista(grafo_ej, "g", "f", FALSE, 2);
	grafo_crear_arista(grafo_ej, "h", "g", FALSE, 1);
	grafo_crear_arista(grafo_ej, "a", "h", FALSE, 8);
	grafo_crear_arista(grafo_ej, "b", "h", FALSE, 11);
	grafo_crear_arista(grafo_ej, "i", "h", FALSE, 7);	
	grafo_crear_arista(grafo_ej, "i", "c", FALSE, 2);
	grafo_crear_arista(grafo_ej, "g", "i", FALSE, 6);
	grafo_crear_arista(grafo_ej, "f", "c", FALSE, 4);	
	grafo_crear_arista(grafo_ej, "d", "f", FALSE, 14);
	
	return grafo_ej;
	
}

int main(){
	//printf("Hola mundo!\r\n");
	grafo_t* ng = crear_ejemplo();
	hash_t* resultados = ejecutar_prim(ng, "a");
	
	printf("Lista de nodos y padres!\r\n");
	printf("Nodo %s tiene de padre al %s\r\n", "a", (char*)hash_obtener(resultados, "a"));
	printf("Nodo %s tiene de padre al %s\r\n", "b", (char*)hash_obtener(resultados, "b"));
	printf("Nodo %s tiene de padre al %s\r\n", "c", (char*)hash_obtener(resultados, "c"));
	printf("Nodo %s tiene de padre al %s\r\n", "d", (char*)hash_obtener(resultados, "d"));
	printf("Nodo %s tiene de padre al %s\r\n", "e", (char*)hash_obtener(resultados, "e"));
	printf("Nodo %s tiene de padre al %s\r\n", "f", (char*)hash_obtener(resultados, "f"));
	printf("Nodo %s tiene de padre al %s\r\n", "g", (char*)hash_obtener(resultados, "g"));
	printf("Nodo %s tiene de padre al %s\r\n", "h", (char*)hash_obtener(resultados, "h"));
	printf("Nodo %s tiene de padre al %s\r\n", "i", (char*)hash_obtener(resultados, "i"));
	hash_destruir(resultados);
	grafo_destruir(ng);
	return 0;
}

