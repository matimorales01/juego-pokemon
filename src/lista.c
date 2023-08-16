#include "lista.h"
#include "stdlib.h"
#define EXITO 0
#define ERROR -1
lista_t* lista_crear(lista_liberar_elemento destructor){
    lista_t* lista = malloc(sizeof(lista_t));
    if (!lista){
        return NULL;
    }
    lista->nodo_inicio = NULL;
    lista->nodo_fin = NULL;
    lista->cantidad = 0;
    lista->destructor = destructor;
    return lista;

}
int lista_insertar(lista_t* lista, void* elemento){
    if (!lista){
        return ERROR;
    }
    nodo_t* nodo_nuevo = malloc(sizeof(nodo_t));
    
    if (!nodo_nuevo){
        return ERROR;
    }
    nodo_nuevo->elemento = elemento;
    nodo_nuevo->siguiente = NULL;
    
    if (lista->cantidad == 0){
        lista->nodo_inicio = nodo_nuevo;
        lista->nodo_fin = nodo_nuevo;
        lista->cantidad ++;
        return  EXITO;
    }
    else{
        
        nodo_t *nodo_aux = lista->nodo_fin;
 
       
        nodo_aux->siguiente = nodo_nuevo;
        lista->nodo_fin = nodo_nuevo;
        lista->cantidad++;
        return EXITO;
    }
    

    return ERROR;
}
 
int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){
    if (!lista){
        return ERROR;
    }
    if (lista->cantidad == 0 || posicion > lista->cantidad-1){
        lista_insertar(lista, elemento);
        return EXITO;
    }
    else{
        nodo_t* nodo_nuevo = malloc(sizeof(nodo_t));
        if ( !nodo_nuevo){
            return ERROR;
        }
        nodo_nuevo->elemento = elemento;
        nodo_nuevo->siguiente = NULL;
        nodo_t *nodo_aux = lista->nodo_inicio;

        if (posicion == 0){
            nodo_nuevo->siguiente = nodo_aux;
            lista->nodo_inicio = nodo_nuevo;
            lista->cantidad ++;
            return EXITO;
        }
        else{
            for (size_t i = 0; i< posicion-1; i++){
                nodo_aux = nodo_aux->siguiente;
            }
            nodo_nuevo->siguiente  = nodo_aux->siguiente;
            nodo_aux->siguiente = nodo_nuevo;
            lista->cantidad ++;
            return EXITO;

        }

    }
   return ERROR;

}     

int lista_borrar(lista_t* lista){
    if (!lista || lista->cantidad == 0){
        return ERROR;
    }
    nodo_t* nodo_aux = lista->nodo_inicio;
    if (lista->cantidad == 1){
        lista->nodo_inicio = NULL;
        lista->nodo_fin = NULL;
        if (lista->destructor){
            lista->destructor(nodo_aux->elemento);
        }
        free(nodo_aux);
        lista->cantidad --;
        return EXITO;
    }
    else{  
        for (size_t i = 0; i<lista->cantidad-2; i++){
            nodo_aux = nodo_aux->siguiente;
        }
        lista->nodo_fin = nodo_aux;
        if (lista->destructor){
            lista->destructor(nodo_aux->siguiente->elemento);
        }
        free(nodo_aux->siguiente);
        lista->cantidad--;
        lista->nodo_fin->siguiente = NULL;
        return EXITO;
    }
    return ERROR;
   
}

int lista_borrar_de_posicion(lista_t* lista, size_t posicion){
    if (!lista || lista->cantidad == 0){
        return ERROR;
    }
    if (posicion >= lista->cantidad -1){
        lista_borrar(lista);
        return EXITO;
    }
    else{
        
        
            nodo_t* nodo_aux = lista->nodo_inicio;

            if (posicion == 0){
                lista->nodo_inicio = nodo_aux->siguiente;
                lista->destructor(nodo_aux->elemento);
                free(nodo_aux);
                
                lista->cantidad --;
                return EXITO;
            }
            else{
                nodo_t* nodo_a_borrar = NULL;
                for (size_t i = 0; i<posicion-1; i++){
                    nodo_aux = nodo_aux ->siguiente;
                    

                }
                nodo_a_borrar = nodo_aux ->siguiente;
                nodo_aux->siguiente = nodo_a_borrar->siguiente;
                if (lista->destructor){
                    lista->destructor(nodo_a_borrar->elemento);
                }
                free(nodo_a_borrar);
                lista->cantidad --;
                return EXITO;
                
                
                
            }
        
    }


    return ERROR;
}
void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){
    void* elemento;
    if (!lista || posicion > lista->cantidad-1 || lista->cantidad==0 ){
        elemento =  NULL;
    }
    else{
        nodo_t* nodo_aux = lista->nodo_inicio;
        for (size_t i = 0; i<posicion; i++){
            nodo_aux = nodo_aux->siguiente;
        }
        elemento = nodo_aux->elemento;
    }
   return elemento;
}

