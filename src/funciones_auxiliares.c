#include "funciones_auxiliares.h"

int comparador_elementos(void *p1, void *p2)
{
	pokemon_t *poke = p1;
	char *pokemon = p2;
	int numero = strcmp(pokemon, pokemon_nombre(poke));
	return numero;
}

void funcion(pokemon_t *poke, void *v2)
{
	lista_t *lista = v2;
	lista_insertar(lista, poke);
}