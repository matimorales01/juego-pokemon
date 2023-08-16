#include "entrenador.h"
#include "lista.h"
#include "util.h"
#include "salon.h"
#include "hash.h"
#include "abb.h"
#include "pokemon.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

#define EXITO 0
#define ERROR -1

typedef struct comando{
    const char* nombre;
    char* (*ejecutar)(char** , salon_t*);

}comando_t;

typedef struct regla{
    const char* nombre;
    const char* descripcion;
    int (*ejecutar)(pokemon_t*, pokemon_t*);
}regla_t;

struct _salon_t{
    abb_t* entrenadores;
    hash_t* comandos;
    hash_t* reglas;
};


//-----------------------------------------------------------------------------REGLAS--------------------------------------------------------------------------------------------------//

regla_t* crear_regla(const char *nombre, const char *descripcion, int (*ejecutar)(pokemon_t* pokemon_uno, pokemon_t* pokemon_dos)){
    regla_t* regla = malloc(sizeof(regla_t));
    if (!regla){
        return NULL;
    }
    regla->nombre = nombre;
    regla->descripcion = descripcion;
    regla->ejecutar = ejecutar; 
    return regla;
    
}

void insertar_regla(hash_t* hash_reglas, const char *nombre, const char *descripcion, int (*ejecutar)(pokemon_t* pokemon_uno, pokemon_t* pokemon_dos)){
    regla_t* regla_nueva = crear_regla(nombre, descripcion, ejecutar);
    if (!regla_nueva){
        return;
    }
    hash_insertar(hash_reglas, regla_nueva->nombre, regla_nueva);
}

void destruir_regla(void* elemento){
    if (elemento){
        free(elemento);
    }
}


int ganador_batalla(double atributos_pokemon_uno, double atributos_pokemon_dos){
    int ganador  ;
    if (!atributos_pokemon_uno || !atributos_pokemon_dos){
        ganador = 0 ;
    }
    if (atributos_pokemon_uno > atributos_pokemon_dos){
        ganador = 1;
    }
    else{
        ganador = 2;
    }

    return ganador;

}

int comparar_moderno(pokemon_t* pokemon_uno, pokemon_t* pokemon_dos){
    if (!pokemon_uno || !pokemon_dos){
        return 0;
    }
    double total_uno = 0.5 *devolver_nivel_pokemon(pokemon_uno) + 0.9 * devolver_defensa_pokemon(pokemon_uno)  + 3 * devolver_inteligencia_pokemon(pokemon_uno);
    double total_dos = 0.5 * devolver_nivel_pokemon(pokemon_dos) + 0.9 * devolver_defensa_pokemon(pokemon_dos) + 3 * devolver_inteligencia_pokemon(pokemon_dos);
   
    return ganador_batalla(total_uno, total_dos);
    
}

int comparar_clasico(pokemon_t* pokemon_uno, pokemon_t* pokemon_dos){
    if (!pokemon_uno || !pokemon_dos){
        return 0;
    }
    double total_uno = 0.8 *devolver_nivel_pokemon(pokemon_uno) + devolver_fuerza_pokemon(pokemon_uno) + 2 * devolver_velocidad_pokemon(pokemon_uno);
    double total_dos = 0.8 * devolver_nivel_pokemon(pokemon_dos) + devolver_fuerza_pokemon(pokemon_dos) + 2 * devolver_velocidad_pokemon(pokemon_dos);
   
    return ganador_batalla(total_uno, total_dos);
    
}

int comparar_antiguo(pokemon_t* pokemon_uno, pokemon_t* pokemon_dos){
    if (!pokemon_uno || !pokemon_dos){
        return 0;
    }
    int total_uno = devolver_fuerza_pokemon(pokemon_uno) + devolver_velocidad_pokemon(pokemon_uno) + devolver_inteligencia_pokemon(pokemon_uno) + devolver_defensa_pokemon(pokemon_uno);
    int total_dos = devolver_fuerza_pokemon(pokemon_dos) + devolver_velocidad_pokemon(pokemon_dos) + devolver_inteligencia_pokemon(pokemon_dos) + devolver_defensa_pokemon(pokemon_dos);

    return ganador_batalla(total_uno,total_dos);
}

