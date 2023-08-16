#include "pa2mm.h"
#include "src/salon.h"
#include <string.h>

bool filtrar_entrenadores_por_victorias(entrenador_t *entrenador, void *cant_victorias)
{
    if (!entrenador)
        return false;

    return entrenador_victorias(entrenador) > *(int *)cant_victorias;
}

void PruebasCrearSalon(){
    salon_t *salon = NULL;
    pa2m_afirmar(!salon, "El salon es NULL antes de leer el archivo");
    salon = salon_leer_archivo(NULL);
    pa2m_afirmar(!salon, "Luego de leer un archivo NULL, el salon sigue siendo NULL");
    salon_t* salon_dos = salon_leer_archivo("salones/salon_estandar.txt");
    pa2m_afirmar(salon_dos != NULL, "Puedo crear un salon leyendo un archivo no nulO");
    salon_destruir(salon);
    salon_destruir(salon_dos);
}



void PruebasFiltrarEntrenadores(){
    salon_t *salon = NULL;
    int cant_victorias = 0;
    pa2m_afirmar(!salon_filtrar_entrenadores(salon, filtrar_entrenadores_por_victorias, &cant_victorias), "No puedo filtrar entrenadores de un salon NULL");
    salon = salon_leer_archivo("salones/salon_estandar.txt");
    pa2m_afirmar(!salon_filtrar_entrenadores(salon,NULL,&cant_victorias), "No puedo filtrar entrenadores si me pasan una funcion NULL");
    lista_t *lista_entrenadores_filtrados = salon_filtrar_entrenadores(salon, filtrar_entrenadores_por_victorias, &cant_victorias);
    pa2m_afirmar(!lista_vacia(lista_entrenadores_filtrados), "Filtrar entrenadores con más de 0 victorias devuelve una lista no vacia.");
    pa2m_afirmar(lista_elementos(lista_entrenadores_filtrados) == 5, "Filtrar entrenadores con más de 0 victorias devuelve 5 entrenadores.");
    lista_destruir(lista_entrenadores_filtrados);
    salon_destruir(salon);

}

void PruebasComandoEntrenadores(){
    salon_t *salon = NULL;
    char *resultado = salon_ejecutar_comando(salon, "ENTRENADORES");
    pa2m_afirmar(!resultado, "Si el salon es NULL, el comando 'ENTRENADORES' devuelve NULL");
    if (resultado){
        free(resultado);
    }
    salon_destruir(salon);
    salon = salon_leer_archivo("salones/salon_estandar.txt");
    resultado = salon_ejecutar_comando(salon, "ENTRENADORES");
    pa2m_afirmar(strcmp(resultado, "Dani,47\nLucas,33\nMariano,68\nPucci,52\nStephi,49\n") == 0, "Devuelve correctamente el string \'Dani,47\\nLucas,33\\nMariano,68\\nPucci,52\\nStephi,49\\n\' (ordenado alfabéticamente) al ejecutar el comando ENTRENADORES en un salon");

    if (resultado){
        free(resultado);
    }
    char *resultado_dos = salon_ejecutar_comando(salon, "ENTRENADORES:victorias,10");

    pa2m_afirmar(strcmp(resultado_dos, "Dani\nLucas\nMariano\nPucci\nStephi\n") == 0, "Devuelve correctamente el string \'Dani\\nLucas\\nMariano\\nPucci\\nStephi\\n\' (ordenado alfabéticamente) al ejecutar el comando ENTRENADORES:victorias,10 en un salon");
    if (resultado_dos){
        free(resultado_dos);
    }
    char *resultado_tres = salon_ejecutar_comando(salon, "ENTRENADORES:pokemon,pikachu");
    pa2m_afirmar(strcmp(resultado_tres, "Dani\nLucas\n") == 0, "Devuelve correctamente el string \'Dani\\nLucas\\n\' (ordenado alfabéticamente) al ejecutar el comando ENTRENADORES:pokemon,pikachu en un salon");

    if (resultado_tres)
        free(resultado_tres);
    salon_destruir(salon);
}

void PruebasComandoSalon(){
     salon_t *salon = NULL;
    char *resultado = salon_ejecutar_comando(salon, "EQUIPO:Dani");
    pa2m_afirmar(!resultado, "Devuelve NULL al ejecutar el comando EQUIPO:Dani en un salon NULL");

    if (resultado)
        free(resultado);
    
    salon = salon_leer_archivo("salones/salon_estandar.txt");
    resultado = salon_ejecutar_comando(salon, "EQUIPO:Mariano");

    pa2m_afirmar(strcmp(resultado, "Lapras,46,47,18,29,40\nArticuno,21,12,60,14,15\nDragonair,71,92,71,34,25\nLapras,81,22,90,7,35\n") == 0, "Devuelve correctamente el string \'Lapras,46,47,18,29,40\\nArticuno,21,12,60,14,15\\nDragonair,71,92,71,34,25\\nLapras,81,22,90,7,35\\n\' al ejecutar el comando EQUIPO:Mariano en un salon");

    if (resultado)
        free(resultado);

    salon_destruir(salon);
}

