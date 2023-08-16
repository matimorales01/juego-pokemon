#include "entrenador.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <stdbool.h>

#define MAX_NOMBRE_ENTRENADOR 30

struct _entrenador_t{
    char nombre[MAX_NOMBRE_ENTRENADOR];
    int victorias;
    lista_t* equipo;
};

entrenador_t* crear_entrenador(char* nombre, char* victorias){
    entrenador_t* entrenador = malloc(sizeof(entrenador_t));
    if (!entrenador){
        return NULL;
    }
    
    strcpy(entrenador->nombre, nombre);
    entrenador->victorias = atoi(victorias);
    entrenador->equipo = lista_crear(destruir_pokemon);
    return entrenador;
}

int comparar_entrenadores(void* e1, void* e2){
    entrenador_t* _e1 = e1;
    entrenador_t* _e2 = e2;
    char* nombre_e1 = _e1->nombre;
    char* nombre_e2 = _e2->nombre;
    return strcmp(nombre_e1, nombre_e2);
}

int entrenador_victorias(entrenador_t* entrenador){
    return entrenador->victorias;
}

char* entrenador_nombre(entrenador_t* entrenador){
    return entrenador->nombre;
}


lista_t* entrenador_lista(entrenador_t* entrenador){
    return entrenador->equipo;
}

entrenador_t* buscar_entrenador(abb_t* arbol, char*nombre){
    entrenador_t* _entrenador = crear_entrenador(nombre, "0");
    if (!_entrenador){
        return NULL;
    }
    entrenador_t* entrenador = arbol_buscar(arbol, _entrenador);
    destruir_entrenador(_entrenador);
    if (!entrenador){
        return NULL;
    }
    return entrenador;
}


entrenador_t* agregar_pokemon(entrenador_t* entrenador, pokemon_t* pokemon){
    lista_insertar(entrenador->equipo, pokemon);
    return entrenador;
}


bool imprimir_entrenadores_en_pantalla_lista_solo_nombre(void *entrenador, void *buffer){
    if (!entrenador){
        return false;
    }
    
    size_t longitud = strlen(*(char **)buffer) + strlen(entrenador_nombre((entrenador_t *)entrenador)) + 2;
    char texto_auxiliar[longitud];
    sprintf(texto_auxiliar,"%s%s\n", *(char **)buffer, entrenador_nombre((entrenador_t *)entrenador));
    strcpy(*(char **)buffer, texto_auxiliar);
    
    return true;
}


bool imprimir_entrenadores_en_pantalla_lista(void *entrenador, void *buffer){
    if (!entrenador){
        return false;
    }
      
    char victorias[10];
    sprintf(victorias, "%i", entrenador_victorias((entrenador_t *)entrenador));
    size_t longitud = strlen(*(char **)buffer) + strlen(entrenador_nombre((entrenador_t *)entrenador)) + strlen(victorias) + 2; 
    char texto_auxiliar[longitud];
    sprintf(texto_auxiliar, "%s%s,%i\n", *(char **)buffer, entrenador_nombre((entrenador_t *)entrenador), entrenador_victorias((entrenador_t *)entrenador));
    strcpy(*(char **)buffer, texto_auxiliar);
    
    return true;
}

bool siempre_true(entrenador_t *entrenador, void *extra){
    return true;
}

bool verificar_pokemones(void *pokemon, void *extra){
    if (pokemon) {
        char *nombre_pokemon = *(char **)lista_elemento_en_posicion(*(lista_t **)extra, 0);
        size_t *cantidad_pokemones = (size_t *)lista_elemento_en_posicion(*(lista_t **)extra, 1);

        if (strcasecmp(nombre_pokemon,devolver_nombre_pokemon((pokemon_t *)pokemon)) == 0) {
            (*cantidad_pokemones)++;
            return false;
        }
    }
    return true;
}

bool entrenadores_por_pokemon(entrenador_t *entrenador, void *extra){
    lista_t *equipo = entrenador_lista(entrenador);
    size_t *cantidad_pokemones = (size_t *)lista_elemento_en_posicion(*(lista_t **)extra, 1);
    *cantidad_pokemones = 0;
    lista_con_cada_elemento(equipo, verificar_pokemones, extra);
    return (*cantidad_pokemones > 0);
}


