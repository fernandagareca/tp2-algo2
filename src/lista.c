#include "lista.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

struct lista {
	nodo_t *nodo_inicio;
	nodo_t *nodo_final;
	size_t cantidad;
};

struct lista_iterador {
	nodo_t *nodo_actual;
	lista_t *lista;
};

lista_t *lista_crear()
{
	return calloc(1, sizeof(lista_t));
}

nodo_t *crear_nodo_nuevo(void *elemento)
{
	nodo_t *nodo_nuevo = calloc(1, sizeof(nodo_t));
	if (!nodo_nuevo) {
		return NULL;
	}
	nodo_nuevo->elemento = elemento;
	return nodo_nuevo;
}
lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if (!lista) {
		return NULL;
	}

	nodo_t *nodo_nuevo = crear_nodo_nuevo(elemento);
	if (!nodo_nuevo) {
		return NULL;
	}

	if (lista_vacia(lista)) {
		lista->nodo_inicio = nodo_nuevo;
	} else {
		lista->nodo_final->siguiente = nodo_nuevo;
	}
	lista->nodo_final = nodo_nuevo;
	lista->cantidad++;

	return lista;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if (!lista) {
		return NULL;
	}

	if (posicion >= lista->cantidad || lista_vacia(lista)) {
		return lista_insertar(lista, elemento);
	}
	nodo_t *nodo_nuevo = crear_nodo_nuevo(elemento);
	if (!nodo_nuevo) {
		return NULL;
	}

	nodo_t *nodo_actual = lista->nodo_inicio;

	if (posicion == 0) {
		lista->nodo_inicio = nodo_nuevo;
		nodo_nuevo->siguiente = nodo_actual;
		lista->cantidad++;
		return lista;
	}

	for (int i = 1; i < posicion; i++) {
		nodo_actual = nodo_actual->siguiente;
	}
	nodo_nuevo->siguiente = nodo_actual->siguiente;
	nodo_actual->siguiente = nodo_nuevo;
	lista->cantidad++;

	return lista;
}

void *lista_quitar(lista_t *lista)
{
	if (!lista || lista_vacia(lista)) {
		return NULL;
	}

	nodo_t *nodo_actual = lista->nodo_inicio;
	void *elemento_buscado = lista->nodo_final->elemento;
	nodo_t *nodo_aux = lista->nodo_final;

	if (lista_tamanio(lista) == 1) {
		lista->nodo_inicio = NULL;
		lista->nodo_final = NULL;
	} else {
		for (int i = 1; i < lista->cantidad - 1; i++) {
			nodo_actual = nodo_actual->siguiente;
		}

		nodo_actual->siguiente = NULL;
		lista->nodo_final = nodo_actual;
	}
	free(nodo_aux);
	lista->cantidad--;

	return elemento_buscado;
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if (!lista || lista_vacia(lista)) {
		return NULL;
	}

	if (posicion >= lista_tamanio(lista)) {
		return lista_quitar(lista);
	}
	nodo_t *nodito = lista->nodo_inicio;
	nodo_t *nodo_aux;
	void *elemento;

	if (posicion == 0) {
		nodo_aux = lista->nodo_inicio;
		elemento = nodo_aux->elemento;
		lista->nodo_inicio = lista->nodo_inicio->siguiente;
	} else {
		for (int i = 1; i < posicion; i++) {
			nodito = nodito->siguiente;
		}
		nodo_aux = nodito->siguiente;
		nodito->siguiente = nodo_aux->siguiente;
		elemento = nodo_aux->elemento;
	}
	free(nodo_aux);
	lista->cantidad--;
	return elemento;
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if (!lista || posicion >= lista_tamanio(lista)) {
		return NULL;
	}

	nodo_t *nodito = lista->nodo_inicio;
	for (int i = 0; i < posicion; i++) {
		nodito = nodito->siguiente;
	}

	return nodito->elemento;
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	if (!lista || lista_vacia(lista) || !comparador) {
		return NULL;
	}

	nodo_t *nodito = lista->nodo_inicio;
	bool encontrado = false;
	int i = 0;

	while (!encontrado && i < lista->cantidad) {
		int valor = comparador(nodito->elemento, contexto);
		if (valor == 0) {
			encontrado = true;
		} else {
			nodito = nodito->siguiente;
			i++;
		}
	}
	if (!encontrado) {
		return NULL;
	}
	return nodito->elemento;
}

void *lista_primero(lista_t *lista)
{
	if (!lista || lista_vacia(lista)) {
		return NULL;
	}
	return lista->nodo_inicio->elemento;
}

void *lista_ultimo(lista_t *lista)
{
	if (!lista || lista_vacia(lista)) {
		return NULL;
	}
	return lista->nodo_final->elemento;
}

bool lista_vacia(lista_t *lista)
{
	if (!lista || lista_tamanio(lista) == 0) {
		return true;
	}
	return false;
}

size_t lista_tamanio(lista_t *lista)
{
	if (!lista) {
		return 0;
	}
	return lista->cantidad;
}

void lista_destruir(lista_t *lista)
{
	if (!lista) {
		return;
	}

	while (lista_tamanio(lista) > 0) {
		nodo_t *nodito = lista->nodo_inicio;
		lista->nodo_inicio = lista->nodo_inicio->siguiente;
		free(nodito);
		lista->cantidad--;
	}

	free(lista);
}

void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (!lista) {
		return;
	}
	if (!funcion) {
		lista_destruir(lista);
		return;
	}

	while (lista_tamanio(lista) > 0) {
		nodo_t *nodito = lista->nodo_inicio;

		funcion(nodito->elemento);
		lista->nodo_inicio = lista->nodo_inicio->siguiente;
		free(nodito);
		lista->cantidad--;
	}
	free(lista);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista) {
		return NULL;
	}

	lista_iterador_t *iterador = malloc(sizeof(lista_iterador_t));
	if (!iterador) {
		return NULL;
	}
	iterador->nodo_actual = lista->nodo_inicio;
	iterador->lista = lista;

	return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if (!iterador || iterador->lista == NULL ||
	    lista_vacia(iterador->lista) || iterador->nodo_actual == NULL) {
		return false;
	}
	return true;
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (!iterador || iterador->lista == NULL ||
	    lista_vacia(iterador->lista)) {
		return false;
	}

	if (iterador->nodo_actual->siguiente == NULL) {
		iterador->nodo_actual = iterador->nodo_actual->siguiente;
		return false;
	}
	if (iterador->nodo_actual->siguiente != NULL) {
		iterador->nodo_actual = iterador->nodo_actual->siguiente;
		return true;
	}

	return false;
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if (!iterador || iterador->lista == NULL ||
	    lista_vacia(iterador->lista) || iterador->nodo_actual == NULL) {
		return NULL;
	}

	return iterador->nodo_actual->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	if (!iterador) {
		return;
	}

	free(iterador);
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{
	if (!lista || !funcion || lista_vacia(lista)) {
		return 0;
	}

	bool avanzar = true;
	size_t contador = 0;
	nodo_t *nodo = lista->nodo_inicio;

	while (contador < lista_tamanio(lista) && avanzar) {
		avanzar = funcion(nodo->elemento, contexto);
		contador++;
		nodo = nodo->siguiente;
	}

	return contador;
}