int comparar_carrera(pokemon_t* pokemon_uno, pokemon_t* pokemon_dos){
    if (!pokemon_uno || !pokemon_dos){
        return 0;
    }
    int total_uno = devolver_velocidad_pokemon(pokemon_uno);
    int total_dos = devolver_velocidad_pokemon(pokemon_dos);

    return ganador_batalla(total_uno, total_dos);
    
}

int comparar_lucha(pokemon_t* pokemon_uno, pokemon_t* pokemon_dos){
    if (!pokemon_uno || !pokemon_dos){
        return 0;
    }
    int total_uno = devolver_fuerza_pokemon(pokemon_uno);
    int total_dos = devolver_fuerza_pokemon(pokemon_dos);

    return ganador_batalla(total_uno, total_dos);
}

hash_t* crear_reglas(){
    hash_t* hash_reglas = hash_crear(destruir_regla,10);
    if (!hash_reglas){
        return NULL;
    }

    insertar_regla(hash_reglas,"MODERNO","0.5 * nivel + 0.9 * defensa + 3 * inteligencia", comparar_moderno);
    insertar_regla(hash_reglas,"CLASICO","0.8 * nivel + fuerza + 2 * velocidad", comparar_clasico);
    insertar_regla(hash_reglas,"ANTIGUO"," fuerza + velocidad + inteligencia + defensa",comparar_antiguo);
    insertar_regla(hash_reglas,"CARRERA","Gana el pokemon con mayor velocidad",comparar_carrera);
    insertar_regla(hash_reglas,"LUCHA","Gana el pokemon con mas fuerza", comparar_lucha);

    return hash_reglas;


}

bool imprimir_reglas(hash_t *hash, const char *clave, void *aux){
    if (!hash || !clave || !aux) {
        return false;
    }
    regla_t* regla = (regla_t *)hash_obtener(hash, clave);
    size_t longitud = strlen(*(char **)aux) + strlen(clave) + strlen(regla->descripcion) + 2;
    char texto_auxiliar[longitud];
    sprintf(texto_auxiliar,"%s%s,%s\n", *(char **)aux, clave, regla->descripcion);
    strcpy(*(char **)aux, texto_auxiliar);
    return false;
    
}

bool enfrentamiento_entrenadores(salon_t *salon, entrenador_t *entrenador_1, entrenador_t *entrenador_2, char *regla, void *resultado){
    

    lista_iterador_t *iterador_equipo_1 = lista_iterador_crear(entrenador_lista(entrenador_1));
    lista_iterador_t *iterador_equipo_2 = lista_iterador_crear(entrenador_lista(entrenador_2));
    int resultado_enfrentamiento = 0;
    regla_t* regla_ejecutar =  hash_obtener(salon->reglas, regla);
    if (!regla_ejecutar){
        lista_iterador_destruir(iterador_equipo_1);
        lista_iterador_destruir(iterador_equipo_2);
        return false;
    }
    while (lista_iterador_tiene_siguiente(iterador_equipo_1) && lista_iterador_tiene_siguiente(iterador_equipo_2)) {
        resultado_enfrentamiento = regla_ejecutar->ejecutar(lista_iterador_elemento_actual(iterador_equipo_1),lista_iterador_elemento_actual(iterador_equipo_2));
        if (resultado_enfrentamiento == 0){
            lista_iterador_destruir(iterador_equipo_1);
            lista_iterador_destruir(iterador_equipo_2);
            return false;
        }
        if (resultado_enfrentamiento == 1) {
            lista_iterador_avanzar(iterador_equipo_2);
        } else {
            lista_iterador_avanzar(iterador_equipo_1);
        }
        size_t longitud = strlen(*(char **)resultado) + 3; 
        char resultado_enfrentamiento_char[longitud];
        sprintf(resultado_enfrentamiento_char, "%s%i\n", *(char **)resultado, resultado_enfrentamiento);
        strcpy(*(char **)resultado, resultado_enfrentamiento_char);
    }
    lista_iterador_destruir(iterador_equipo_1);
    lista_iterador_destruir(iterador_equipo_2);
    return true;
}


//----------------------------------------------------------------------------COMANDOS-----------------------------------------------------------------------------------------------//


