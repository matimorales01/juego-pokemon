#include "abb.h"
#include <stdio.h>
#include <stdbool.h>
#define ABB_RECORRER_INORDEN   0
#define ABB_RECORRER_PREORDEN  1
#define ABB_RECORRER_POSTORDEN 2
#define EXITO 0
#define ERROR -1


nodo_abb_t* crear_nodo(void* elemento) {

	nodo_abb_t* nodo_nuevo = malloc(sizeof(nodo_abb_t));
	if(!nodo_nuevo) {
		return NULL;
	}
	nodo_nuevo->elemento = elemento;
	nodo_nuevo->derecha = NULL;
	nodo_nuevo->izquierda = NULL;
	return nodo_nuevo;

}

void destruir_elemento(nodo_abb_t* nodo, abb_liberar_elemento destructor){
	
	if(destructor && nodo->elemento){
		destructor(nodo->elemento);
	}
	free(nodo);
	
	
}

nodo_abb_t* nodo_hijo(nodo_abb_t* nodo){
	nodo_abb_t* nodo_hijo;

	if(nodo->derecha){
		nodo_hijo = (nodo->derecha);
	}
	else{
		nodo_hijo = (nodo->izquierda);
	}

	return nodo_hijo;
}


int arbol_insertar_aux(abb_t* arbol, nodo_abb_t* nodo, void* elemento) {

	if(((int*)nodo->elemento != 0) && (!nodo->elemento)) {
		return ERROR;
	}

	int comparador = arbol->comparador(nodo->elemento, elemento);

	if(comparador >= 0) {
		if(nodo->izquierda == NULL) {
			nodo_abb_t* nodo_nuevo = crear_nodo(elemento);
			nodo->izquierda = nodo_nuevo;
			return EXITO; 
		}
		return arbol_insertar_aux(arbol, nodo->izquierda, elemento);
	}
	if(comparador < 0) {
		if(nodo->derecha == NULL) {
			nodo_abb_t* nodo_nuevo = crear_nodo(elemento);
			nodo->derecha = nodo_nuevo;
			return EXITO; 
		}
		return arbol_insertar_aux(arbol, nodo->derecha, elemento);
	}
	return ERROR; 
	
}
nodo_abb_t* arbol_borrar_aux(abb_t* arbol, nodo_abb_t* nodo, void* elemento ){
	
	if(!arbol || !nodo){
		return NULL;
	}
	int comparar = arbol->comparador(elemento, nodo->elemento);
	if(comparar < 0){
		nodo->izquierda = arbol_borrar_aux(arbol, nodo->izquierda, elemento);
	}
	else if(comparar > 0){
		nodo->derecha = arbol_borrar_aux(arbol, nodo->derecha, elemento);
	}
	else{
		if(!nodo->izquierda && !nodo->derecha){
			destruir_elemento(nodo, arbol->destructor);
			return NULL;
		}
		else if(!nodo->izquierda || !nodo->derecha){
			nodo_abb_t* nodo_auxiliar = nodo_hijo(nodo);
			destruir_elemento(nodo, arbol->destructor);
			return nodo_auxiliar;
		}
		else{
		nodo_abb_t* nodo_auxiliar = nodo->izquierda;
		nodo_abb_t* nodo_actual = nodo->izquierda;

		while(nodo_actual->derecha){

			nodo_auxiliar = nodo_actual->derecha;
			nodo_actual = nodo_actual->derecha;
		}

		void* elemento_auxiliar = nodo->elemento;
	
		nodo->elemento = nodo_auxiliar->elemento;
		nodo_auxiliar->elemento = elemento_auxiliar;

		nodo_abb_t* elemento_actual = elemento_auxiliar;

		nodo->izquierda = arbol_borrar_aux(arbol, nodo->izquierda, elemento_actual);
		}
	}

	return nodo;
}

