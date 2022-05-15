//
// DEFINICIÓN DEL TIPO DE DATOS QUE GESTIONA UNA COLA.
// NO PUEDE MODIFICAR ESTE FICHERO.

/*
 * Cola: Lista de entradas gestionada con un esquema FIFO.
 *
 * Almacena una referencia (no copia) de cada valor.
 *
 * Al destruir una cola, su implementación invoca la función
 * de liberación especificada como parámetro por cada entrada existente
 * en el mismo para permitir que el usuario de esta colección pueda liberar,
 * si lo considera oportuno, la información asociada a cada clave y valor.
 *
 * Al eliminar una entrada, también se invoca la función especificada
 * como parámetro, si no es NULL, para esa entrada.
 */

#ifndef _QUEUE_H
#define _QUEUE_H      1

// Tipo opaco para ocultar la implementación
typedef struct queue queue;

// Tipo de datos para una función que visita una entrada
typedef void (*func_entry_queue_t) (void *valor);

// Crea una cola. Recibe como parámetro si usa mutex para asegurar exclusión
// mutua en sus operaciones.
// Devuelve una referencia a una cola o NULL en caso de error.
queue *queue_create(int locking);

// Destruye la cola especificada. Si tiene todavía entradas
// se invocará la función especificada como parámetro por cada una de ellas
// pasando como argumento a la misma el valor asociado a la entrada.
// Si la aplicación no está interesada en ser notificada de las entradas
// existentes, debe especificar NULL en el parámetro de esta función.
// Devuelve 0 si OK y -1 si error.
int queue_destroy(queue *q, func_entry_queue_t release_entry);

// Permite recorrer todas las entradas de una cola.
// Devuelve 0 si OK y -1 si error.
int queue_visit(const queue *q, func_entry_queue_t visit_entry);

// Inserta al final de la cola un nuevo elemento.
// Almacena una referencia (y no copia) del valor.
// Devuelve 0 si OK y -1 si error.
int queue_push_back(queue *q, const void *valor);

// Extrae el primer elemento de la cola.
// Dado que cualquier valor es válido, devuelve en el segundo parámetro
// si se ha producido un error: 0 si OK y -1 si error.
void * queue_pop_front(queue *q, int *error);

// Devuelve el nº de elementos en la cola, -1 si error.
int queue_length(const queue *q);

#endif // _QUEUE_H