void PruebasComandoReglas(){
    salon_t *salon = NULL;
    char *resultado = salon_ejecutar_comando(salon, "REGLAS");
    pa2m_afirmar(!resultado, "Devuelve NULL al ejecutar el comando REGLAS en un salon NULL");

    if (resultado){
        free(resultado);
    }
    salon = salon_leer_archivo("salones/salon_estandar.txt");
    resultado = salon_ejecutar_comando(salon, "REGLAS");

    pa2m_afirmar(resultado != NULL, "El comando REGLAS devuelve un string no vacio");

    printf("%s\n", resultado);
    
    if (resultado){
        free(resultado);
    }


    salon_destruir(salon);
}



void PruebasComandoAgregar(){
    salon_t *salon = NULL;
    char *resultado = salon_ejecutar_comando(salon, "AGREGAR_POKEMON:Lucas,scyther,10, 19, 20, 15");
    pa2m_afirmar(!resultado, "Devuelve NULL al ejecutar el comando AGREGAR_POKEMON:Stephi,scyther,10, 10, 10, 10 en un salon NULL");

    if (resultado){
        free(resultado);
    }
    salon = salon_leer_archivo("salones/salon_estandar.txt");

    resultado = salon_ejecutar_comando(salon, "EQUIPO:Mariano");

    pa2m_afirmar(strcmp(resultado, "Lapras,46,47,18,29,40\nArticuno,21,12,60,14,15\nDragonair,71,92,71,34,25\nLapras,81,22,90,7,35\n") == 0, "Los pokemones de mariano antes de agregar uno son Lapras,Articuno,Dragonair,Lapras");

    if (resultado){
        free(resultado);
    }
    resultado = salon_ejecutar_comando(salon, "AGREGAR_POKEMON:Mariano,Pikachu,10, 20, 30, 40, 50");

    pa2m_afirmar(strcmp(resultado, "OK") == 0, "Puedo agregar el pokemon pikachu usando el comando AGREGAR_POKEMOn y devuelve OK ");

    if (resultado){
        free(resultado);
    }

    resultado = salon_ejecutar_comando(salon, "EQUIPO:Mariano");

    pa2m_afirmar(strcmp(resultado, "Lapras,46,47,18,29,40\nArticuno,21,12,60,14,15\nDragonair,71,92,71,34,25\nLapras,81,22,90,7,35\nPikachu,10,20,30,40,50\n") == 0, "Los pokemones de mariano ahora son Lapras,Articuno,Dragonair,Lapras,Pikachu");
    printf("%s\n", resultado);
    if (resultado){
        free(resultado);
    }

    salon_destruir(salon);

}

void PruebasComandoQuitar(){
    salon_t *salon = NULL;
    salon = salon_leer_archivo("salones/salon_estandar.txt");

    char *resultado = salon_ejecutar_comando(salon, "EQUIPO:Mariano");

    pa2m_afirmar(strcmp(resultado, "Lapras,46,47,18,29,40\nArticuno,21,12,60,14,15\nDragonair,71,92,71,34,25\nLapras,81,22,90,7,35\n") == 0, "Devuelve correctamente los pokemones de Mariano antes de quitar uno");

    if (resultado){
        free(resultado);
    }
    resultado = salon_ejecutar_comando(salon, "QUITAR_POKEMON:Mariano,Dragonair");

    pa2m_afirmar(strcmp(resultado, "OK") == 0, "Devuelve correctamente el string 'OK' al ejecutar el comando QUITAR_POKEMON:Mariano,Dragonair en un salon");

    if (resultado){
        free(resultado);
    }
    resultado = salon_ejecutar_comando(salon, "EQUIPO:Mariano");

    pa2m_afirmar(strcmp(resultado, "Lapras,46,47,18,29,40\nArticuno,21,12,60,14,15\nLapras,81,22,90,7,35\n") == 0, "Devuelve correctamente los pokemones de Mariano despues de intentar quitar uno que no estaba en el equipo");

    if (resultado){
        free(resultado);
    }
    resultado = salon_ejecutar_comando(salon, "QUITAR_POKEMON:Mariano,Lapras");

    pa2m_afirmar(strcmp(resultado, "OK")== 0, "QUITAR_POKEMON:Mariano,Lapras, devuelve OK ");

    if (resultado){
        free(resultado);
    }
    resultado = salon_ejecutar_comando(salon, "EQUIPO:Mariano");

    pa2m_afirmar(strcmp(resultado, "Articuno,21,12,60,14,15\nLapras,81,22,90,7,35\n") == 0, "Devuelve correctamente los pokemones de Mariano despues de intentar quitar uno que no estaba en el equipo");

    if (resultado){
        free(resultado);
    }
    salon_destruir(salon);

}