bool entrenadores_por_victoria(entrenador_t *entrenador, void *extra){
    int cant_victorias = entrenador_victorias(entrenador);

    int *cant_victorias_requeridas = (int *)lista_elemento_en_posicion(*(lista_t **)extra, 0);
    return (cant_victorias >= *cant_victorias_requeridas);
}





bool hacer_string_pokemon(void *pokemon, void *buffer)
{
    if (pokemon) {
        char niv[20];
        sprintf(niv, "%i", devolver_nivel_pokemon((pokemon_t *)pokemon));
        char def[20];
        sprintf(def, "%i", devolver_defensa_pokemon((pokemon_t *)pokemon));
        char fue[20];
        sprintf(fue, "%i", devolver_fuerza_pokemon((pokemon_t *)pokemon));
        char intel[20];
        sprintf(intel, "%i", devolver_inteligencia_pokemon((pokemon_t *)pokemon));
        char vel[20];
        sprintf(vel, "%i", devolver_velocidad_pokemon((pokemon_t *)pokemon));
        size_t longitud = strlen(*(char **)buffer) + strlen(devolver_nombre_pokemon((pokemon_t *)pokemon)) + strlen(niv) + strlen(def) + strlen(fue) + strlen(intel) + strlen(vel) + 2; 
        char texto_auxiliar[longitud];
        sprintf(texto_auxiliar, "%s%s,%i,%i,%i,%i,%i\n", *(char **)buffer, devolver_nombre_pokemon((pokemon_t *)pokemon), devolver_nivel_pokemon((pokemon_t *)pokemon), devolver_defensa_pokemon((pokemon_t *)pokemon), devolver_fuerza_pokemon((pokemon_t *)pokemon), devolver_inteligencia_pokemon((pokemon_t *)pokemon), devolver_velocidad_pokemon((pokemon_t *)pokemon));
       
        strcpy(*(char **)buffer, texto_auxiliar);
    }
    return true;
}
size_t buscar_posicion_pokemon(lista_t* equipo, char* nombre_pokemon){
    size_t posicion = 100 ;
    size_t cantidad_pokemones = lista_elementos(equipo);
    for (size_t i = 0; i < cantidad_pokemones; i++)
    {
        pokemon_t* pokemon = lista_elemento_en_posicion(equipo, i);
        if (strcmp(devolver_nombre_pokemon(pokemon), nombre_pokemon) == 0){
            return  i;
        }
    }
    return posicion;
    
}

bool imprimir_pokemones(void *pokemon, void *archivo){
    if (pokemon || archivo) {
        fprintf(*(FILE **)archivo, "%s;%i;%i;%i;%i;%i\n", devolver_nombre_pokemon((pokemon_t *)pokemon), devolver_nivel_pokemon((pokemon_t *)pokemon), devolver_defensa_pokemon((pokemon_t *)pokemon), devolver_fuerza_pokemon((pokemon_t *)pokemon), devolver_inteligencia_pokemon((pokemon_t *)pokemon), devolver_velocidad_pokemon((pokemon_t *)pokemon));
    }
    return false;
}


bool imprimir_entrenadores(void *entrenador, void *archivo){
    if (entrenador || archivo) {
        if (!lista_vacia(entrenador_lista((entrenador_t*)entrenador))){
            fprintf(*(FILE **)archivo, "%s;%i\n", entrenador_nombre((entrenador_t *)entrenador), entrenador_victorias((entrenador_t *)entrenador));
            lista_con_cada_elemento((lista_t *)entrenador_lista((entrenador_t *)entrenador), imprimir_pokemones, archivo);
        }
        
    }
    return false;
}

void destruir_pokemon(void* pokemon){
    free(pokemon);
}

void destruir_entrenador(void* entrenador){
    entrenador_t* entrenador_aux = entrenador;
    if (entrenador_aux->equipo != NULL){
        lista_destruir(entrenador_aux->equipo);
    }
    free(entrenador_aux);
    entrenador = NULL;
}