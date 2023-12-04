#include "menu.h"
#include "funciones_auxiliares.h"

typedef struct estado_juego {
	juego_t *juego;
	bool ejecutando;
	menu_t *menu;
} estado_juego_t;

typedef struct info_comando {
	char *descripcion;
	RESULTADO_FUNCION (*funcion)(void *, void *);

} info_comando_t;

struct menu {
	hash_t *comandos;
};

menu_t *crear_menu()
{
	menu_t *menu = calloc(1, sizeof(menu_t));
	if (!menu)
		return NULL;
	menu->comandos = hash_crear(4);
	if (!menu->comandos) {
		free(menu);
		return NULL;
	}
	return menu;
}
bool menu_agregar_comando(menu_t *menu, char *comando, char *descripcion,
			  RESULTADO_FUNCION (*f)(void *, void *))
{
	if (!menu || !comando || !f || !descripcion)
		return false;

	info_comando_t *info = calloc(1, sizeof(info_comando_t));
	if (!info)
		return false;

	info->descripcion = descripcion;
	info->funcion = f;
	menu->comandos = hash_insertar(menu->comandos, comando, info, NULL);
	if (!menu->comandos) {
		free(info);
		return false;
	}
	return true;
}
RESULTADO_FUNCION menu_ejecutar_comando(menu_t *menu, char *comando,
					void *contexto, void *contexto2)
{
	estado_juego_t *estado = contexto;
	info_comando_t *info = hash_obtener(menu->comandos, comando);
	if (!info) {
		return COMANDO_INEXISTENTE;
	}
	return info->funcion(estado, contexto2);
}

size_t menu_tamanio(menu_t *menu)
{
	if (!menu)
		return 0;
	return hash_cantidad(menu->comandos);
}

size_t menu_con_cada_comando(menu_t *menu,
			     bool (*funcion)(const char *clave, void *valor,
					     void *contexto),
			     void *contexto)
{
	if (!menu)
		return 0;
	return hash_con_cada_clave(menu->comandos, funcion, contexto);
}
void menu_destrir(menu_t *menu)
{
	if (!menu)
		return;
	hash_destruir_todo(menu->comandos, free);
	free(menu);
}