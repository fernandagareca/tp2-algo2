#include <stdlib.h>
#include "pokemon.h"
#include "tipo.h"
#include <stdio.h>
#include "ataque.h"
#include <string.h>
#include <stdbool.h>

#define MAX_NOMBRE 200
#define MAX_TIPO 10
#define MAX_ATAQUES 3
#define FORMATO_LECTURA_POKEMON "%[^;];%c"
#define FORMATO_LECTURA_INFO_POKEMON "%[^;];%c;%i"
#define LECTURA "r"
const char TIPO_NORMAL = 'N';
const char TIPO_FUEGO = 'F';
const char TIPO_AGUA = 'A';
const char TIPO_PLANTA = 'P';
const char TIPO_ELECTRICO = 'E';
const char TIPO_ROCA = 'R';
const int MINIMO_CARACTERES = 0;
const int PODER_MINIMO = 0;
const int CANTIDAD_DATOS_POKEMON = 2;
const int CANTIDAD_DATOS_INFO_POKEMON = 3;
const int POSICION_PRIMER_ATAQUE = 0;
const int POSICION_SEGUNDO_ATAQUE = 1;
const int POSICION_TERCER_ATAQUE = 2;
const int LINEA_POKEMON = 1;
const int LINEA_PRIMNER_ATAQUE = 2;
const int LINEA_SEGUNDO_ATAQUE = 3;
const int LINEA_TERCER_ATAQUE = 4;
const int MAX_CARACTERES = 200;
const int MAX_LINEAS_POKEMON = 4;

struct pokemon {
	char nombre[MAX_NOMBRE];
	enum TIPO tipo_pokemon;
	struct ataque ataques[MAX_ATAQUES];
};

struct info_pokemon {
	pokemon_t **pokemones;
	int cantidad;
};

/*
 *pre: recibe el texto y la posicion actual en el texto. 
 * post: devuelve el tipo segun el caracter 
 */
enum TIPO tipo_pokemon(char tipo)
{
	enum TIPO tipo_buscado;
	if (tipo == TIPO_ELECTRICO) {
		tipo_buscado = ELECTRICO;
	} else if (tipo == TIPO_ROCA) {
		tipo_buscado = ROCA;
	} else if (tipo == TIPO_AGUA) {
		tipo_buscado = AGUA;
	} else if (tipo == TIPO_FUEGO) {
		tipo_buscado = FUEGO;
	} else if (tipo == TIPO_NORMAL) {
		tipo_buscado = NORMAL;
	} else {
		tipo_buscado = PLANTA;
	}

	return tipo_buscado;
}
/* 
 *pre: Recibe un string.
 * post:Verifica que el string tenga algun caracter.
 */
bool existe_un_nombre(const char *nombre)
{
	return strlen(nombre) > MINIMO_CARACTERES;
}

/* 
 *pre: Recibe un tipo.
 * post:verifica que el tipo sea algun tipo valido.
 */
bool existe_un_tipo(char tipo)
{
	return (tipo == TIPO_AGUA || tipo == TIPO_ELECTRICO ||
		tipo == TIPO_FUEGO || tipo == TIPO_NORMAL ||
		tipo == TIPO_PLANTA || tipo == TIPO_ROCA);
}

/* 
 *pre: recibe un poder.
 * post:verifica que sea mayor a 0.
 */
bool existe_un_poder(int poder)
{
	return (poder > PODER_MINIMO);
}

/* 
 *pre: Recibe la linea actual recibe un pokemon.
 * post:completa los datos del pokemon y verifica que los datos dean correctos.
 */
void leer_pokemon(const char *linea, pokemon_t *pokemon, bool *datos_correctos)
{
	char tipo_poke = ' ';
	int leidos = sscanf(linea, FORMATO_LECTURA_POKEMON, pokemon->nombre,
			    &tipo_poke);

	if (leidos != CANTIDAD_DATOS_POKEMON ||
	    !existe_un_nombre(pokemon->nombre) || !existe_un_tipo(tipo_poke)) {
		*datos_correctos = false;
		return;
	}
	pokemon->tipo_pokemon = tipo_pokemon(tipo_poke);
}