void destruir_nodos(nodo_abb_t* nodo, abb_liberar_elemento destructor){
    if(!nodo){
        return;
    }

    if(nodo->izquierda){
        destruir_nodos(nodo->izquierda, destructor);
    }

    if(nodo->derecha){
        destruir_nodos(nodo->derecha, destructor);
    }

    if(destructor){
        destructor(nodo->elemento);
    }

   free(nodo);
}

void nodo_recorrido_inorden(nodo_abb_t* nodo, void** array, size_t tamanio_array,size_t* contador){

	if (!nodo || *contador >= tamanio_array){
		return;
	}
	if(nodo->izquierda){
		nodo_recorrido_inorden(nodo->izquierda,array,tamanio_array,contador);
	}
	if(nodo && *contador < tamanio_array){
		array[*contador] = nodo->elemento;
		(*contador)++;
	}
	else
		return;
	if (nodo->derecha){
		nodo_recorrido_inorden(nodo->derecha,array,tamanio_array,contador);
	}

	return;
}

void nodo_recorrido_preorden(nodo_abb_t* nodo, void** array, size_t tamanio_array,size_t* contador){

	if (!nodo || *contador >= tamanio_array){
		return;

	}
	if(nodo && *contador < tamanio_array){
		array[*contador] = nodo->elemento;
		(*contador)++;
	}
	else
		return;
	if(nodo->izquierda){
		nodo_recorrido_preorden(nodo->izquierda,array,tamanio_array,contador);
	}
	if (nodo->derecha){
		nodo_recorrido_preorden(nodo->derecha,array,tamanio_array,contador);
	}

	return;
}

void recorrido_postorden(nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* contador){
	if (!nodo || *contador >= tamanio_array){
		return;
	}
	if(nodo->izquierda){
		recorrido_postorden(nodo->izquierda,array,tamanio_array,contador);
	}
	if (nodo->derecha){
		recorrido_postorden(nodo->derecha,array,tamanio_array,contador);
	}
	if(nodo && *contador < tamanio_array){
		array[*contador] = nodo->elemento;
		(*contador)++;
	}
	else
		return;
}

void recorrer_inorden(nodo_abb_t *nodo, bool (*funcion)(void*, void*), void* extra, size_t* contador, bool*corte){
    
    if (!nodo||*corte == true || !funcion){
        return;
    }
    recorrer_inorden(nodo->izquierda, funcion, extra, contador, corte);
    if (!nodo||*corte == true){
        return;  
    }
    (*contador)++;
    if (funcion(nodo->elemento, extra)){
        *corte = true;
    }
       

    recorrer_inorden(nodo->derecha, funcion, extra, contador, corte);
	return;

}


void recorrer_preorden(nodo_abb_t *nodo, bool (*funcion)(void*, void*), void* extra, size_t* contador, bool*corte){

    if (!nodo||*corte == true || !funcion ){
        return;
    }
    
    (*contador)++;
    if (funcion(nodo->elemento, extra)){
        *corte = true;
    }

    recorrer_preorden(nodo->izquierda, funcion, extra, contador, corte);
    
    recorrer_preorden(nodo->derecha, funcion, extra, contador, corte);
    return;
}

void recorrer_postorden(nodo_abb_t *nodo, bool (*funcion)(void*, void*), void* extra, size_t* contador, bool*corte){
    if (!nodo||*corte == true || !funcion){
        return ;
    }
    recorrer_postorden(nodo->izquierda, funcion, extra, contador, corte);
    
    recorrer_postorden(nodo->derecha, funcion, extra, contador, corte);
    
    if (!nodo||*corte == true){
        return;  
    }
	
    (*contador)++;
	
    if (funcion(nodo->elemento, extra)){
        *corte = true;
    }
	return;	
}

abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){
    abb_t* arbol = malloc(sizeof(abb_t));
    if (!arbol){
        return NULL;
    }    
    arbol->nodo_raiz = NULL;
    arbol->comparador = comparador;
    arbol->destructor = destructor;
    return arbol;
}
int arbol_insertar(abb_t* arbol, void* elemento){
    if (!arbol){
        return ERROR;
    }
    
    if (!arbol->nodo_raiz){
    	nodo_abb_t* nodo_nuevo  = crear_nodo(elemento);
        arbol->nodo_raiz = nodo_nuevo;
        
        return EXITO;
    }
    else{
    	arbol_insertar_aux(arbol, arbol->nodo_raiz, elemento);
		return EXITO;
	}
	return ERROR;

}

