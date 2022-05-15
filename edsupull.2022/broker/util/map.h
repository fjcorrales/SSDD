//
// DEFINICIÓN DEL TIPO DE DATOS QUE GESTIONA UN MAPA.
// NO PUEDE MODIFICAR ESTE FICHERO.

/*
 * Mapa: Colección de entradas -> [clave y valor]
 *
 * Almacena referencias (no copias) de cada clave y valor.
 *
 * Al destruir un map, su implementación invoca por cada entrada
 * existente en el mismo la función de liberación especificada como parámetro
 * para permitir que el usuario de esta colección pueda liberar,
 * si lo considera oportuno, la información asociada a cada clave y valor.
 *
 * Al eliminar una entrada, también se invoca la función especificada
 * como parámetro, si no es NULL, para esa entrada.
 */

#ifndef _MAP_H
#define _MAP_H      1

// Tipo opaco para ocultar la implementación
typedef struct map map;

// Tipo de datos para una función que visita una entrada
typedef void (*func_entry_map_t) (void *key, void *value);

// Tipo de datos para una función que compara claves
// Debe devolver verdadero si son iguales
typedef int (*func_cmp_keys_t) (const void *k1, const void *k2);

// funciones de compararación predefinidas para varios tipos de claves
// para strings
int key_string(const void *k1, const void *k2);

// para int
int key_int(const void *k1, const void *k2);

// Crea un mapa. Recibe como parámetros la función que se usará
// para comparar la claves y si usa mutex para asegurar exclusión
// mutua en sus operaciones.
// Devuelve una referencia a un mapa o NULL en caso de error.
map *map_create(func_cmp_keys_t cmp, int locking);

// Destruye el mapa especificado. Si tiene todavía entradas
// se invocará la función especificada como parámetro por cada una de ellas
// pasando como argumentos a la misma la clave y valor de la entrada.
// Si la aplicación no está interesada en ser notificada de las entradas
// existentes, debe especificar NULL en el parámetro de esta función.
// Devuelve 0 si OK y -1 si error.
int map_destroy(map *m, func_entry_map_t release_entry);

// Permite recorrer todas las entradas de un mapa.
// Se invocará la función especificada como parámetro por cada una de ellas
// pasando como argumentos a la misma la clave y valor de la entrada.
// Devuelve 0 si OK y -1 si error.
int map_visit(const map *m, func_entry_map_t visit_entry);

// Inserta en el mapa indicado la entrada especificada por la clave
// y el valor. Almacena referencias (y no copias) de la clave y el valor.
// Devuelve 0 si OK y -1 si error (p.e. si duplicada).
int map_put(map *m, const void *key, const void *value);

// Retorna el valor asociado a una clave.
// Dado que cualquier valor es válido, devuelve en el tercer parámetro
// si se ha producido un error: 0 si OK y -1 si error.
void * map_get(const map *m, const void *key, int *error);

// Elimina una entrada dada su clave. Si se recibe un valor distinto
// de NULL en el tercer parámetro, se invoca esa función como parte
// de la eliminación de la entrada.
// Devuelve 0 si OK y -1 si error.
int map_remove_entry(map *m, const void *key, func_entry_map_t release_entry);

// Devuelve cuántas entradas tiene un mapa.
int map_size(const map *m);

#endif // _MAP_H
