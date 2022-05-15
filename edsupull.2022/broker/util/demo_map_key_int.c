// EJEMPLO DE USO DEL TIPO DE DATOS MAPA CON CLAVE DE TIPO INT
#include <stdio.h>
#include <stdlib.h>
#include "map.h"

struct persona {
    int ID; // se usará como clave
    char *nombre;
    int edad;
    // ....
};

void libera_persona(void *c, void *v) {
    struct persona *p = v;
    free(p->nombre);
    // libera la estructura persona reservada mediante malloc
    free(p);
}
void lee_persona(map *m) {
    char *nombre=NULL;
    int ID, edad;
    struct persona *p;
    printf("Introduzca en una línea el ID de la persona, su nombre y su edad:\n");
    // la opción %m realiza un malloc internamente
    scanf("%d%ms%d", &ID, &nombre, &edad); 
    p = malloc(sizeof(struct persona));
    p->nombre = nombre;
    p->ID = ID;
    p->edad = edad;
    if (map_put(m, &p->ID, p) < 0)
        fprintf(stderr, "persona duplicada\n");
}
void elimina_persona(map *m) {
    int ID;
    printf("Introduzca en una línea el ID de la persona a eliminar:\n");
    scanf("%d", &ID); 
    if (map_remove_entry(m, &ID, libera_persona) < 0)
        fprintf(stderr, "persona no existente\n");
}
void datos_persona(map *m) {
    int ID;
    struct persona *p;
    int error;
    printf("Introduzca en una línea el ID de la persona cuyos datos se quieren conocer:\n");
    scanf("%d", &ID); 
    p=map_get(m, &ID, &error);
    if (error < 0)
        fprintf(stderr, "persona no existente\n");
    else
        printf("edad %d\n", p->edad);
}
void imprime_persona(void *c, void *v) {
    struct persona *p = v;
    printf("ID %d nombre %s edad %d\n", *(int *)c, p->nombre, p->edad);
}
int main(int argc, char *argv[]) {
    map *mapa;

    mapa = map_create(key_string, 0);
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