int arbol_borrar(abb_t* arbol, void* elemento){
	
    if (!arbol || !arbol->nodo_raiz){
        return ERROR;
    }
	else{
		arbol->nodo_raiz = arbol_borrar_aux(arbol, arbol->nodo_raiz, elemento);
		return EXITO;
	}

	return ERROR;
}




void* arbol_buscar(abb_t* arbol, void* elemento){
    if (!arbol || !arbol->nodo_raiz){
        return NULL;
    }

    nodo_abb_t* nodo_aux = arbol->nodo_raiz;
	if (!arbol->nodo_raiz->izquierda && !arbol->nodo_raiz->derecha ){
		if (arbol->nodo_raiz->elemento == elemento){
			return arbol->nodo_raiz->elemento;
		}
		else{
			return NULL;
		}
		
	}
    while (nodo_aux)
    {
        if (arbol->comparador(nodo_aux->elemento, elemento) == 0){
            return nodo_aux->elemento;
        }
       
    	if (arbol->comparador(elemento, nodo_aux->elemento)> 0){
        	nodo_aux = nodo_aux->derecha;
		
    	}
		else{
        	nodo_aux = nodo_aux->izquierda;
    	}
	}
    return NULL; 
}
void* arbol_raiz(abb_t* arbol){
    if (!arbol || !arbol->nodo_raiz){
        return NULL;
    }
    return arbol->nodo_raiz->elemento;
}
bool arbol_vacio(abb_t* arbol){
    if (!arbol || !arbol->nodo_raiz){
        return true;
    }
    return false;
}



size_t arbol_recorrido_inorden(abb_t* arbol, void** array, size_t tamanio_array){

	if (!arbol||arbol_vacio(arbol)||tamanio_array == 0){
		return 0;
	}
	size_t elementos_recorridos = 0;
    
    nodo_recorrido_inorden(arbol->nodo_raiz,array,tamanio_array,&elementos_recorridos);

    return elementos_recorridos;
}



size_t arbol_recorrido_preorden(abb_t* arbol, void** array, size_t tamanio_array){

	if (!arbol||arbol_vacio(arbol)||tamanio_array == 0)
		return 0;

	size_t elementos_recorridos = 0;
    
    nodo_recorrido_preorden(arbol->nodo_raiz,array,tamanio_array,&elementos_recorridos);

    return elementos_recorridos;
}

size_t arbol_recorrido_postorden(abb_t* arbol, void** array, size_t tamanio_array){
    if (!arbol || arbol_vacio(arbol)|| tamanio_array == 0){
		return 0;
	}
	size_t elementos_recorridos = 0;
	recorrido_postorden(arbol->nodo_raiz, array, tamanio_array, &elementos_recorridos);
	return elementos_recorridos;
}

void arbol_destruir(abb_t* arbol){
    if(!arbol){
        return;
    }

    destruir_nodos(arbol->nodo_raiz, arbol->destructor);

	free(arbol);
}


size_t abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra){

    if (arbol_vacio(arbol)||!funcion){
        return 0;
    }

    size_t elementos_recorridos = 0;
    bool corte = false;

    if (recorrido == ABB_RECORRER_INORDEN){
        recorrer_inorden(arbol->nodo_raiz, funcion, extra, &elementos_recorridos, &corte);
        
    }else if (recorrido == ABB_RECORRER_PREORDEN){
        recorrer_preorden(arbol->nodo_raiz, funcion, extra, &elementos_recorridos, &corte);
        
    }else if (recorrido == ABB_RECORRER_POSTORDEN){
        recorrer_postorden(arbol->nodo_raiz, funcion, extra, &elementos_recorridos, &corte);
        
    }else{
        elementos_recorridos = 0;
    }
    return elementos_recorridos;
} 