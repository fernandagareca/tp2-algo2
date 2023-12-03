#ifndef FUNCIONES_AUXILIARES_H_
#define FUNCIONES_AUXILIARES_H_
#include "lista.h"
#include "juego.h"
#include "adversario.h"
#include "pokemon.h"
#include "ataque.h"
#include "menu.h"
#include "juego.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct poke_usado {
	char *nombre_poke;
	int ataques_usado;
} poke_usado_t;

typedef struct info_juego {
	adversario_t *adversario;
	lista_t *pokes_jugador1;
	lista_t *pokes_jugador2;
	jugada_t jugadas_disponibles[9];
	int turnos;
	lista_t *lista;
	int indice;
} info_juego_t;

int comparador_elementos(void *p1, void *p2);

void funcion(pokemon_t *poke, void *v2);

void explicar_juego();

void resultado_comando(RESULTADO_FUNCION comando_ejecutado);

char *tipo_de_pokemon(enum TIPO tipo);

void leer_linea(char linea[]);

void mostrar_ataque(const struct ataque *ataque, void *p1);

bool mostrar_pokes(void *p1, void *v2);

RESULTADO_FUNCION mostrar_pokemones(void *v1, void *v2);

RESULTADO_FUNCION cargar_archivo_pokes(void *p1, void *p2);

void cargar_lista_jugador(lista_t *lista, char *poke1, char *poke2, char *poke3,
			  lista_t *jugador);

RESULTADO_FUNCION seleccion_de_pokemones(void *p1, void *p2);

RESULTADO_FUNCION mostrar_pokes_usuario(void *p1, void *p2);

RESULTADO_FUNCION mostrar_pokes_adversario(void *p1, void *p2);

RESULTADO_FUNCION cerrar_juego(void *p1, void *p2);

bool mostrar_comando(const char *clave, void *valor, void *contexto);

RESULTADO_FUNCION mostrar_ayuda(void *p1, void *p2);

RESULTADO_FUNCION jugada(void *p1, void *p2);

void cargar_comandos(menu_t *menu);

#endif // FUNCIONES_AUXILIARES_H_
