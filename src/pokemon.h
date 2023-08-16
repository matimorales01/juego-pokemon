typedef struct _pokemon_t pokemon_t;

pokemon_t* crear_pokemon(char** campos);

char* devolver_nombre_pokemon(pokemon_t* pokemon);

int devolver_nivel_pokemon(pokemon_t* pokemon);

int devolver_fuerza_pokemon(pokemon_t* pokemon);

int devolver_inteligencia_pokemon(pokemon_t* pokemon);

int devolver_velocidad_pokemon(pokemon_t* pokemon);

int devolver_defensa_pokemon(pokemon_t* pokemon);

void pokemon_destruir(void* pokemon);