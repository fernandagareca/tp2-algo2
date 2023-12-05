#include "funciones_auxiliares.h"
#include "menu.h"
#include "juego.h"

typedef struct estado_juego {
	juego_t *juego;
	bool ejecutando;
	menu_t *menu;
} estado_juego_t;

typedef struct info_comando {
	char *descripcion;
	RESULTADO_FUNCION (*funcion)(void *, void *);

} info_comando_t;

int comparador_elementos(void *p1, void *p2)
{
	pokemon_t *poke = p1;
	char *pokemon = p2;
	int numero = strcmp(pokemon, pokemon_nombre(poke));
	return numero;
}

void funcion_insertar_pokes(pokemon_t *poke, void *v2)
{
	lista_t *lista = v2;
	lista_insertar(lista, poke);
}

void explicar_juego()
{
	printf(" ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf(" ┃  _____   ______ _______   __  __ __      __ _______  __  __  ______  ______     ____                 ┃\n");
	printf(" ┃ |  _ \\  |_   _| |  ____| | \\ | | \\ \\    / / |  ____| | \\ | | |_   _| |  __ \\   / __ \\                ┃\n");
	printf(" ┃ | |_) |   | |   | |__    |  \\| |  \\ \\  / /  | |__    |  \\| |   | |   | |  | | | |  | |               ┃\n");
	printf(" ┃ |  _ <    | |   |  __|   | . ` |   \\ \\/ /   |  __|   | . ` |   | |   | |  | | | |  | |               ┃\n");
	printf(" ┃ | |_) |  _| |_  | |____  | |\\  |    \\  /    | |____  | |\\  |  _| |_  | |__| | | |__| |               ┃\n");
	printf(" ┃ |____/  |_____| |______| |_| \\_|     \\/     |______| |_| \\_| |_____| |_____/   \\____/                ┃\n");
	printf(" ┃                                                                                                      ┃\n");
	printf(" ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
}

void resultado_comando(RESULTADO_FUNCION comando_ejecutado)
{
	if (comando_ejecutado == COMANDO_INEXISTENTE) {
		printf(" EL COMANDO NO EXISTE \n");
	} else if (comando_ejecutado == ARCHIVO_EXISTENTE) {
		printf(" ya ah sido ingresado una archivo valido\n");
	} else if (comando_ejecutado == POKES_INSUFICIENTES) {
		printf(" no hay suficientes pokemones\n");
	} else if (comando_ejecutado == ERROR_ARCHIVO) {
		printf(" hubo un error con el archivo\n");
	} else if (comando_ejecutado == ARCHIVO_OK) {
		printf(" se cargo el archivo correctante\n");
	} else if (comando_ejecutado == ARCHIVO_INEXISTENTE) {
		printf(" aun no se ingreso un archivo\n");
	} else if (comando_ejecutado == POKES_REPETIDO) {
		printf(" no se permiten pokemon repetidos , no se cargaron los pokemones\n");
	} else if (comando_ejecutado == POKES_INEXIXTENTES) {
		printf(" los pokemones debes encontrarse en la lista,no se cargaron los pokemones\n");
	} else if (comando_ejecutado == ERROR_DE_SELECCION) {
		printf(" la lista esta vacia , aun no se seleccionaron pokemones\n");
	} else if (comando_ejecutado == LISTA_NO_VACIA) {
		printf(" la lista ya a sido llenada\n");
	} else if (comando_ejecutado == JUGADA_INVALIDA) {
		printf(" el pokemon o ataque no son validos\n");
	} else if (comando_ejecutado == POKEMONES_CORRECTOS) {
		printf(" Los pokemones se cargaron correctamnete\n");
	}
}

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

void leer_linea(char linea[])
{
	char *leido;
	leido = fgets(linea, 200, stdin);
	if (leido != NULL) {
		linea[strlen(linea) - 1] = 0;
	}
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
	printf(" ┃______________________________________________\n");
	con_cada_ataque(poke, mostrar_ataque, poke);
	printf(" ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

	return true;
}

RESULTADO_FUNCION mostrar_pokemones(void *v1, void *v2)
{
	info_juego_t *info = v2;
	if (lista_vacia(info->lista))
		return ARCHIVO_INEXISTENTE;
	printf("  ━━━━━━━━━━━━ LISTA DE POKEMONES ━━━━━━━━━━━━━━\n");
	lista_con_cada_elemento(info->lista, mostrar_pokes, NULL);
	return TODO_BIEN;
}

RESULTADO_FUNCION cargar_archivo_pokes(void *p1, void *p2)
{
	estado_juego_t *estado = p1;
	info_juego_t *info = p2;

	if (!lista_vacia(info->lista))
		return ARCHIVO_EXISTENTE;
	char archivo[50];
	printf(" ingrese un archivo de pokemones\n");
	printf(" ▶ ");
	leer_linea(archivo);

	JUEGO_ESTADO estado_cargar =
		juego_cargar_pokemon(estado->juego, archivo);
	if (estado_cargar == ERROR_GENERAL)
		return ERROR_ARCHIVO;

	if (estado_cargar == POKEMON_INSUFICIENTES)
		return POKES_INSUFICIENTES;
	info->lista = juego_listar_pokemon(estado->juego);
	return ARCHIVO_OK;
}

void completar_datos(lista_t *lista, char *poke1, lista_t *jugador)
{
	pokemon_t *pokemon1 =
		lista_buscar_elemento(lista, comparador_elementos, poke1);
	lista_insertar(jugador, pokemon1);
}
void cargar_lista_jugador(lista_t *lista, char *poke1, char *poke2, char *poke3,
			  lista_t *jugador)
{
	completar_datos(lista, poke1, jugador);
	completar_datos(lista, poke2, jugador);
	completar_datos(lista, poke3, jugador);
}

void completar_jugadas(const struct ataque *at, void *p1)
{
	info_juego_t *info = p1;
	strcpy(info->jugadas_disponibles[info->indice].ataque, at->nombre);
	info->indice++;
}
void cargar_jugadas(info_juego_t *info)
{
	int pos_pokemon = 0;
	info->indice = 0;
	for (size_t i = 0; i < 3; i++) {
		pokemon_t *poke =
			lista_elemento_en_posicion(info->pokes_jugador1, i);
		for (int j = 0; j < 3; j++) {
			strcpy(info->jugadas_disponibles[pos_pokemon].pokemon,
			       pokemon_nombre(poke));
			pos_pokemon++;
		}
		con_cada_ataque(poke, completar_jugadas, info);
	}
}
RESULTADO_FUNCION seleccion_de_pokemones(void *p1, void *p2)
{
	estado_juego_t *estado = p1;
	info_juego_t *info = p2;

	if (lista_vacia(info->lista))
		return ARCHIVO_INEXISTENTE;
	if (!lista_vacia(info->pokes_jugador1))
		return LISTA_NO_VACIA;

	char *nombre1 = NULL;
	char *nombre2 = NULL;
	char *nombre3 = NULL;
	char poke1[50];
	char poke2[50];
	char poke3[50];
	printf(" seleccione 3 pokemones de la lista\n");
	printf(" ▶ ");
	leer_linea(poke1);
	printf(" ▶ ");
	leer_linea(poke2);
	printf(" ▶ ");
	leer_linea(poke3);

	JUEGO_ESTADO estado_pokes = juego_seleccionar_pokemon(
		estado->juego, JUGADOR1, poke1, poke2, poke3);
	if (estado_pokes == TODO_OK) {
		bool estado_adversario = adversario_seleccionar_pokemon(
			info->adversario, &nombre1, &nombre2, &nombre3);
		if (estado_adversario) {
			adversario_pokemon_seleccionado(info->adversario, poke1,
							poke2, poke3);
			JUEGO_ESTADO estdo_jugador2 = juego_seleccionar_pokemon(
				estado->juego, JUGADOR2, nombre1, nombre2,
				nombre3);
			if (estdo_jugador2 == TODO_OK) {
				cargar_lista_jugador(info->lista, poke1, poke2,
						     nombre3,
						     info->pokes_jugador1);
				cargar_lista_jugador(info->lista, nombre1,
						     nombre2, poke3,
						     info->pokes_jugador2);

				cargar_jugadas(info);
				return POKEMONES_CORRECTOS;
			}
		}
	}
	if (estado_pokes == POKEMON_REPETIDO)
		return POKES_REPETIDO;
	if (estado_pokes == POKEMON_INEXISTENTE)
		return POKES_INEXIXTENTES;
	return ERROR;
}

RESULTADO_FUNCION mostrar_pokes_usuario(void *p1, void *p2)
{
	info_juego_t *info = p2;
	if (lista_vacia(info->pokes_jugador1))
		return ERROR_DE_SELECCION;
	printf(" ━━━━━━━━━━━━━━ POKEMONES DEL JUGADOR ━━━━━━━━━━━━━━\n");
	lista_con_cada_elemento(info->pokes_jugador1, mostrar_pokes, NULL);
	return TODO_BIEN;
}

RESULTADO_FUNCION mostrar_pokes_adversario(void *p1, void *p2)
{
	info_juego_t *info = p2;
	if (lista_vacia(info->pokes_jugador2))
		return ERROR_DE_SELECCION;
	printf(" ━━━━━━━━━━━━━━ POKEMONES DEL ADVERSARIO ━━━━━━━━━━━━━━\n");
	lista_con_cada_elemento(info->pokes_jugador2, mostrar_pokes, NULL);
	return TODO_BIEN;
}

RESULTADO_FUNCION cerrar_juego(void *p1, void *p2)
{
	estado_juego_t *estado = p1;
	estado->ejecutando = false;
	return TODO_BIEN;
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

RESULTADO_FUNCION mostrar_ayuda(void *p1, void *p2)
{
	estado_juego_t *estado = p1;
	printf(" ━━━━━━━━━━━━━━ LISTA DE COMANDOS ━━━━━━━━━━━━━━━━━━━\n");
	menu_con_cada_comando(estado->menu, mostrar_comando, NULL);
	return TODO_BIEN;
}

bool verificar_datos(jugada_t jugada, info_juego_t *info)
{
	for (int i = 0; i < 9; i++) {
		if (strcmp(info->jugadas_disponibles[i].pokemon,
			   jugada.pokemon) == 0 &&
		    strcmp(info->jugadas_disponibles[i].ataque,
			   jugada.ataque) == 0) {
			strcpy(info->jugadas_disponibles[i].ataque, "usado");
			strcpy(info->jugadas_disponibles[i].pokemon, "usado");
			return true;
		}
	}
	return false;
}
void mostrar(jugada_t jugador, jugada_t adversario, int turnos,
	     int puntos_jugador, int puntos_adversario)
{
	printf("┏━━━━━━━━━━━━━━━━━━━ PARTIDA n° %i ━━━━━━━━━━━━━━━━━━━━━━━━━\n",
	       turnos);

	printf(" ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf(" ┃ Seleccion del usuario  \n");
	printf(" ┃ POKEMON %s  ATAQUE  %s\n", jugador.pokemon, jugador.ataque);
	printf(" ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf(" ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf(" ┃ Seleccion del adversario  \n");
	printf(" ┃ POKEMON %s  ATAQUE  %s\n", adversario.pokemon,
	       adversario.ataque);
	printf(" ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

	printf(" ----------------------------------------------------------\n");
	printf(" Resultado: Puntos_jugador %i      Puntos adversario  %i\n",
	       puntos_jugador, puntos_adversario);
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}

RESULTADO_FUNCION jugada(void *p1, void *p2)
{
	estado_juego_t *estado = p1;
	info_juego_t *info = p2;
	jugada_t jugada_adversario = { .ataque = "", .pokemon = "" };
	jugada_t jugada_jugador = { .ataque = "", .pokemon = "" };

	if (lista_vacia(info->lista))
		return ARCHIVO_INEXISTENTE;
	if (lista_vacia(info->pokes_jugador1))
		return ERROR_DE_SELECCION;

	printf(" Ingrese un pokemon (sin espacios)\n");
	printf(" ━➤ ");
	leer_linea(jugada_jugador.pokemon);
	printf(" Ingrese un ataque (sin espacios)\n");
	printf(" ━➤ ");
	leer_linea(jugada_jugador.ataque);
	bool jugada_valida = verificar_datos(jugada_jugador, info);
	if (jugada_valida) {
		jugada_adversario = adversario_proxima_jugada(info->adversario);
	}
	resultado_jugada_t resultado = juego_jugar_turno(
		estado->juego, jugada_jugador, jugada_adversario);
	if (resultado.jugador1 == ATAQUE_ERROR ||
	    resultado.jugador2 == ATAQUE_ERROR)
		return JUGADA_INVALIDA;
	int puntos_jugador = juego_obtener_puntaje(estado->juego, JUGADOR1);
	int puntos_adversario = juego_obtener_puntaje(estado->juego, JUGADOR2);
	mostrar(jugada_jugador, jugada_adversario, info->turnos, puntos_jugador,
		puntos_adversario);
	if (info->turnos == 9) {
		(puntos_jugador > puntos_adversario) ? printf("GANASTE\n") :
						       printf("PERDISTE\n");
	}
	info->turnos++;

	return TODO_BIEN;
}

void cargar_comandos(menu_t *menu)
{
	menu_agregar_comando(menu, "listar", "muestra los pokemones",
			     mostrar_pokemones);
	menu_agregar_comando(menu, "cargar", "carga un archivo",
			     cargar_archivo_pokes);
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
}
