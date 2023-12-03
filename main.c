#include "src/pokemon.h"
#include "src/ataque.h"
#include "src/juego.h"
#include "src/lista.h"
#include "src/hash.h"
#include "src/menu.h"
#include "src/adversario.h"
#include "src/funciones_auxiliares.h"
#include <stdio.h>
#include <time.h>

typedef struct info_comando {
	char *descripcion;
	RESULTADO_FUNCION (*funcion)(void *, void *);

} info_comando_t;

typedef struct estado_juego {
	juego_t *juego;
	bool ejecutando;
	menu_t *menu;
	bool archivo_cargado;
} estado_juego_t;
int main()
{
	explicar_juego();
	srand((unsigned)time(NULL));

	juego_t *juego = juego_crear();
	if (!juego)
		return 1;

	menu_t *menu = crear_menu();
	if (!menu) {
		juego_destruir(juego);
		return 1;
	}

	lista_t *lista = juego_listar_pokemon(juego);
	adversario_t *adversario = adversario_crear(lista);

	estado_juego_t estado = { .ejecutando = true,
				  .juego = juego,
				  .menu = menu };
	info_juego_t info = { .adversario = adversario,
			      .turnos = 1,
			      .lista = lista };
	info.pokes_jugador1 = lista_crear();
	info.pokes_jugador2 = lista_crear();
	if (!adversario) {
		juego_destruir(juego);
		menu_destrir(menu);
		lista_destruir(info.pokes_jugador1);
		lista_destruir(info.pokes_jugador2);
		return 1;
	}
	cargar_comandos(menu);
	char linea[200];
	while (!juego_finalizado(juego) && estado.ejecutando) {
		printf(" \n Ingrese un comando o ayuda para mostrar los comandos\n");
		printf(" ➜ ");
		leer_linea(linea);

		RESULTADO_FUNCION comando_ejecutado =
			menu_ejecutar_comando(menu, linea, &estado, &info);
		resultado_comando(comando_ejecutado);
	}
	adversario_destruir(adversario);
	menu_destrir(menu);
	juego_destruir(juego);
	lista_destruir(info.pokes_jugador1);
	lista_destruir(info.pokes_jugador2);
	return 0;
}