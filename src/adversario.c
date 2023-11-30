#include "lista.h"
#include "juego.h"
#include "adversario.h"
#include "pokemon.h"
#include "ataque.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funciones_auxiliares.h"

struct adversario {
	lista_t *pokes;
	lista_t *elegidos;
	struct ataque varios_ataques[9];
	int indice;
};
typedef struct nombres_ataques {
	struct ataque nombres[3];
	int indice;
} nombres_ataques_t;

adversario_t *adversario_crear(lista_t *pokemon)
{
	adversario_t *ad = calloc(1, sizeof(adversario_t));
	if (!ad)
		return NULL;
	ad->elegidos = lista_crear();
	if (!ad->elegidos) {
		free(ad);
		return NULL;
	}
	ad->pokes = pokemon;
	return ad;
}

bool obtener_nombre(lista_t *lista, char **nombre, int posicion_poke)
{
	size_t pos = (size_t)posicion_poke;
	pokemon_t *poke1 = lista_elemento_en_posicion(lista, pos);
	if (poke1) {
		const char *nombre2 = pokemon_nombre(poke1);

		char *puntero_a_puntero[1];
		puntero_a_puntero[0] = (char *)nombre2;
		*nombre = *puntero_a_puntero;
		return true;
	}
	return false;
}

void copiar_ataque(const struct ataque *at, void *p2)
{
	adversario_t *ad = p2;
	ad->varios_ataques[ad->indice] = *at;
	ad->indice++;
}

bool adversario_seleccionar_pokemon(adversario_t *adversario, char **nombre1,
				    char **nombre2, char **nombre3)
{
	if (!adversario)
		return false;
	size_t cantidad_pokes = lista_tamanio(adversario->pokes);
	int numero1 = (rand() % (int)cantidad_pokes + 0);
	int numero2 = (rand() % (int)cantidad_pokes + 0);
	int numero3 = (rand() % (int)cantidad_pokes + 0);
	printf("tam %li pos1 %i pos2 %i pos3 %i\n", cantidad_pokes, numero1,
	       numero2, numero3);
	while (numero1 == numero2 || numero1 == numero3 || numero3 == numero2) {
		numero1 = (rand() % (int)cantidad_pokes + 0);
		numero2 = (rand() % (int)cantidad_pokes + 0);
		numero3 = (rand() % (int)cantidad_pokes + 0);
	}
	printf("pos1 %i pos2 %i pos3 %i\n", numero1, numero2, numero3);

	if (obtener_nombre(adversario->pokes, nombre1, numero1) &&
	    obtener_nombre(adversario->pokes, nombre2, numero2) &&
	    obtener_nombre(adversario->pokes, nombre3, numero3)) {
		pokemon_t *poke1 = lista_buscar_elemento(
			adversario->pokes, comparador_elementos, *nombre1);
		pokemon_t *poke2 = lista_buscar_elemento(
			adversario->pokes, comparador_elementos, *nombre2);
		lista_insertar(adversario->elegidos, poke1);
		lista_insertar(adversario->elegidos, poke2);
		adversario->indice = 0;
		con_cada_ataque(poke1, copiar_ataque, adversario);
		con_cada_ataque(poke2, copiar_ataque, adversario);
	}

	return false;
}

bool adversario_pokemon_seleccionado(adversario_t *adversario, char *nombre1,
				     char *nombre2, char *nombre3)
{
	if (!adversario)
		return false;

	pokemon_t *poke = lista_buscar_elemento(adversario->pokes,
						comparador_elementos, nombre3);
	lista_insertar(adversario->elegidos, poke);
	con_cada_ataque(poke, copiar_ataque, adversario);
	return true;
}
void mostrar(const struct ataque *p1, void *p2)
{
	nombres_ataques_t *posicion = p2;
	posicion->nombres[posicion->indice] = *p1;
	posicion->indice++;
}

jugada_t adversario_proxima_jugada(adversario_t *adversario)
{
	jugada_t j = { .ataque = "", .pokemon = "" };
	if (!adversario || lista_tamanio(adversario->elegidos) != 3)
		return j;
	bool ataque_encontrado = false;
	nombres_ataques_t opciones;
	opciones.indice = 0;
	pokemon_t *poke = NULL;
	size_t posicion = 0;
	size_t posicion_ataque = 0;

	while (!ataque_encontrado) {
		posicion = (size_t)(rand() % 3 + 0);
		posicion_ataque = (size_t)(rand() % 3 + 0);
		poke = lista_elemento_en_posicion(adversario->elegidos,
						  posicion);

		con_cada_ataque(poke, mostrar, &opciones);
		for (int i = 0; i < 9; i++) {
			if (strcmp(adversario->varios_ataques[i].nombre,
				   opciones.nombres[posicion_ataque].nombre) ==
			    0) {
				ataque_encontrado = true;
			}
		}
		opciones.indice = 0;
	}
	strcpy(j.pokemon, pokemon_nombre(poke));
	strcpy(j.ataque, opciones.nombres[posicion_ataque].nombre);
	printf("%s    %s \n", j.pokemon, j.ataque);
	bool encontrado = false;
	int i = 0;

	while (!encontrado && i < 9) {
		if (strcmp(adversario->varios_ataques[i].nombre, j.ataque) ==
		    0) {
			encontrado = true;
			strcpy(adversario->varios_ataques[i].nombre, "usado");
		}
		i++;
	}

	return j;
}

void adversario_informar_jugada(adversario_t *a, jugada_t j)
{
}

void adversario_destruir(adversario_t *adversario)
{
	if (!adversario)
		return;
	lista_destruir(adversario->elegidos);
	//lista_destruir(adversario->pokes);
	free(adversario);
}