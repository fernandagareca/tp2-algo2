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
typedef struct pokes_usados {
	char nombre_pokes[50];
	lista_t *ataques;
	int posibilidades;
	int indice;
} pokes_usados_t;

struct adversario {
	lista_t *pokes;
	lista_t *elegidos;
	pokes_usados_t usados[3];
};

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
	ad->usados[0].ataques = lista_crear();
	ad->usados[1].ataques = lista_crear();
	ad->usados[2].ataques = lista_crear();
	if (!ad->usados[0].ataques || !ad->usados[1].ataques ||
	    !ad->usados[2].ataques) {
		free(ad->elegidos);
		free(ad->usados[0].ataques);
		free(ad->usados[1].ataques);
		free(ad->usados[2].ataques);
		free(ad);
		return NULL;
	}
	return ad;
}

pokemon_t *obtener_nombre(lista_t *lista, char **nombre, int posicion_poke)
{
	size_t pos = (size_t)posicion_poke;
	pokemon_t *poke1 = lista_elemento_en_posicion(lista, pos);
	if (poke1) {
		*nombre = (char *)pokemon_nombre(poke1);
		return poke1;
	}
	return NULL;
}

void cargar_vector(const struct ataque *at, void *p2)
{
	adversario_t *ad = p2;
	lista_insertar(ad->usados[ad->usados->indice].ataques,
		       (struct ataque *)at);
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
	while (numero1 == numero2 || numero1 == numero3 || numero3 == numero2) {
		numero1 = (rand() % (int)cantidad_pokes + 0);
		numero2 = (rand() % (int)cantidad_pokes + 0);
		numero3 = (rand() % (int)cantidad_pokes + 0);
	}

	pokemon_t *poke1 = obtener_nombre(adversario->pokes, nombre1, numero1);
	pokemon_t *poke2 = obtener_nombre(adversario->pokes, nombre2, numero2);
	pokemon_t *poke3 = obtener_nombre(adversario->pokes, nombre3, numero3);
	if (poke1 && poke2 && poke3) {
		lista_insertar(adversario->elegidos, poke1);
		lista_insertar(adversario->elegidos, poke2);
		strcpy(adversario->usados[0].nombre_pokes, *nombre1);
		strcpy(adversario->usados[1].nombre_pokes, *nombre2);
		con_cada_ataque(poke1, cargar_vector, adversario);
		adversario->usados->indice++;
		con_cada_ataque(poke2, cargar_vector, adversario);
		adversario->usados->indice++;
		return true;
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
	strcpy(adversario->usados[2].nombre_pokes, nombre3);
	con_cada_ataque(poke, cargar_vector, adversario);

	return true;
}

jugada_t adversario_proxima_jugada(adversario_t *adversario)
{
	jugada_t j = { .ataque = "", .pokemon = "" };
	if (!adversario)
		return j;

	size_t posicion =
		(size_t)(rand() % (int)lista_tamanio(adversario->elegidos) + 0);

	pokemon_t *poke =
		lista_elemento_en_posicion(adversario->elegidos, posicion);
	char *nombre = (char *)pokemon_nombre(poke);

	bool encontrado = false;
	int segundo_numero = 0;
	const struct ataque *ataque_buscado;
	for (int i = 0; i < 3; i++) {
		if (strcmp(adversario->usados[i].nombre_pokes, nombre) == 0 &&
		    !encontrado) {
			encontrado = true;
			segundo_numero =
				rand() %
					(int)lista_tamanio(
						adversario->usados[i].ataques) +
				0;
			ataque_buscado = lista_elemento_en_posicion(
				adversario->usados[i].ataques,
				(size_t)segundo_numero);
			strcpy(j.ataque, ataque_buscado->nombre);
			strcpy(j.pokemon, pokemon_nombre(poke));
			lista_quitar_de_posicion(adversario->usados[i].ataques,
						 (size_t)segundo_numero);
		}
	}
	bool entro = false;
	for (int i = 0; i < 3; i++) {
		if (strcmp(adversario->usados[i].nombre_pokes, j.pokemon) ==
		    0) {
			adversario->usados[i].posibilidades++;
			if (adversario->usados[i].posibilidades == 3 &&
			    !entro) {
				entro = true;
				lista_quitar_de_posicion(adversario->elegidos,
							 posicion);
				strcpy(adversario->usados[i].nombre_pokes,
				       "usado");
			}
		}
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
	lista_destruir(adversario->usados[0].ataques);
	lista_destruir(adversario->usados[1].ataques);
	lista_destruir(adversario->usados[2].ataques);
	free(adversario);
}
