/*
 *
 * NO MODIFICAR
 *
 */
#ifndef _EDSU_H
#define _EDSU_H        1

#include <stddef.h>
#include <stdint.h>

// operaciones que implementan la funcionalidad del proyecto
int begin_clnt(void); // inicio de un cliente
int end_clnt(void); // fin del cliente

int subscribe(const char *tema);
int unsubscribe(const char *tema);

// es necesario especificar el tamaño del 2º campo (el valor asociado al
// evento) ya que puede tener un contenido de tipo binario
int publish(const char *tema, const void *evento, uint32_t tam_evento);

// obtiene el siguiente evento destinado a este cliente; los tres parámetros
// son de salida
int get(char **tema, void **evento, uint32_t *tam_evento);

// operaciones que facilitan la depuración y la evaluación
int topics(); // cuántos temas existen en el sistema
int clients(); // cuántos clientes existen en el sistema
int subscribers(const char *tema); // cuántos subscriptores tiene este tema
int events(); // nº eventos pendientes de recoger por este cliente

#endif // _EDSU_H