/* 
 *pre: Recibe la linea actual un pokemon un ataque 
 * post: Completa el ataque del pokemon y verifgica que los datos sean correctos.
 */
void leer_ataques(const char *linea, pokemon_t *pokemon, struct ataque *ataque,
		  bool *datos_correctos)
{
	char tipo_ataque = ' ';
	int poder = 0;

	int leidos = sscanf(linea, FORMATO_LECTURA_INFO_POKEMON, ataque->nombre,
			    &tipo_ataque, &poder);

	if (leidos != CANTIDAD_DATOS_INFO_POKEMON ||
	    !existe_un_nombre(ataque->nombre) || !existe_un_tipo(tipo_ataque) ||
	    !existe_un_poder(poder)) {
		*datos_correctos = false;
		return;
	}
	ataque->tipo = tipo_pokemon(tipo_ataque);
	ataque->poder = (unsigned int)poder;
}

/* 
 *pre: Recibe la linea actual un pokemon un contador lineas.
 * post:Completa los ataques y al pokemon segun un orden .
 */
void completar_pokemon(const char *linea_poke, pokemon_t *poke,
		       bool *poke_listo, int contador_lineas, bool *correcto)
{
	if (contador_lineas == LINEA_POKEMON) {
		leer_pokemon(linea_poke, poke, correcto);

	} else if (contador_lineas == LINEA_PRIMNER_ATAQUE) {
		leer_ataques(linea_poke, poke,
			     &(poke->ataques[POSICION_PRIMER_ATAQUE]),
			     correcto);

	} else if (contador_lineas == LINEA_SEGUNDO_ATAQUE) {
		leer_ataques(linea_poke, poke,
			     &(poke->ataques[POSICION_SEGUNDO_ATAQUE]),
			     correcto);

	} else if (contador_lineas == LINEA_TERCER_ATAQUE) {
		leer_ataques(linea_poke, poke,
			     &(poke->ataques[POSICION_TERCER_ATAQUE]),
			     correcto);
		*poke_listo = true;
	}
}
/*
 *pre: Recibo un pokemon valido la info de pokemones distinta de null .
 * post:agrego un pokemon valido al vector y aumento su tamaño tambien reinicio el contador y las variables booleanas.
 */
void guardar_pokemon(bool *problema_memoria, pokemon_t poke,
		     informacion_pokemon_t *info, int *contador,
		     bool *poke_listo)
{
	pokemon_t *pokemon_listo = calloc(1, sizeof(pokemon_t));
	if (pokemon_listo == NULL) {
		*problema_memoria = true;
	}
	*pokemon_listo = poke;
	pokemon_t **pokemon = realloc(info->pokemones,
				      (long unsigned int)(1 + info->cantidad) *
					      (sizeof(pokemon_t *)));

	if (pokemon != NULL) {
		info->pokemones = pokemon;
		info->pokemones[info->cantidad] = pokemon_listo;
		info->cantidad++;
	} else {
		free(pokemon_listo);
		*problema_memoria = true;
	}
	*contador = 0;
	*poke_listo = false;
}
informacion_pokemon_t *pokemon_cargar_archivo(const char *path)
{
	if (path == NULL) {
		return NULL;
	}

	FILE *archivo = fopen(path, LECTURA);
	if (archivo == NULL) {
		return NULL;
	}
	informacion_pokemon_t *info = calloc(1, sizeof(informacion_pokemon_t));
	if (info == NULL) {
		fclose(archivo);
		return NULL;
	}
	char linea_poke[MAX_CARACTERES];
	bool poke_listo = false;
	int contador = 0;
	bool leido_correctamente = true;
	bool problema_memoria = false;

	pokemon_t poke;
	while (fgets(linea_poke, MAX_CARACTERES, archivo) != NULL &&
	       leido_correctamente && !problema_memoria) {
		if (contador < MAX_LINEAS_POKEMON) {
			contador++;
			completar_pokemon(linea_poke, &poke, &poke_listo,
					  contador, &leido_correctamente);
		}
		if (poke_listo && leido_correctamente) {
			guardar_pokemon(&problema_memoria, poke, info,
					&contador, &poke_listo);
		}
	}
	if (info->cantidad == 0) {
		fclose(archivo);
		free(info);
		return NULL;
	}

	fclose(archivo);
	return info;
}

