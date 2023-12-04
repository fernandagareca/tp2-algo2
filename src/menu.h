#ifndef MENU_H_
#define MENU_H_
#include "hash.h"
#include "tipo.h"
#include "juego.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct menu menu_t;

typedef enum {
	COMANDO_INEXISTENTE,
	TODO_BIEN,
	ARCHIVO_EXISTENTE,
	ERROR_ARCHIVO,
	POKES_INSUFICIENTES,
	POKES_REPETIDO,
	POKES_INEXIXTENTES,
	ARCHIVO_OK,
	ARCHIVO_INEXISTENTE,
	ERROR,
	ERROR_DE_SELECCION,
	JUGADA_INVALIDA,
	LISTA_NO_VACIA,
	POKEMONES_CORRECTOS
} RESULTADO_FUNCION;

menu_t *crear_menu();

bool menu_agregar_comando(menu_t *menu, char *comando, char *descripcion,
			  RESULTADO_FUNCION (*f)(void *, void *));

RESULTADO_FUNCION menu_ejecutar_comando(menu_t *menu, char *comando,
					void *contexto, void *contexto2);

size_t menu_tamanio(menu_t *menu);

size_t menu_con_cada_comando(menu_t *menu,
			     bool (*funcion)(const char *clave, void *valor,
					     void *contexto),
			     void *contexto);

void menu_destrir(menu_t *menu);

#endif // MENU_H_
