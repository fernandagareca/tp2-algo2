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

typedef struct estado_juego {
	juego_t *juego;
	bool ejecutando;
	menu_t *menu;
	bool archivo_cargado;
} estado_juego_t;

typedef struct info_comando {
	char *descripcion;
	void (*funcion)(void *, void *);

} info_comando_t;

typedef struct info_juego {
	adversario_t *adversario;
	lista_t *pokes_jugador1;
	lista_t *pokes_jugador2;
	jugada_t jugada_jugador;
	jugada_t jugada_adversario;
	int turnos;
	bool listo_para_empezar;
} info_juego_t;

char *tipo_de_pokemon(enum TIPO tipo)
{
	if (tipo == ELECTRICO)
		return "Electrico";
	if (tipo == ROCA)
		return "Roca";
	if (tipo == AGUA)
		return "Agua";
	if (tipo == FUEGO)
		return "Fuego";
	if (tipo == NORMAL)
		return "Normal";

	return "Planta";
}

void mostrar_ataque(const struct ataque *ataque, void *p1)
{
	const struct ataque *at = ataque;
	printf(" ┃ ATAQUE : %s (tipo:%s daño :%i)\n", at->nombre,
	       tipo_de_pokemon(at->tipo), at->poder);
}

bool mostrar_pokes(void *p1, void *v2)
{
	pokemon_t *poke = p1;
	enum TIPO tipo = pokemon_tipo(poke);
	printf(" ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf(" ┃ NOMBRE :%s TIPO(%s)     \n", pokemon_nombre(poke),
	       tipo_de_pokemon(tipo));
	printf(" ┃-----------------------------------------------\n");
	con_cada_ataque(poke, mostrar_ataque, poke);
	printf(" ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

	return true;
}

void mostrar_pokemones(void *v1, void *v2)
{
	estado_juego_t *estado = v1;
	lista_t *lista = juego_listar_pokemon(estado->juego);
	if (!lista_vacia(lista)) {
		printf("  ━━━━━━━━━━━━ LISTA DE POKEMONES ━━━━━━━━━━━━━━\n");

		lista_con_cada_elemento(lista, mostrar_pokes, NULL);
	} else {
		printf("no hay un archivo cargado aun\n");
	}
}

void cargar_archivo(void *p1, void *p2)
{
	estado_juego_t *estado = p1;
	if (estado->archivo_cargado == true) {
		printf(" Ya fue cargado un archivo valido\n");
		return;
	}
	/*char ruta[20];
    strcpy(ruta,"ejemplos/");
    char archivo[20];
    printf(" ingrese un archivo de pokemones\n");
    printf(" ▶");
    scanf("%s",archivo);
    strcat(ruta,archivo);*/

	JUEGO_ESTADO estado_cargar =
		juego_cargar_pokemon(estado->juego, "ejemplos/correcto.txt");
	if (estado_cargar == ERROR_GENERAL) {
		printf(" ubo un error al cargar archivo\n");
	} else if (estado_cargar == POKEMON_INSUFICIENTES) {
		printf(" el archivo no es valido para jugar\n");
	} else {
		printf(" se cargo el archivo con exito\n");
		estado->archivo_cargado = true;
	}
}

void cargar_lista_jugador(lista_t *lista, char *poke1, char *poke2, char *poke3,
			  lista_t *jugador)
{
	pokemon_t *pokemon1 =
		lista_buscar_elemento(lista, comparador_elementos, poke1);
	pokemon_t *pokemon2 =
		lista_buscar_elemento(lista, comparador_elementos, poke2);
	pokemon_t *pokemon3 =
		lista_buscar_elemento(lista, comparador_elementos, poke3);
	lista_insertar(jugador, pokemon1);
	lista_insertar(jugador, pokemon2);
	lista_insertar(jugador, pokemon3);
}
void seleccion_de_pokemones(void *p1, void *p2)
{
	estado_juego_t *estado = p1;
	info_juego_t *info = p2;
	if (estado->archivo_cargado == false) {
		printf("aun no hay un archivo cargado\n");
		return;
	}
	char *nombre1 = NULL;
	char *nombre2 = NULL;
	char *nombre3 = NULL;
	char poke1[20];
	char poke2[20];
	char poke3[20];
	printf(" seleccione 3 pokemones de la lista\n");
	printf(" ▶");
	scanf("%s", poke1);
	printf(" ▶");
	scanf("%s", poke2);
	printf(" ▶");
	scanf("%s", poke3);
	lista_t *lista = juego_listar_pokemon(estado->juego);
	JUEGO_ESTADO estado_pokes = juego_seleccionar_pokemon(
		estado->juego, 0, poke1, poke2, poke3);
	if (estado_pokes == TODO_OK) {
		adversario_seleccionar_pokemon(info->adversario, &nombre1,
					       &nombre2, &nombre3);

		adversario_pokemon_seleccionado(info->adversario, poke1, poke2,
						poke3);
		if (juego_seleccionar_pokemon(estado->juego, 1, nombre1,
					      nombre2, nombre3) == TODO_OK) {
			cargar_lista_jugador(lista, poke1, poke2, nombre3,
					     info->pokes_jugador1);
			cargar_lista_jugador(lista, nombre1, nombre2, poke3,
					     info->pokes_jugador2);
			info->listo_para_empezar = true;
		}
	} else if (estado_pokes == POKEMON_REPETIDO) {
		printf("no se permiten pokemon repetidos , no se cargaron los pokemones\n");
	} else if (estado_pokes == POKEMON_INEXISTENTE) {
		printf("los pokemones debes encontrarse en la lista,no se cargaron los pokemones\n");
	} else {
		printf("hubo un problema ,no se cargaron los pokemones\n");
	}
}

void mostrar_pokes_usuario(void *p1, void *p2)
{
	info_juego_t *info = p2;
	if (!lista_vacia(info->pokes_jugador1)) {
		printf(" ━━━━━━━━━━━━━━ POKEMONES DEL JUGADOR ━━━━━━━━━━━━━━\n");
		if (lista_con_cada_elemento(info->pokes_jugador1, mostrar_pokes,
					    NULL) ==
		    lista_tamanio(info->pokes_jugador1)) {
			return;
		}
	}
	printf("la lista esta vacia ,aun no se seleccionaron pokes\n");
}

void mostrar_pokes_adversario(void *p1, void *p2)
{
	info_juego_t *info = p2;
	if (!lista_vacia(info->pokes_jugador2)) {
		printf(" ━━━━━━━━━━━━━━ POKEMONES DEL ADVERSARIO ━━━━━━━━━━━━━━\n");
		if (lista_con_cada_elemento(info->pokes_jugador2, mostrar_pokes,
					    NULL) ==
		    lista_tamanio(info->pokes_jugador2)) {
			return;
		}
	}
	printf("la lista esta vacia ,aun no se seleccionaron pokes\n");
}

void cerrar_juego(void *p1, void *p2)
{
	estado_juego_t *estado = p1;
	estado->ejecutando = false;
}

bool mostrar_comando(const char *clave, void *valor, void *contexto)
{
	info_comando_t *info = valor;
	printf(" ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf(" ┃ COMANDO: %s\n", clave);
	printf(" ┃ FUNCIONALIDAD: %s\n", info->descripcion);
	printf(" ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	return true;
}

void mostrar_ayuda(void *p1, void *p2)
{
	estado_juego_t *estado = p1;
	printf(" ━━━━━━━━━━━━━━ LISTA DE COMANDOS ━━━━━━━━━━━━━━━━━━━\n");
	menu_con_cada_comando(estado->menu, mostrar_comando, NULL);
}

void jugada(void *p1, void *p2)
{
	estado_juego_t *estado = p1;
	info_juego_t *info = p2;
	if (estado->archivo_cargado == false) {
		printf("aun no hay un archivo cargado\n");
		return;
	} else if (info->listo_para_empezar == false) {
		printf("aun no se seleccionaron pokemones para jugar\n");
		return;
	}
	int puntos_jugador = juego_obtener_puntaje(estado->juego, 0);
	int puntos_adversario = juego_obtener_puntaje(estado->juego, 1);
	printf("┏━━━━━━━━━━━━━━━━━━━ PARTIDA n° %i ━━━━━━━━━━━━━━━━━━━━━━━━━\n",
	       info->turnos);
	printf("┃ Puntos Jugador %i                    Puntos Adversario %i \n",
	       puntos_jugador, puntos_adversario);
	printf("┃ Ingrese un pokemon\n");
	printf("┃ ━➤ ");
	scanf("%s", info->jugada_jugador.pokemon);
	printf("┃ Ingrese un ataque\n");
	printf("┃ ━➤ ");
	scanf("%s", info->jugada_jugador.ataque);
	jugada_t jugada_adversario =
		adversario_proxima_jugada(info->adversario);
	resultado_jugada_t resultado = juego_jugar_turno(
		estado->juego, info->jugada_jugador, jugada_adversario);
	if (resultado.jugador1 == ATAQUE_ERROR ||
	    resultado.jugador2 == ATAQUE_ERROR) {
		printf("la jugada ingresada no es valida ,no se proceso la jugada\n");
		return;
	}
	printf(" ----------------------------------------------------------\n");
	printf(" Resultado: Puntos_jugador :%i      Puntos adversario : %i\n",
	       juego_obtener_puntaje(estado->juego, 0),
	       juego_obtener_puntaje(estado->juego, 1));
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	info->turnos++;
}
void leer_linea()
{
	char linea[200];
	char *leido;
	leido = fgets(linea, 200, stdin);
	if (leido != NULL) {
		linea[strlen(linea) - 1] = 0;
	}
}
int main()
{
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
				  .menu = menu,
				  .archivo_cargado = false };
	info_juego_t info = { .adversario = adversario,
			      .turnos = 0,
			      .listo_para_empezar = false };
	info.pokes_jugador1 = lista_crear();
	info.pokes_jugador2 = lista_crear();
	if (!adversario) {
		juego_destruir(juego);
		menu_destrir(menu);
		lista_destruir(info.pokes_jugador1);
		lista_destruir(info.pokes_jugador2);
		return 1;
	}
	menu_agregar_comando(menu, "listar", "muestra los pokemones",
			     mostrar_pokemones);
	menu_agregar_comando(menu, "cargar", "carga un archivo",
			     cargar_archivo);
	menu_agregar_comando(menu, "seleccionar", "selecciona pokemones",
			     seleccion_de_pokemones);
	menu_agregar_comando(menu, "salir", "sale del juego", cerrar_juego);
	menu_agregar_comando(menu, "ayuda", "muestra info de comandos",
			     mostrar_ayuda);
	menu_agregar_comando(menu, "pokemones",
			     "muestra los pokemones del usuario",
			     mostrar_pokes_usuario);
	menu_agregar_comando(menu, "adversario",
			     "muestra los pokemones del adversario",
			     mostrar_pokes_adversario);
	menu_agregar_comando(menu, "jugada", "ingresa una jugada", jugada);
	char linea[200];
	while (!juego_finalizado(juego) && estado.ejecutando) {
		printf(" selecciones un comando o ayuda para mostrar los comandos\n");
		printf(" ➜ ");
		leer_linea();

		bool comando_ejecutado =
			menu_ejecutar_comando(menu, linea, &estado, &info);
		if (!comando_ejecutado) {
			printf(" no se pudo ejecutar el comando \n");
		}
	}
	adversario_destruir(adversario);
	menu_destrir(menu);
	juego_destruir(juego);
	lista_destruir(info.pokes_jugador1);
	lista_destruir(info.pokes_jugador2);
	return 0;
}