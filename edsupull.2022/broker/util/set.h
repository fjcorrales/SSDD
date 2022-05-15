//
// DEFINICIÓN DEL TIPO DE DATOS QUE IMPLEMENTA UN CONJUNTO.
// ASEGURA QUE NO SE INCLUYEN DOS ENTRADAS QUE REFERENCIAN AL MISMO OBJETO.
//
// NO PUEDE MODIFICAR ESTE FICHERO.

/*
 * Conjunto: Colección de elementos no repetidos
 *
 * Almacena referencias (no copias) de cada elemento.
 * No permite añadir dos entradas (dos referencias al mismo objeto)
 *
 * Al destruir un conjunto, su implementación invoca por cada entrada
 * existente en el mismo la función de liberación especificada como parámetro
 * para permitir que el usuario de esta colección pueda liberar,
 * si lo considera oportuno, la información asociada a cada elemento.
 *
 * Al eliminar una entrada, también se invoca la función especificada
 * como parámetro, si no es NULL, para esa entrada.
 */

#ifndef _SET_H
#define _SET_H      1

// Tipo opaco para ocultar la implementación
typedef struct set set;
typedef struct set_iter set_iter;

// Tipo de datos para una función que visita un elemento
typedef void (*func_entry_set_t) (void *elem);

// Crea un conjunto. Recibe como parámetro si usa mutex para asegurar exclusión
// mutua en sus operaciones.
// Devuelve una referencia a un conjunto o NULL en caso de error.
set *set_create(int locking);

// Destruye el conjunto especificado. Si tiene todavía entradas
// se invocará la función especificada como parámetro por cada una de ellas
// pasando como argumentos a la misma el elemento contenido en la entrada.
// Si la aplicación no está interesada en ser notificada de las entradas
// existentes, debe especificar NULL en el parámetro de esta función.
// Devuelve 0 si OK y -1 si error.
int set_destroy(set *s, func_entry_set_t release_entry);

// Permite recorrer todas las entradas de un conjunto.
// Se invocará la función especificada como parámetro por cada una de ellas
// pasando como argumento a la misma el elemento almacenado en la entrada.
// Devuelve 0 si OK y -1 si error.
int set_visit(const set *s, func_entry_set_t visit_entry);

// Añade un elemento al conjunto.
// Almacena una referencia (y no una copia) del elemento.
// Devuelve 0 si OK y -1 si error (p.e. si duplicado).
int set_add(set *s, const void *elem);

// Comprueba si un elemento está incluido en un conjunto.
// Devuelve 1 si pertenece y 0 en caso contrario o si hay un error.
int set_contains(const set *s, const void *elem);

// Elimina un elemento del conjunto.
// de NULL en el tercer parámetro, se invoca esa función como parte
// de la eliminación de la entrada.
// Devuelve 0 si OK y -1 si error.
int set_remove(set *s, const void *elem, func_entry_set_t release_entry);

// Devuelve el nº de elementos del conjunto, -1 si error.
int set_size(const set *s);

// crea un iterador
set_iter *set_iter_init(set *m);

// comprueba si el iterador no ha llegado al final
int set_iter_has_next(set_iter *i);

// avanza el iterador 
void set_iter_next(set_iter *i);

// obtiene el elemento apuntado por el iterador 
const void *set_iter_value(set_iter *i);

// destruye un iterador
void set_iter_exit(set_iter *i);

#endif // _SET_H
