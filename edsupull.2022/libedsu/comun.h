/*
 * Incluya en este fichero todas las definiciones que pueden
 * necesitar compartir el broker y la biblioteca, si es que las hubiera.
 */

#ifndef _COMUN_H
#define _COMUN_H	1

#define UUID_SIZE 36
typedef char UUID_t[UUID_SIZE+1]; // +1 para el nulo del string

int generate_UUID(UUID_t uuid);

#endif // _COMUN_H
