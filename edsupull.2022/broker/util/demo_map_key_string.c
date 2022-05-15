// EJEMPLO DE USO DEL TIPO DE DATOS MAPA CON CLAVE DE TIPO STRING
#include <stdio.h>
#include <stdlib.h>
#include "map.h"

struct persona {
    char *nombre; // se usará como clave
    int edad;
    // ....
};

void libera_persona(void *c, void *v) {
    // libera la clave reservada por la opción %m de scanf
    free(c);
    // libera la estructura persona reservada mediante malloc
    free(v);
}
void lee_persona(map *m) {
    char *nombre=NULL;
    int edad;
    struct persona *p;
    printf("Introduzca en una línea el nombre de la persona y su edad:\n");
    // la opción %m realiza un malloc internamente
    scanf("%ms%d", &nombre, &edad); 
    p = malloc(sizeof(struct persona));
    p->nombre = nombre;
    p->edad = edad;
    if (map_put(m, p->nombre, p) < 0)
        fprintf(stderr, "persona duplicada\n");
}
void elimina_persona(map *m) {
    char *nombre=NULL;
    printf("Introduzca en una línea el nombre de la persona a eliminar:\n");
    // la opción %m realiza un malloc internamente
    scanf("%ms", &nombre); 
    if (map_remove_entry(m, nombre, libera_persona) < 0)
        fprintf(stderr, "persona no existente\n");
    free(nombre);
}
void datos_persona(map *m) {
    char *nombre=NULL;
    struct persona *p;
    int error;
    printf("Introduzca en una línea el nombre de la persona cuyos datos se quieren conocer:\n");
    // la opción %m realiza un malloc internamente
    scanf("%ms", &nombre); 
    p=map_get(m, nombre, &error);
    if (error < 0)
        fprintf(stderr, "persona no existente\n");
    else
        printf("edad %d\n", p->edad);
    free(nombre);
}
void imprime_persona(void *c, void *v) {
    struct persona *p = v;
    printf("nombre %s edad %d\n", (char *)c, p->edad);
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
