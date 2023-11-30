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

menu_t *crear_menu();

bool menu_agregar_comando(menu_t *menu, char *comando, char *descripcion,
			  void (*f)(void *, void *));

bool menu_ejecutar_comando(menu_t *menu, char *comando, void *contexto,
			   void *contexto2);

size_t menu_tamanio(menu_t *menu);

size_t menu_con_cada_comando(menu_t *menu,
			     bool (*funcion)(const char *clave, void *valor,
					     void *contexto),
			     void *contexto);

void menu_destrir(menu_t *menu);

#endif // MENU_H_
