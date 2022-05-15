/*
 * Incluya en este fichero todas las implementaciones que pueden
 * necesitar compartir el broker y la biblioteca, si es que las hubiera.
 */
#include "comun.h"

#include <unistd.h>
#include <stdio.h>

// debe usarse para obtener un UUID para el cliente
#define STR_VALUE(x) STR(x)
#define STR(x) #x

int generate_UUID(UUID_t uuid) {
    FILE *d = popen("dbus-uuidgen", "r");
    if (d)
        fscanf(d, "%"STR_VALUE(UUID_SIZE)"s", uuid);
    pclose(d);
    return (d? 0:-1);
}
