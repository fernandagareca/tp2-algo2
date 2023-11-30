#include "juego.h"
#include "lista.h"
#include "hash.h"
#include "tipo.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pokemon.h"
#include "adversario.h"
#include "ataque.h"
#include "funciones_auxiliares.h"

typedef struct jugador {
	lista_t *pokemones;
	int puntaje;
	size_t turnos_restantes;
	struct ataque varios_ataques[9];
	int indice;
} jugador_t;

struct juego {
	lista_t *lista;
	informacion_pokemon_t *info_pokes;
	jugador_t jugador1;
	jugador_t jugador2;
};
juego_t *juego_crear()
{
	juego_t *juego = calloc(1, sizeof(juego_t));
	if (!juego)
		return NULL;
	juego->lista = lista_crear();
	if (!juego->lista) {
		free(juego);
		return NULL;
	}
	juego->jugador1.pokemones = lista_crear();
	if (!juego->jugador1.pokemones) {
		free(juego->lista);
		free(juego);
		return NULL;
	}
	juego->jugador2.pokemones = lista_crear();
	if (!juego->jugador2.pokemones) {
		free(juego->jugador1.pokemones);
		free(juego->lista);
		free(juego);
		return NULL;
	}
	juego->jugador1.turnos_restantes = 9;
	juego->jugador2.turnos_restantes = 9;
	return juego;
}

JUEGO_ESTADO juego_cargar_pokemon(juego_t *juego, char *archivo)
{
	if (!juego || !archivo)
		return ERROR_GENERAL;

	informacion_pokemon_t *info = pokemon_cargar_archivo(archivo);
	if (!info)
		return ERROR_GENERAL;
	//printf("cantidad %i\n",pokemon_cantidad(info));
	if (pokemon_cantidad(info) < 6) {
		pokemon_destruir_todo(info);
		return POKEMON_INSUFICIENTES;
	}
	juego->info_pokes = info;

	return TODO_OK;
}

lista_t *juego_listar_pokemon(juego_t *juego)
{
	if (!juego)
		return NULL;

	if (con_cada_pokemon(juego->info_pokes, funcion, juego->lista) !=
	    pokemon_cantidad(juego->info_pokes)) {
		return NULL;
	}
	return juego->lista;
}

bool repetidos(const char *v1, const char *v2)
{
	return strcmp(v1, v2) == 0;
}
void funcion_insertar(const struct ataque *atake, void *p1)
{
	jugador_t *jugador = p1;
	jugador->varios_ataques[jugador->indice] = *atake;
	jugador->indice++;
}

JUEGO_ESTADO juego_seleccionar_pokemon(juego_t *juego, JUGADOR jugador,
				       const char *nombre1, const char *nombre2,
				       const char *nombre3)
{
	if (!juego || !nombre1 || !nombre2 || !nombre3)
		return ERROR_GENERAL;

	if (repetidos(nombre1, nombre2) || repetidos(nombre1, nombre3) ||
	    repetidos(nombre2, nombre3))
		return POKEMON_REPETIDO;

	pokemon_t *poke1 = pokemon_buscar(juego->info_pokes, nombre1);
	pokemon_t *poke2 = pokemon_buscar(juego->info_pokes, nombre2);
	pokemon_t *poke3 = pokemon_buscar(juego->info_pokes, nombre3);
	if (!poke1 || !poke2 || !poke3)
		return POKEMON_INEXISTENTE;

	if (jugador == JUGADOR1) {
		if (!lista_insertar(juego->jugador1.pokemones, poke1) ||
		    !lista_insertar(juego->jugador1.pokemones, poke2) ||
		    !lista_insertar(juego->jugador2.pokemones, poke3)) {
			return ERROR_GENERAL;
		}
		con_cada_ataque(poke1, funcion_insertar, &(juego->jugador1));
		con_cada_ataque(poke2, funcion_insertar, &(juego->jugador1));
		con_cada_ataque(poke3, funcion_insertar, &(juego->jugador2));
	} else if (jugador == JUGADOR2) {
		if (!lista_insertar_en_posicion(juego->jugador2.pokemones,
						poke2, 0) ||
		    !lista_insertar_en_posicion(juego->jugador2.pokemones,
						poke1, 0) ||
		    !lista_insertar(juego->jugador1.pokemones, poke3)) {
			return ERROR_GENERAL;
		}
		con_cada_ataque(poke1, funcion_insertar, &(juego->jugador2));
		con_cada_ataque(poke2, funcion_insertar, &(juego->jugador2));
		con_cada_ataque(poke3, funcion_insertar, &(juego->jugador1));
	} else {
		return ERROR_GENERAL;
	}
	return TODO_OK;
}

