#include "util.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include <stdio.h>

size_t vtrlen(void* ptr){
		void** aux = ptr;

		size_t  longitud = 0;
        
		if (ptr == NULL){
			return 0;
		}
        
		while (aux[longitud] != NULL){
			longitud++;
		}
	
	return longitud;

	}

void* vtradd(void* ptr, void* item){


    if (ptr == NULL){
        
        ptr = malloc(sizeof(void*)*2);
        if (!ptr){
            return NULL;
        }
        void** ptr_aux = ptr;
        ptr_aux[0] = item;
        ptr_aux[1] = NULL;


    }
    else{
        
        size_t tamanio = vtrlen(ptr)+2;
        ptr = realloc(ptr,sizeof(void*)*tamanio);
        if (!ptr){
            return NULL;
        }
        void** ptr_aux = ptr;
        ptr_aux[tamanio-2] = item;
        ptr_aux[tamanio-1] = NULL;

    }

    return ptr;

}


void vtrfree(void* ptr){
	void** aux = ptr;
	size_t len_ptr = vtrlen(ptr);
	for(size_t i = 0; i < len_ptr; i++){
		free(aux[i]);
	}
	free(ptr);

}


char** split(const char* str, char separador){
    
    if (!str || !(strlen(str))){
        return NULL;
    }
    size_t len = strlen(str);
    size_t ultima_letra = 0;
    size_t primera_letra = 0;
    size_t i = 0;
    char* ptr_palabra;
    char**ptr = NULL;

    while (ultima_letra < len+1){
        if (str[ultima_letra] != separador){
            ultima_letra++;
        }

        else{
            size_t len_palabra = ultima_letra - primera_letra;
            char *aux = calloc(len_palabra+1 * sizeof(char), sizeof(char));
            if (!aux){
                return NULL;
            }
            ptr_palabra = aux;
            for (size_t i = 0; i< len_palabra; i++){
                ptr_palabra[i] = str[primera_letra + i];

            }
           
            ptr = vtradd(ptr, ptr_palabra);
            ultima_letra++;
            primera_letra = ultima_letra;
        }
        
        if (ultima_letra == len && str[ultima_letra] != separador){
            ultima_letra++;
            size_t len_palabra = ultima_letra - primera_letra;
            char *aux = calloc(len_palabra+1 * sizeof(char), sizeof(char));
            if (!aux){
                return NULL;
            }
            ptr_palabra = aux;
            for (i = 0; i< len_palabra; i++){
                ptr_palabra[i] = str[primera_letra +i];

            }
          
            
            ptr = vtradd(ptr, ptr_palabra);
        }
    }
    return ptr;
 }
    
    




char* fgets_alloc(FILE* archivo){
    
    size_t tamanio = 512;
    size_t bytes_contados = 0;
    char* string = malloc(tamanio * sizeof(char));
    bool salir = false;
 
    while (fgets(string + bytes_contados, (int)tamanio - (int)bytes_contados, archivo)&& salir != true){
        
        size_t bytes_leidos = strlen(string + bytes_contados);
        if (bytes_leidos > 0 && *(string + bytes_contados + bytes_leidos -1) == '\n'){
            return string;
            
        } 
        else {
             char* str_aux = realloc(string,  (tamanio*2));
            if (!str_aux){
                salir = true;
            }
            
            string = str_aux;
            tamanio *= 2;
            
        }
        bytes_contados += bytes_leidos;
    }
    if(!bytes_contados){
        free(string);
        return NULL;
    }
    

    
    return string;
}
    
void fclosen(FILE* archivo){
    if(archivo){
        fclose(archivo);
    }
}