char* comando_entrenadores( char** campos, salon_t* salon){
  
    if (!campos || !salon){
        return false;
    }

    char *string_resultado = calloc(1, sizeof(char) * 2048);
    if (!string_resultado){
        return NULL;
    }
    lista_t *extra = lista_crear(NULL);
    if (!extra){
        free(string_resultado);
        return NULL;
    }
    lista_t *entrenadores_filtrados = NULL;
    size_t cantidad_campos = vtrlen(campos);
    if (cantidad_campos == 1) {
        entrenadores_filtrados = salon_filtrar_entrenadores(salon, siempre_true, &extra);
        lista_con_cada_elemento(entrenadores_filtrados, imprimir_entrenadores_en_pantalla_lista, &string_resultado);
    }
     else if (cantidad_campos == 2) {
        char **campos_dos = split(campos[1], ',');
        if (!campos_dos) {
            free(string_resultado);
            lista_destruir(extra);
            return NULL;
        }
        if (vtrlen(campos_dos) != 2 ) {
            free(string_resultado);
            lista_destruir(extra);
            vtrfree(campos_dos);
            return NULL;
        }

        if (strcmp(campos_dos[0], "victorias") == 0) {
            int cant_victorias = atoi(campos_dos[1]);
            lista_encolar(extra, &cant_victorias);
            entrenadores_filtrados = salon_filtrar_entrenadores(salon, entrenadores_por_victoria, &extra);
            lista_con_cada_elemento(entrenadores_filtrados, imprimir_entrenadores_en_pantalla_lista_solo_nombre, &string_resultado);
        } 
        else if (strcmp(campos_dos[0], "pokemon") == 0) {
            size_t cant_pokemones = 0;
            lista_encolar(extra, &campos_dos[1]);
            lista_encolar(extra, &cant_pokemones);
            entrenadores_filtrados = salon_filtrar_entrenadores(salon, entrenadores_por_pokemon, &extra);
            lista_con_cada_elemento(entrenadores_filtrados, imprimir_entrenadores_en_pantalla_lista_solo_nombre, &string_resultado);
        }
        else {
            free(string_resultado);
            lista_destruir(extra);
            lista_destruir(entrenadores_filtrados);
            vtrfree(campos_dos);
            return NULL;

        }
        vtrfree(campos_dos);
    }
    
    lista_destruir(extra);
    lista_destruir(entrenadores_filtrados);
    return string_resultado;
}


char* comando_equipo( char** campos, salon_t* salon){
    if (!campos || !salon){
        return NULL;
    }
    char *resultado = calloc(1, sizeof(char) * 2048);
    if (!resultado){
        return NULL;
    }
    size_t cantidad_campos = vtrlen(campos);
    if (cantidad_campos != 2){
        free(resultado);
        return NULL;
    }
    char separador = ',';
    char** campos_uno = split(campos[1], separador);
    if (!campos_uno){
        free(resultado);
        return NULL;
    }
    if (vtrlen(campos_uno) != 1){
        free(resultado);
        vtrfree(campos_uno);
        return NULL;
    }
    char* nombre_entrenador = campos_uno[0];
    entrenador_t* entrenador_buscado = arbol_buscar(salon->entrenadores, nombre_entrenador);
    if (!entrenador_buscado){
        free(resultado);
        vtrfree(campos_uno);
        return NULL;
    }

    lista_con_cada_elemento(entrenador_lista(entrenador_buscado), hacer_string_pokemon, &resultado);
    vtrfree(campos_uno);
    return resultado;
}


char* comando_reglas(char** campos, salon_t* salon){
    if ( !campos || !salon)
        return NULL;
   
    size_t cantidad_campos = vtrlen(campos);
    if (cantidad_campos != 1) {
        return NULL;
    }
    char *resultado = calloc(1, sizeof(char) * 2048);
    if (!resultado){
        return NULL;
    } 
    
    hash_con_cada_clave(salon->reglas,imprimir_reglas,&resultado);
    return resultado;
}