void PruebasComandoQuitarCuandoQuedaUno(){
   return;
    
}

void PruebasComandoGuardar()
{
    salon_t *salon = NULL;

   

    char *resultado = salon_ejecutar_comando(salon, "GUARDAR:salones/guardar_comando_salon_null.txt");
    pa2m_afirmar(!resultado, "No puedo guardar un salon que es NULL");

    if (resultado){
        free(resultado);
    }
    salon = salon_leer_archivo("salones/salon_estandar.txt");
    
    resultado = salon_ejecutar_comando(salon, "AGREGAR_POKEMON:Mariano,scyther,10, 10, 10, 10, 10"); // fua el diego


    pa2m_afirmar(strcmp(resultado,"OK")==0,"Agrego un pokemon al salon y devuelve OK");
    
    if(resultado){
        free(resultado);
    }

    resultado = salon_ejecutar_comando(salon, "AGREGAR_POKEMON:Pucci,roman,10,10,10,10,10"); // fuael diego

    pa2m_afirmar(strcmp(resultado,"OK")==0,"Agrego otro pokemon al salon y devuelve OK");

    if (resultado){
        free(resultado);
    }

    resultado = salon_ejecutar_comando(salon, "QUITAR_POKEMON:Lucas,Pikachu");

    pa2m_afirmar(strcmp(resultado,"OK")==0,"Quito un pokemon y devuelve OK");    

    if (resultado){
        free(resultado);
    }
    resultado = salon_ejecutar_comando(salon, "QUITAR_POKEMON:Lucas,Nidoking");

    pa2m_afirmar(strcmp(resultado,"OK")==0,"Quito un pokemon y devuelve OK");    

    if (resultado){
        free(resultado);
    }
    resultado = salon_ejecutar_comando(salon, "QUITAR_POKEMON:Lucas,Zapdos");

    pa2m_afirmar(strcmp(resultado,"OK")==0,"Quito un pokemon y devuelve OK");    

    if (resultado){
        free(resultado);
    }
    resultado = salon_ejecutar_comando(salon, "QUITAR_POKEMON:Lucas,Eevee");

    pa2m_afirmar(strcmp(resultado,"OK")==0,"Quito un pokemon y devuelve OK");    

    if (resultado){
        free(resultado);
    }
   resultado = salon_ejecutar_comando(salon, "QUITAR_POKEMON:Lucas,Ditto");

    pa2m_afirmar(strcmp(resultado,"OK")==0,"Quito un pokemon y devuelve OK");    

    if (resultado){
        free(resultado);
    }
    resultado = salon_ejecutar_comando(salon, "QUITAR_POKEMON:Lucas,Scyther");

    pa2m_afirmar(resultado == NULL,"Quito un pokemon y devuelve NULL, ya que es el ultimo del entrenador");    

    if (resultado){
        free(resultado);
    }
    resultado = salon_ejecutar_comando(salon,"GUARDAR:guardar_comando_salon.txt");

    pa2m_afirmar(strcmp(resultado, "OK")==0 ,"Puedo ejecutar el comando GUARDAR y devuelve 'OK' ");
    if (resultado){
        free(resultado);
    }
    salon_destruir(salon);
}

int main(){

    pa2m_nuevo_grupo("Pruebas de crear salon");
    PruebasCrearSalon();
    pa2m_nuevo_grupo("Pruebas de filtrar entrenadores");
    PruebasFiltrarEntrenadores();
    pa2m_nuevo_grupo("PRUEBAS COMANDOS");
    pa2m_nuevo_grupo("Pruebas del comando ENTRENADORES");
    PruebasComandoEntrenadores();
    pa2m_nuevo_grupo("Pruebas del comando EQUIPO");
    PruebasComandoSalon();
    pa2m_nuevo_grupo("Pruebas del comando REGLAS");
    PruebasComandoReglas();
    pa2m_nuevo_grupo("Pruebas del comando AGREGAR POKEMON");
    PruebasComandoAgregar();
    pa2m_nuevo_grupo("Pruebas del comando QUITAR POKEMON");
    PruebasComandoQuitar();
    PruebasComandoQuitarCuandoQuedaUno();
    pa2m_nuevo_grupo("Pruebas del comando GUARDAR");
    PruebasComandoGuardar();
    return pa2m_mostrar_reporte();
}
