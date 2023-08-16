#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "hash.h"
#define FACTOR_REHASH 0.7
#define EXITO 0 
#define ERROR -1
typedef struct par
{
  bool vacio;
  char* clave;
  void* valor;
}par_t;

struct hash {
  size_t capacidad;
  size_t cantidad;
  hash_destruir_dato_t destruir;
  par_t** tabla;
};

int funcion_hash(const char* clave, size_t capacidad){
  size_t largo_clave = strlen(clave);
  int valor = 0;
  for (size_t i = 0; i < largo_clave; i++)
  {
    valor += (int)clave[i];
  }
  return valor % (int) capacidad;
}

par_t* crear_par(const char* clave, void* elemento){
  par_t* dato = malloc( sizeof(par_t));

  if (!dato){
    return NULL;
  }
  char* copia  = malloc(strlen(clave) + 1);
  if (!copia){
    free(dato);
    return NULL;
  }
  dato->clave = strcpy(copia, clave);
  dato->valor = elemento;
  dato->vacio = false;
  return dato;

}

int buscar_posicion(hash_t* hash, const char* clave){
  if (!hash){
    return EXITO;
  }
  int posicion = funcion_hash(clave, hash->capacidad) ;

  if (!hash->tabla[posicion]){
    return ERROR;
  }

  if (strcmp(hash->tabla[posicion]->clave, clave) == 0){
    return posicion;
  }else{
    int posicion_aux;
    posicion_aux = posicion +1;
    while (posicion_aux != posicion)
    {
      if (hash->capacidad == posicion_aux){
        posicion_aux = 0;
        if (posicion_aux == posicion){
          return ERROR;
        }
      }
      if (hash->tabla[posicion_aux] && strcmp(hash->tabla[posicion_aux]->clave, clave) == 0){
        return posicion_aux;
      }
      posicion_aux++;
    }
  }
  return ERROR;
}
int actualizar_clave(hash_t* hash, const char* clave, void* elemento, int posicion){
  if (!hash){
    return ERROR;
  }
  if(strcmp(hash->tabla[posicion]->clave, clave) == 0){
    if (hash->tabla[posicion] && hash->tabla[posicion]->valor && hash->destruir){
      hash->destruir(hash->tabla[posicion]->valor);
    }
    hash->tabla[posicion]->valor = elemento;
    return EXITO;
  }
  int nueva_posicion = buscar_posicion(hash,clave);
    if (strcmp(hash->tabla[nueva_posicion]->clave, clave) == 0){
      if (hash->tabla[nueva_posicion] && hash->tabla[nueva_posicion]->valor &&hash->destruir){
        hash->destruir(hash->tabla[nueva_posicion]->valor);
      }
      hash->tabla[nueva_posicion]->valor = elemento;
      return EXITO;
    }
    
  
  return ERROR;
}

int insertar_dato(hash_t* hash, const char* clave, void* elemento, int posicion){
  if (!hash){
    return ERROR;
  }
  if (!hash->tabla[posicion]){
    par_t* dato = crear_par(clave, elemento);
    if (!dato){
      return ERROR;
    }
    hash->tabla[posicion] = dato;
    hash->cantidad++;
  }else{
    char* copia = malloc(strlen(clave) + 1);
    strcpy(copia, clave);
    free(hash->tabla[posicion]->clave);
    hash->tabla[posicion]->clave = copia;
    hash->tabla[posicion]->valor = elemento;
    hash->tabla[posicion]->vacio = false;
    hash->cantidad++;
  }
  return EXITO;
}

int colision(hash_t* hash, const char* clave, void* elemento, int posicion){
  if (!hash){
    return ERROR;
  }
  if (hash_contiene(hash, clave) == true){
    return actualizar_clave(hash, clave, elemento, posicion);
  }
  int posicion_aux = posicion+1;
  while (posicion_aux != posicion)
  {
    if (hash->capacidad == posicion_aux){
      posicion_aux = 0;
      if (posicion_aux == posicion){
        return ERROR ;
      }
    }
    if (!hash->tabla[posicion_aux]|| hash->tabla[posicion_aux]->vacio == true){ 
      return insertar_dato(hash, clave, elemento, posicion_aux);
    }
    posicion_aux++;
  }
  return ERROR;
}
par_t** rehasheo(hash_t* hash){
  if (!hash){
    return NULL;
  }
  hash_t* nuevo_hash = hash_crear(hash->destruir, hash->capacidad*2);

  if (!nuevo_hash){
    return NULL;
  }
  for (size_t i = 0; i < hash->capacidad; i++)
  {
    if (hash->tabla[i] != NULL){
      hash_insertar(nuevo_hash, hash->tabla[i]->clave, hash->tabla[i]->valor);
      free(hash->tabla[i]->clave);
      free(hash->tabla[i]);
    }
  }
  free(hash->tabla);

  par_t** nueva_tabla = nuevo_hash->tabla;
  free(nuevo_hash);
  return nueva_tabla;
}