const struct ataque *jugador_ataque(informacion_pokemon_t *info,
				    jugada_t jugada_jugador)
{
	pokemon_t *poke1 = pokemon_buscar(info, jugada_jugador.pokemon);
	return pokemon_buscar_ataque(poke1, jugada_jugador.ataque);
}

RESULTADO_ATAQUE ataque_efectivo(enum TIPO tipo1, enum TIPO tipo2)
{
	if (tipo1 == FUEGO) {
		if (tipo2 == AGUA)
			return ATAQUE_INEFECTIVO;
		return ATAQUE_EFECTIVO;

	} else if (tipo1 == PLANTA) {
		if (tipo2 == FUEGO)
			return ATAQUE_INEFECTIVO;
		return ATAQUE_EFECTIVO;

	} else if (tipo1 == ROCA) {
		if (tipo2 == PLANTA)
			return ATAQUE_INEFECTIVO;
		return ATAQUE_EFECTIVO;

	} else if (tipo1 == ELECTRICO) {
		if (tipo2 == ROCA)
			return ATAQUE_INEFECTIVO;
		return ATAQUE_EFECTIVO;

	} else if (tipo1 == AGUA) {
		if (tipo2 == ELECTRICO)
			return ATAQUE_INEFECTIVO;
		return ATAQUE_EFECTIVO;
	}
	return ATAQUE_REGULAR;
}
int resultado_division(unsigned int numero)
{
	int cociente = (int)(numero / 2);
	float resto = (float)(numero % 2);
	if (resto > 0) {
		cociente++;
	}
	return cociente;
}
void puntos(RESULTADO_ATAQUE resultado, jugador_t *jugador, unsigned int poder)
{
	if (resultado == ATAQUE_EFECTIVO) {
		jugador->puntaje += (int)(poder * 3);
	} else if (resultado == ATAQUE_INEFECTIVO) {
		jugador->puntaje += resultado_division(poder);

	} else {
		jugador->puntaje += (int)poder;
	}
	jugador->turnos_restantes--;
}

bool se_encuntra(jugador_t *jugador, char *nombre_ataque)
{
	for (int i = 0; i < 9; i++) {
		if (strcmp(jugador->varios_ataques[i].nombre, nombre_ataque) ==
		    0) {
			strcpy(jugador->varios_ataques[i].nombre, "usado");
			return true;
		}
	}
	return false;
}
resultado_jugada_t juego_jugar_turno(juego_t *juego, jugada_t jugada_jugador1,
				     jugada_t jugada_jugador2)
{
	resultado_jugada_t resultado;
	resultado.jugador1 = ATAQUE_ERROR;
	resultado.jugador2 = ATAQUE_ERROR;
	if (!juego)
		return resultado;
	pokemon_t *poke1 = lista_buscar_elemento(juego->jugador1.pokemones,
						 comparador_elementos,
						 jugada_jugador1.pokemon);
	pokemon_t *poke2 = lista_buscar_elemento(juego->jugador2.pokemones,
						 comparador_elementos,
						 jugada_jugador2.pokemon);
	if (!poke1 || !poke2) {
		return resultado;
	}

	const struct ataque *ataque_jugador1 =
		jugador_ataque(juego->info_pokes, jugada_jugador1);
	const struct ataque *ataque_jugador2 =
		jugador_ataque(juego->info_pokes, jugada_jugador2);
	if (!se_encuntra(&(juego->jugador1), jugada_jugador1.ataque) ||
	    !se_encuntra(&(juego->jugador2), jugada_jugador2.ataque)) {
		return resultado;
	}

	resultado.jugador1 =
		ataque_efectivo(ataque_jugador1->tipo, ataque_jugador2->tipo);
	resultado.jugador2 =
		ataque_efectivo(ataque_jugador2->tipo, ataque_jugador1->tipo);
	puntos(resultado.jugador1, &(juego->jugador1), ataque_jugador1->poder);
	puntos(resultado.jugador2, &(juego->jugador2), ataque_jugador2->poder);

	return resultado;
}

int juego_obtener_puntaje(juego_t *juego, JUGADOR jugador)
{
	if (!juego)
		return 0;

	if (jugador == JUGADOR1) {
		printf("puntos jugador 1  %i\n", juego->jugador1.puntaje);
		return juego->jugador1.puntaje;
	} else {
		return juego->jugador2.puntaje;
	}
	return 0;
}

bool juego_finalizado(juego_t *juego)
{
	if (!juego)
		return true;
	if (juego->jugador1.turnos_restantes > 0 &&
	    juego->jugador2.turnos_restantes > 0)
		return false;

	return true;
}

void juego_destruir(juego_t *juego)
{
	if (!juego)
		return;
	lista_destruir(juego->lista);
	pokemon_destruir_todo(juego->info_pokes);
	lista_destruir(juego->jugador1.pokemones);
	lista_destruir(juego->jugador2.pokemones);
	free(juego);
}
