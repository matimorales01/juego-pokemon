#ifndef ENTRENADOR_H_
#define ENTRENADOR_H_
#include "abb.h"
#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include "pokemon.h"


typedef struct _entrenador_t entrenador_t;

entrenador_t* crear_entrenador(char* nombre, char* victorias);

int comparar_entrenadores(void* e1, void* e2);

int entrenador_victorias(entrenador_t* entrenador);

char* entrenador_nombre(entrenador_t* entrenador);

lista_t* entrenador_lista(entrenador_t* entrenador);

entrenador_t* buscar_entrenador(abb_t* arbol, char* nombre);

entrenador_t* agregar_pokemon(entrenador_t* entrenador, pokemon_t* pokemon);

bool imprimir_entrenadores_en_pantalla_lista_solo_nombre(void *entrenador, void *buffer);

bool imprimir_entrenadores_en_pantalla_lista(void *entrenador, void *buffer);

bool siempre_true(entrenador_t *entrenador, void *extra);

bool verificar_pokemones(void *pokemon, void *extra);

bool entrenadores_por_pokemon(entrenador_t *entrenador, void *extra);

bool entrenadores_por_victoria(entrenador_t *entrenador, void *extra);

bool hacer_string_pokemon(void* pokemon, void* buffer);

size_t buscar_posicion_pokemon(lista_t* equipo, char* nombre_pokemon);

bool imprimir_pokemones(void *pokemon, void *archivo);

bool imprimir_entrenadores(void *entrenador, void *archivo);

void destruir_pokemon(void* pokemon);

void destruir_entrenador(void* entrenador);


#endif // ENTRENADOR_H_