char* comando_comparar( char** campos, salon_t* salon){
    if ( !campos || !salon){
        return NULL;
    }
    size_t cantidad_campos = vtrlen(campos);
    if (!cantidad_campos || cantidad_campos != 2){
        return NULL;
    }

   char *resultado = calloc(1, sizeof(char) * 2048);
    if (!resultado){
        return NULL;
    }
    char **campos_dos = split(campos[1], ',');
    if (!campos_dos) {
        free(resultado);
        return NULL;
    }
    if (vtrlen(campos_dos) != 3) {
        free(resultado);
        vtrfree(campos_dos);
        return NULL;
    }
    char* nombre_uno = campos_dos[0];
    entrenador_t *entrenador_buscado_1 = arbol_buscar(salon->entrenadores, nombre_uno);
    if (!entrenador_buscado_1) {
        free(resultado);
        vtrfree(campos_dos);
        return NULL;
    }
    char* nombre_dos = campos_dos[1];
    entrenador_t *entrenador_buscado_2 = arbol_buscar(salon->entrenadores, nombre_dos);
    if (!entrenador_buscado_2) {
        free(resultado);
        vtrfree(campos_dos);
        return NULL;
    }
    char* regla = campos_dos[2];
    bool exito = enfrentamiento_entrenadores(salon, entrenador_buscado_1, entrenador_buscado_2, regla, &resultado);

    if (!exito){
        free(resultado);
        vtrfree(campos_dos);
        return NULL;
    }

    vtrfree(campos_dos);
    return resultado;
}

char* comando_agregar_pokemon(char** campos, salon_t* salon){
  if ( !campos || !salon ){
        return NULL;
    }

    char *resultado = calloc(1, sizeof(char) * 2048);
    if (!resultado){
        return NULL;
    }
    size_t cantidad_campos = vtrlen(campos);
    if (cantidad_campos != 2){
        free(resultado);
        return NULL;
    }
    char** campos_dos = split(campos[1], ',');
    if (!campos_dos || vtrlen(campos_dos) != 7){
        free(resultado);
        vtrfree(campos_dos);
        return NULL;
    }
    char* nombre_entrenador = campos_dos[0];
    entrenador_t *aux = arbol_buscar(salon->entrenadores,nombre_entrenador);
    if (!aux){
        free(resultado);
        vtrfree(campos_dos);
        return NULL;
    }
    size_t cantidad_pokemones = lista_elementos(entrenador_lista(aux));
    char* campos_pokemon[6];
    for (size_t i = 0; i < 6; i++)
    {
        campos_pokemon[i] = campos_dos[i+1];
    }
    pokemon_t* pokemon = crear_pokemon(campos_pokemon);
    if (!pokemon){
        free(resultado);
        destruir_entrenador(aux);
        
        vtrfree(campos_dos);
        return NULL;
    }
    aux = agregar_pokemon(aux, pokemon);
    if (cantidad_pokemones < lista_elementos(entrenador_lista(aux))){
        strcpy(resultado, "OK");
    }

   
    vtrfree(campos_dos);
    return resultado;

}

char*  comando_quitar_pokemon( char** campos, salon_t* salon){
     if (!campos || !salon ){
        return NULL;
    }
    size_t cantidad_campos = vtrlen(campos);
    if (cantidad_campos != 2){
        return NULL;
    }
    char** campos_dos = split(campos[1], ',');
    if (!campos_dos ){
        return NULL;
    }
    if (vtrlen(campos_dos) != 2){
        vtrfree(campos_dos);
        return NULL;
    }
    char* nombre_entrenador = campos_dos[0];
    
    entrenador_t *entrenador = arbol_buscar(salon->entrenadores,nombre_entrenador);
    if (lista_elementos(entrenador_lista(entrenador)) <= 1){
 
        vtrfree(campos_dos);
        return NULL;
    }
    char* nombre_pokemon = campos_dos[1];
    size_t posicion =  buscar_posicion_pokemon(entrenador_lista(entrenador),nombre_pokemon);
    if (posicion == 200 || posicion > lista_elementos(entrenador_lista(entrenador))){
        vtrfree(campos_dos);
        return NULL;
    }

    int borrado = lista_borrar_de_posicion(entrenador_lista(entrenador), posicion);
    char *resultado = calloc(1, sizeof(char) * 2048);
    if (!resultado){
        vtrfree(campos_dos);
        return NULL;
    }
    if (borrado != -1){
        strcpy(resultado, "OK");
        
    }
    vtrfree(campos_dos);
    return resultado;
    
   
}
char* comando_guardar( char** campos, salon_t* salon){
    if (!campos || !salon){
        return NULL;
    }
    char *resultado = calloc(1, sizeof(char) * 2048);
    if (!resultado){
        return NULL;
    }
    size_t cantidad_campos = vtrlen(campos);

    if (!cantidad_campos || cantidad_campos != 2){
        return NULL;
    }

    char* nombre_archivo = campos[1];

    int guardado = salon_guardar_archivo(salon,nombre_archivo);

    if (guardado != -1){
        strcpy(resultado, "OK");
    }
    

    return resultado;

}