int destruir_par(hash_t* hash, int posicion){
  if (!hash){
    return ERROR;
  }
  if (hash->tabla[posicion]->vacio == false && hash->destruir){
    hash->destruir(hash->tabla[posicion]->valor);
  }
  strcpy(hash->tabla[posicion]->clave, "");
  hash->tabla[posicion]->valor = NULL;
  hash->tabla[posicion]->vacio = true;
  hash->cantidad--;
  return EXITO;
}

hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad_inicial){

  hash_t* hash = calloc(1, sizeof(hash_t));
  if (!hash){
    return NULL;
  }
  if ((int)capacidad_inicial<3){
	  capacidad_inicial = 3;
  }
  hash->capacidad = capacidad_inicial;
  hash->destruir = destruir_elemento;
  hash->tabla = calloc(capacidad_inicial,sizeof(par_t*));
  if (!hash->tabla){
    free(hash);
    return NULL;
  }
	return hash;

}


int hash_insertar(hash_t* hash, const char* clave, void* elemento){
  if(!hash||!clave){
    return ERROR;
  }
  float factor_rehash = 0.75;
  if (((hash_cantidad(hash))+1)/hash->capacidad >= factor_rehash){
    hash->tabla = rehasheo(hash);
    if (!hash->tabla){
      free(hash);
      return ERROR;
    }
    hash->capacidad = hash->capacidad*2;
  	}

  int posicion = funcion_hash(clave,hash->capacidad);
	if (!hash->tabla[posicion]){
		par_t* dato = crear_par(clave, elemento);
    if (!dato){
      return ERROR;
    }
    hash->tabla[posicion] = dato;
    hash->cantidad++;
    return EXITO;
    }
	
	else{
    return colision(hash,clave,elemento, posicion);
    
  }
	
	return ERROR;
} 
int hash_quitar(hash_t* hash, const char* clave){
  if (!hash|| !clave){
    return ERROR;
  }
  if (hash_contiene(hash, clave) == false){
    return ERROR;
  }

  int posicion = funcion_hash(clave,hash->capacidad);
  if (!hash->tabla[posicion]){
    return ERROR;
  }
  if (hash->tabla[posicion] && strcmp(hash->tabla[posicion]->clave,  clave) == 0){ 
    return destruir_par(hash, posicion);
  }
  posicion = buscar_posicion(hash,clave);
  if (!hash->tabla[posicion] && hash->tabla[posicion]){
    return ERROR;
  }
  if (strcmp(hash->tabla[posicion]->clave, clave) == 0){
    return destruir_par(hash, posicion);
  }
  
  return ERROR;
}

void* hash_obtener(hash_t* hash, const char* clave){
  if (!hash || !clave){
    return NULL;
  }

	int posicion = buscar_posicion(hash,clave);
	if (posicion == ERROR){
     return NULL;
  	}
	return hash->tabla[posicion]->valor;
}

size_t hash_cantidad(hash_t* hash){
  if (!hash){
    return 0;
  }
  return hash->cantidad;
}

bool hash_contiene(hash_t* hash, const char* clave){
  if (!hash || !clave){
    return false;
  }
  if (buscar_posicion(hash,clave)==ERROR){
    return false;
  }
  return true;
}

void hash_destruir(hash_t* hash){
  if (!hash){
    return;
  }
  if (hash->tabla){
    for (size_t i = 0; i < hash->capacidad; i++)
    {
      if (hash->tabla[i]){
        if (hash->tabla[i]->vacio == false && hash->destruir && hash->tabla[i]->valor){
          hash->destruir(hash->tabla[i]->valor);
        }
        free(hash->tabla[i]->clave);
        free(hash->tabla[i]);
      }
    }
    free(hash->tabla);
  }
  free(hash);
}

size_t hash_con_cada_clave(hash_t* hash, bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux){
  if (!hash||!funcion){
    return 0;
  }
  size_t claves_iteradas = 0;
  size_t posicion = 0;
  while (posicion < hash->capacidad){
    if (hash->tabla[posicion] && hash->tabla[posicion]->vacio == false){
      if (funcion(hash, hash->tabla[posicion]->clave, aux)){
        return claves_iteradas+1;
      }
      claves_iteradas++;
    }
    posicion++;
  }
  return claves_iteradas;
}