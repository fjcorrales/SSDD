// EJEMPLO DE USO DEL TIPO DE DATOS CONJUNTO
#include <stdio.h>
#include <stdlib.h>
#include "set.h"

struct punto {
    int x;
    int y;
};

void libera_punto(void *v) {
    // libera la estructura punto reservada mediante malloc
    free(v);
}
void imprime_punto(const struct punto *p) {
    printf("punto: (%d,%d)\n", p->x, p->y);
}
int main(int argc, char *argv[]) {
    set *cjto_ptos;

    // ejemplo que guarda en cada elemento del cjto_ptos una referencia
    // a un punto 2D
    cjto_ptos = set_create(0);

    struct punto *p = malloc(sizeof(struct punto));
    p->x = 1; p->y = 2;
    if (set_add(cjto_ptos, p) < 0)
        fprintf(stderr, "punto duplicado\n");

    // error al insertar elemento duplicado
    if (set_add(cjto_ptos, p) < 0)
        fprintf(stderr, "punto duplicado\n");

    p = malloc(sizeof(struct punto));
    p->x = 3; p->y = 4;
    if (set_add(cjto_ptos, p) < 0)
        fprintf(stderr, "punto duplicado\n");

    p = malloc(sizeof(struct punto));
    p->x = 5; p->y = 6;
    if (set_add(cjto_ptos, p) < 0)
        fprintf(stderr, "punto duplicado\n");

    printf("\nContenido del conjunto después de crear 3 puntos (%d entradas):\n", set_size(cjto_ptos));
    set_iter *i = set_iter_init(cjto_ptos);
    for ( ; set_iter_has_next(i); set_iter_next(i))
        imprime_punto(set_iter_value(i));
    set_iter_exit(i);

    // elimina el último punto creado
    if (set_remove(cjto_ptos, p, libera_punto) < 0)
        fprintf(stderr, "punto inexistente\n");

    printf("\nContenido del cjto_ptos después de eliminar un punto (%d entradas):\n", set_size(cjto_ptos));
    i = set_iter_init(cjto_ptos);
    for ( ; set_iter_has_next(i); set_iter_next(i))
        imprime_punto(set_iter_value(i));
    set_iter_exit(i);

    set_destroy(cjto_ptos, libera_punto);

    return 0;
}