//-----------------------------------------------------------------------CREACION_COMANDOS--------------------------------------------------------------------------------------//
comando_t* comando_crear(const char* nombre, char* (*ejecutar)( char**, salon_t *)){
    comando_t* comando = malloc(sizeof(comando_t));
    if (!comando){
        return NULL;
    }
    comando->nombre = nombre;
    comando->ejecutar = ejecutar;
    return comando;
}

void agregar_comando(hash_t* hash,const char* nombre, char* (*ejecutar)( char**, salon_t *) ){
    if (!hash){
        return;
    }
    comando_t* comando =  comando_crear(nombre,ejecutar);
    if (!comando){
        return;
    }
    hash_insertar(hash,nombre,comando);
}

hash_t* crear_comandos(){
    hash_t* comandos = hash_crear(free,15);
    if (!comandos){
        return NULL;
    }
    agregar_comando(comandos,"ENTRENADORES", comando_entrenadores);
    agregar_comando(comandos,"EQUIPO", comando_equipo);
    agregar_comando(comandos,"REGLAS", comando_reglas);
    agregar_comando(comandos,"COMPARAR", comando_comparar);
    agregar_comando(comandos,"AGREGAR_POKEMON", comando_agregar_pokemon);
    agregar_comando(comandos,"QUITAR_POKEMON", comando_quitar_pokemon);
    agregar_comando(comandos,"GUARDAR", comando_guardar);
    return comandos;

}
//----------------------------------------------------------------------------AUXILIARES_SALON--------------------------------------------------------------------------------------------------//

salon_t* salon_crear(){
    salon_t* salon = malloc(sizeof(salon_t));
    if (!salon){
        return NULL;
    }
    salon->entrenadores = arbol_crear(comparar_entrenadores, destruir_entrenador);
    if (!salon->entrenadores){
        salon_destruir(salon);
        return NULL;
    }
    salon->comandos = crear_comandos();
    if (!salon->comandos){
        salon_destruir(salon);
        return NULL;
    }
    salon->reglas = crear_reglas();
    if (!salon->reglas){
        salon_destruir(salon);
        return NULL;
    }
    return salon;
}

bool tiene_pokemon(void* _entrenador, void* extra){
    entrenador_t* entrenador = _entrenador;

    if (lista_vacia(entrenador_lista(entrenador))){
        *(bool*)extra = false;
        return true;
    }
    return false;
}

salon_t* leer_salon(FILE* archivo){
    if (!archivo){
        return NULL;
    }

    char separador = ';';
    char* linea = NULL;
    salon_t* salon = salon_crear();
    if (!salon){
        return NULL;
    }
    entrenador_t * entrenador = NULL;
    pokemon_t * pokemon = NULL;

    linea = fgets_alloc(archivo);
    while (linea){
        char** campos = split(linea, separador);
        size_t cantidad_campos = vtrlen(campos);
        if (cantidad_campos == 2){
            entrenador = crear_entrenador(campos[0], campos[1]);
            
            entrenador_t* repetido = buscar_entrenador(salon->entrenadores, campos[0]);
            if (repetido != NULL){
                salon_destruir(salon);
                free(linea);
                vtrfree(campos);
                destruir_entrenador(entrenador);
                fclose(archivo);
                return NULL;
            }
            if (!entrenador){
                salon_destruir(salon);
                free(linea);
                vtrfree(campos);
                fclose(archivo);
                return NULL;
            }
            salon = salon_agregar_entrenador(salon, entrenador);
            
        }
        else{
            pokemon = crear_pokemon(campos);
            if (!pokemon){
                salon_destruir(salon);
                free(linea);
                vtrfree(campos);
                fclose(archivo);
                return NULL;
            
            }
            entrenador = agregar_pokemon(entrenador, pokemon);
        }
        vtrfree(campos);
        free(linea);
        linea = fgets_alloc(archivo);
    }
    free(linea);
    fclosen(archivo);
    bool todos_tienen_pokemon = true ;
    abb_con_cada_elemento(salon->entrenadores, ABB_RECORRER_INORDEN, tiene_pokemon, &todos_tienen_pokemon);
    if (todos_tienen_pokemon == false){
        salon_destruir(salon);
        return NULL;
    }

    return salon;
}

