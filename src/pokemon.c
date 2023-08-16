#include "pokemon.h"
#include "util.h"
#include <string.h>
#include <stdlib.h>


#define MAX_NOMBRE_POKEMON    10

struct _pokemon_t{
    char nombre[MAX_NOMBRE_POKEMON];
    int nivel;
    int fuerza;
    int inteligencia;
    int velocidad;
    int defensa;
    
};

pokemon_t* crear_pokemon(char**campos){

    pokemon_t*pokemon = malloc(sizeof(pokemon_t));
    if (!pokemon){
        return NULL;
    }
    strcpy(pokemon->nombre, campos[0]);
    pokemon->nivel = atoi(campos[1]);
    pokemon->fuerza = atoi(campos[3]);
    pokemon->inteligencia = atoi(campos[4]);
    pokemon->velocidad = atoi(campos[5]);
    pokemon->defensa = atoi(campos[2]);

    return pokemon;
}


char* devolver_nombre_pokemon(pokemon_t* pokemon){
    return pokemon->nombre;
}

int devolver_nivel_pokemon(pokemon_t* pokemon){
    return pokemon->nivel;
}

int devolver_fuerza_pokemon(pokemon_t* pokemon){
    return pokemon->fuerza;
}

int devolver_inteligencia_pokemon(pokemon_t* pokemon){
    return pokemon->inteligencia;
}

int devolver_velocidad_pokemon(pokemon_t* pokemon){
    return pokemon->velocidad;
}

int devolver_defensa_pokemon(pokemon_t* pokemon){
    return pokemon->defensa;
}


void pokemon_destruir(void* pokemon) {
    free(((pokemon_t*)pokemon)->nombre);
    free(pokemon);
}