void* lista_ultimo(lista_t* lista){
    void* ultimo_elemento ;
    if (!lista || lista->cantidad == 0){
        ultimo_elemento = NULL;
    }
    else{
        ultimo_elemento = lista->nodo_fin->elemento;
    }
    return ultimo_elemento;
}

bool lista_vacia(lista_t* lista){
   if (!lista || lista->cantidad == 0){
       return true;
   }
   return false;

}

size_t lista_elementos(lista_t* lista){
    size_t cantidad = 0;
    if (!lista){
        cantidad =  0;
    }
    else{
        cantidad = lista->cantidad;
    }
    return cantidad;
}

int lista_apilar(lista_t* lista, void* elemento){
    if (!lista){
        return ERROR;
    }
    lista_insertar(lista, elemento);
    return EXITO;
}

int lista_desapilar(lista_t* lista){
    if  (lista_vacia(lista)){
        return ERROR;
    }
    lista_borrar(lista);
    return EXITO;
}

void* lista_tope(lista_t* lista){
    void* tope ;
    if (lista_vacia(lista)){
        tope = NULL;
    }
    else{
        tope = lista->nodo_fin->elemento;
    }
    return tope;
}

int lista_encolar(lista_t* lista, void* elemento){
    if (!lista){
        return ERROR;
    }
    lista_insertar(lista, elemento);
    return EXITO;
    
}

int lista_desencolar(lista_t* lista){
    if (lista_vacia(lista)){
        return ERROR;
    }
    lista_borrar_de_posicion(lista,0);
    return EXITO;
}

void* lista_primero(lista_t* lista){
    void* primer_elemento;
    if (lista_vacia(lista)){
        primer_elemento = NULL;
    }
        else{
            primer_elemento = lista->nodo_inicio->elemento;
        }
    return primer_elemento;
}

void lista_destruir(lista_t* lista){
    while (!lista_vacia(lista)){
        lista_borrar(lista);

    }
    free(lista);
}
        

lista_iterador_t* lista_iterador_crear(lista_t* lista){
    if (!lista){
        return NULL;
    }

    lista_iterador_t* iterador = malloc(sizeof(lista_iterador_t));
    if (!iterador){
        return NULL;
    }
    iterador->lista = lista;
    iterador->corriente = lista->nodo_inicio;

    return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
    bool salida;
    if (iterador->corriente == NULL){
        salida = false;
    }else{
        salida = true;
    }
    return salida;
}

bool lista_iterador_avanzar(lista_iterador_t* iterador){
    if (!iterador){
        return false;
    }   
    if (iterador->corriente) {
        iterador->corriente = iterador->corriente->siguiente;
        if (iterador->corriente == NULL)
            return false;
        return true;
    }
    return false;
}

void* lista_iterador_elemento_actual(lista_iterador_t* iterador){
    if (iterador->corriente == NULL){
        return NULL;
    }
    return iterador->corriente->elemento;

}

void lista_iterador_destruir(lista_iterador_t* iterador){
    free(iterador);
}

size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, void*), void *contexto){
    if (!funcion){
        return 0;
    }
    if (!lista){
        return 0;
    }
    size_t cantidad = 0;
    
    nodo_t* nodo_aux = lista->nodo_inicio;
    while(nodo_aux != NULL){
        if(funcion(nodo_aux->elemento, contexto)){
            cantidad++;
        }
        nodo_aux = nodo_aux->siguiente;
    }
    return cantidad;
}