bool insertar_para_filtrar(void *entrenador, void *extra){

    if (entrenador) {
        lista_insertar((lista_t *)extra, (entrenador_t *)entrenador);
    }
    return false;
}
 

 //--------------------------------------------------------------------------------------------SALON------------------------------------------------------------------------------------------------//



salon_t* salon_leer_archivo(const char* nombre_archivo){
    if (!nombre_archivo){
        return NULL;
    }
    FILE * archivo = fopen(nombre_archivo, "r");
    if (!archivo){
        return NULL;
    }

    salon_t* salon = leer_salon(archivo);
    if (!salon){
        return NULL;
    }
    return salon;
}

int salon_guardar_archivo(salon_t *salon, const char *nombre_archivo){
    if (!salon || !nombre_archivo) {
        return ERROR;
    }
    FILE *archivo_nuevo = fopen(nombre_archivo, "w");
    if (archivo_nuevo == NULL) {
        fclose(archivo_nuevo);
        return ERROR;
    }

    int guardados = (int)abb_con_cada_elemento(salon->entrenadores, ABB_RECORRER_INORDEN, imprimir_entrenadores, &archivo_nuevo);
    if (guardados == -1){
        fclosen(archivo_nuevo);
        return ERROR;
    }
    fclosen(archivo_nuevo);
    return guardados;
}

salon_t* salon_agregar_entrenador(salon_t* salon, entrenador_t* entrenador){
    if (!salon || !entrenador){
        return NULL;
    }
    arbol_insertar(salon->entrenadores, entrenador);
    return salon;
}


lista_t *salon_filtrar_entrenadores(salon_t *salon, bool (*f)(entrenador_t *, void *), void *extra){
    if (!salon || !f){
        return NULL;
    }
    lista_t* lista = lista_crear(NULL);
    if (!lista){
        return NULL;
    }

    abb_con_cada_elemento(salon->entrenadores, ABB_RECORRER_INORDEN, insertar_para_filtrar, lista);

    lista_iterador_t* iterador = lista_iterador_crear(lista);
    lista_t *lista_final = lista_crear(NULL);
    while (lista_iterador_tiene_siguiente(iterador)) {
        if (f(lista_iterador_elemento_actual(iterador), extra)) {
            lista_insertar(lista_final, lista_iterador_elemento_actual(iterador));
        }
        lista_iterador_avanzar(iterador);
    }
    lista_iterador_destruir(iterador);
    lista_destruir(lista);
    return lista_final;
}


char *salon_ejecutar_comando(salon_t *salon, const char *comando){
    if (!salon || !comando)
        return NULL;

    char separador = ':' ; 
    char** campos = split(comando,separador);
    if (!campos){
        return NULL;
    }
    comando_t* comando_ejecutar = hash_obtener(salon->comandos, campos[0]);
    if(!comando_ejecutar){
        vtrfree(campos);
        return NULL;
    }

    char* resultado = comando_ejecutar->ejecutar(campos,salon);
    if (!resultado){
        vtrfree(campos);
        return NULL;
    }

    vtrfree(campos);
    return resultado;
}

void salon_destruir(salon_t* salon){
    if (!salon){
        return;
    }
    arbol_destruir(salon->entrenadores);
    hash_destruir(salon->comandos);
    hash_destruir(salon->reglas);

    free(salon);
}