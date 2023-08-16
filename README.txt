-INTRODUCCION:

En el presente trabajo se proveen distintos archivos .c y .h, implementados a lo largo de la catedra, asi como el tda hash, el tda abb, el tda lista, y el tda hash, los cuales nos ayudan a implementar las funciones de un salon de entrenadores de pokemones, donde a partir de un archivo se genera un salon, con un entrenador, y sus respectivos pokemones, y se pueden realizar varias acciones, asi como ver el equipo de un entrenador, agregar o quitar un pokemon, guardar todo a un archivo, o enfrentar a dos entrenadores. 

-Compilacion:

.gcc: para compilar los archivos

.make: para automatizar las tareas de compilacion

.make valgrind-server: para ejecutar el server



-EXPLICACION DE ESTRUCTURAS IMPLEMENTADAS:

.Para la estructura del salon tenemos un arbol de entrenadores,ya que al tener un comparador, me parecio lo mas sencillo para ordenar alfabeticamente los mismos. Ademas es mas simple y rapido buscarlos, por ejemplo para la implementacion de los comandos.
Ademas contamos con un hash de comandos donde le inserto los comandos creados, los cuales tienen el nombre y la funcion a ejecutar, entonces solo con hash_obtener, si existe la regla, ejecuto su comando, y de lo contrario devuelvo NULL.
La misma logica seguimos para las reglas, tambien implementadas como hash en el salon.

.Para la estructura del comando, utilice el nombre del mismo, y un ejecutable que me va a servir para ejecutar las operaciones del comando requerido.
El mismo es una funcion char* que devuelve el resultado para el salon_ejecutar_comando y recibe como parametros los campos, es decir el comando recibido, separado por 
los dos puntos " : " , y un salon donde voy a ejecutarlo.

.Para la estructura de la regla, usamos el nombre, la descripcion, y el ejecutable, el cual es una funcion que retorna como resultado un numero, que en este caso va a ser el ganador de la batalla al ejecutar el comando_comparar, y recibe como parametros dos pokemones, los cuales se comparan dependiendo la regla .

.Para la estructura entrenador, usamos el nombre, el nivel, y una lista que va a ser el equipo del mismo, la cual esta compuesta por pokemones, con sus respectivos atributos. Ya que esta me facilitaria agregar y eliminar pokemones, ya que tengo la posibilidad de eliminar de una posicion indicada.

.Para la estructura pokemon, mantuve la del tp1.


-EXPLICACION DE FUNCIONES:

Salon_leer_archivo: Abrimos el archivo de texto del salon, y llamamos  a una funcion auxiliar para leer el salon, donde creamos el salon, inicializando el arbol de entrenadores, el hash de comandos, y el hash de reglas.
Ademas, a medida que leemos las lineas, creamos los entrenadores, con sus respectivos pokemones, donde sabemos que si una linea tiene dos campos, es un entrenador, y si tiene seis, es un pokemon. Y los agregamos al salon. El arbol tiene un comparador que los ordena alfabeticamente.
Luego chequeamos que los entrenadores tengan al menos un pokemon.

Salon_guardar: Abrimos el archivo nuevo a escribir, con el nombre pasado como parametro. Luego recorremos el arbol de entrenadores, y a medida que lo recorremos, escribimos sus entrenadores y pokemones.

Salon_agregar_entrenador: Usamos arbol_insertar, de la implementacion del TDA ABB, e insertamos el entrenador.
 
Salon_filtrar_entrenadores: Pasamos todos los entrenadores a una lista, para crear un iterador, e ir recorriendo los entrenadores aplicandole la funcion.
De cumplir con la funcion, se insertan a la lista que devuelve la funcion.

Salon_ejecutar_comando: Separamos los campos del comando recibido como parametro, dividiendolos por ":", despues, de pertencer este a la tabla de hash, obtenemos el comando, y ejecutamos la funcion que le corresponde. La cual recibe como parametros los campos ya mencionados, y el salon.

Se ejecutan los comandos:

En cada uno de estos se reserva memoria para el resultado, y de fallar, se libera.
ENTRENADORES: Donde si el campo es uno solo, se devuelven los entrenadores con la ayuda de la funcion para filtrar. De lo contrario se vuelven a dividr los campos que ingresan como parametro, para ver si se requieren las victorias, o el equipo del mismo, y despues se muestra lo pedido.


EQUIPO:Donde se dividen los campos, se busca el entrenador por su nombre, y se muestran los pokemones del mismo.

REGLAS:Donde se muestran las reglas insertadas en el hash, con la ayuda de hash_con_cada_clave

COMPARAR: Donde se dividen los campos, se buscan los entrenadores pasados por parametro, y se llama a una funcion que crea iteradores para el equipo cada uno de ellos, y mientras tengan pokemones, se iteran dependiendo del resultado de el enfrentamiento.

AGREGAR_POKEMON: Donde se dividen los campos, se busca el entrenador pedido, se crea el pokemon con los atributos pasados, y se agrega al entrenador.

QUITAR_POKEMON: Donde se dividen los campos, se busca el entrenador pedido, se busca el pokemon recibido, y se elimina de la lista.

GUARDAR: Donde se dividen los campos, y se llama a la funcion salon_guardar_archivo, pasandole como parametros el salon a escribir, y el nombre recibido.


