// EJEMPLO DE USO DEL TIPO DE DATOS MAPA DONDE LA CLAVE ES UNA
// ESTRUCTURA Y ES NECESARIO DEFINIR UNA FUNCIÓN DE COMPARACIÓN
// PARA ESE TIPO DE CLAVE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"

struct identidad {
    int ID;
    char *nombre;
};
struct persona {
    struct identidad ident; // se usará como clave
    int edad;
    // ....
};

int key_identidad(const void *c1, const void *c2) {
    struct identidad const *i1 = c1, *i2 = c2;
    return ((i1->ID==i2->ID) && !strcmp(i1->nombre,i2->nombre));
}

void libera_persona(void *c, void *v) {
    struct persona *p = v;
    free(p->ident.nombre);
    // libera la estructura persona reservada mediante malloc
    free(p);
}
void lee_persona(map *m) {
    int ID;
    char *nombre=NULL;
    int edad;
    struct persona *p;
    printf("Introduzca en una línea el ID de la persona, su nombre y su edad:\n");
    // la opción %m realiza un malloc internamente
    scanf("%d%ms%d", &ID, &nombre, &edad); 
    p = malloc(sizeof(struct persona));
    p->ident.nombre = nombre;
    p->ident.ID = ID;
    p->edad = edad;
    if (map_put(m, &p->ident, p) < 0)
        fprintf(stderr, "persona duplicada\n");
}
void elimina_persona(map *m) {
    struct identidad id;
    printf("Introduzca en una línea el ID y nombre de la persona a eliminar:\n");
    scanf("%d%ms", &id.ID, &id.nombre); 
    if (map_remove_entry(m, &id, libera_persona) < 0)
        fprintf(stderr, "persona no existente\n");
    free(id.nombre);
}
void datos_persona(map *m) {
    struct persona *p;
    int error;
    printf("Introduzca en una línea el ID y nombre de la persona cuyos datos se quieren conocer:\n");
    struct identidad id;
    scanf("%d%ms", &id.ID, &id.nombre); 
    p=map_get(m, &id, &error);
    if (error < 0)
        fprintf(stderr, "persona no existente\n");
    else
        printf("edad %d\n", p->edad);
    free(id.nombre);
}
void imprime_persona(void *c, void *v) {
    struct persona *p = v;
    printf("ID %d nombre %s edad %d\n", p->ident.ID, p->ident.nombre, p->edad);
}
int main(int argc, char *argv[]) {
    map *mapa;

    mapa = map_create(key_identidad, 0);
    lee_persona(mapa);
    lee_persona(mapa);
    lee_persona(mapa);
    printf("\nContenido del mapa (%d entradas):\n", map_size(mapa));
    map_visit(mapa, imprime_persona);
    elimina_persona(mapa);
    printf("\nContenido del mapa (%d entradas):\n", map_size(mapa));
    map_visit(mapa, imprime_persona);
    datos_persona(mapa);
    map_destroy(mapa, libera_persona);
    return 0;
}