pokemon_t *pokemon_buscar(informacion_pokemon_t *ip, const char *nombre)
{
	if (ip == NULL || nombre == NULL) {
		return NULL;
	}
	bool encontrado = false;

	int i = 0;

	pokemon_t *poke;

	while (i < ip->cantidad && !encontrado) {
		poke = &(*ip->pokemones[i]);
		if (strcmp(poke->nombre, nombre) == 0) {
			encontrado = true;
		}
		i++;
	}
	if (!encontrado) {
		return NULL;
	}
	return poke;
}

int pokemon_cantidad(informacion_pokemon_t *ip)
{
	if (ip == NULL) {
		return 0;
	}
	return ip->cantidad;
}

const char *pokemon_nombre(pokemon_t *pokemon)
{
	if (pokemon == NULL) {
		return NULL;
	}
	return pokemon->nombre;
}

/* 
 *pre: Recibe el nombre de un ataque y un ataque 
 * post: verifica si coinciden los datos del nombre.
 */
bool buscar_ataque(const char *nombre, struct ataque ataque)
{
	return (strcmp(ataque.nombre, nombre) == 0);
}

enum TIPO pokemon_tipo(pokemon_t *pokemon)
{
	if (pokemon == NULL) {
		return 0;
	}
	return pokemon->tipo_pokemon;
}

const struct ataque *pokemon_buscar_ataque(pokemon_t *pokemon,
					   const char *nombre)
{
	if (pokemon == NULL || nombre == NULL) {
		return NULL;
	}
	struct ataque *ataque;

	bool ataque_encontrado = false;
	for (int i = 0; i < 3; i++) {
		if (buscar_ataque(nombre, pokemon->ataques[i])) {
			ataque = &(pokemon->ataques[i]);
			ataque_encontrado = true;
		}
	}
	if (!ataque_encontrado) {
		return NULL;
	}

	return ataque;
}

/* 
 *pre: Recibe la info de pos pokemones.
 * post:ordena los pokemones por orden alfabetico.
 */
void ordenar_pokes(informacion_pokemon_t *info)
{
	pokemon_t *poke_aux;
	for (int i = 1; i < info->cantidad; i++) {
		for (int j = 0; j < info->cantidad - i; j++) {
			const char *primer_poke =
				pokemon_nombre(info->pokemones[j]);
			const char *segundo_poke =
				pokemon_nombre(info->pokemones[j + 1]);
			if (strcmp(primer_poke, segundo_poke) > 0) {
				poke_aux = info->pokemones[j];
				info->pokemones[j] = info->pokemones[j + 1];
				info->pokemones[j + 1] = poke_aux;
			}
		}
	}
}
int con_cada_pokemon(informacion_pokemon_t *ip, void (*f)(pokemon_t *, void *),
		     void *aux)
{
	if (ip == NULL || aux == NULL || f == NULL) {
		return 0;
	}
	ordenar_pokes(ip);
	int contador = 0;
	for (int i = 0; i < ip->cantidad; i++) {
		f(ip->pokemones[i], aux);
		contador++;
	}

	return contador;
}

int con_cada_ataque(pokemon_t *pokemon,
		    void (*f)(const struct ataque *, void *), void *aux)
{
	if (pokemon == NULL || f == NULL) {
		return 0;
	}
	int contador = 0;
	for (int i = 0; i < 3; i++) {
		f(&(pokemon->ataques[i]), aux);
		contador++;
	}

	return contador;
}

void pokemon_destruir_todo(informacion_pokemon_t *ip)
{
	if (ip != NULL) {
		if (ip->cantidad != 0) {
			for (int i = 0; i < (ip->cantidad); i++) {
				free((ip->pokemones[i]));
			}
			free(ip->pokemones);
		}
		free(ip);
	}
}