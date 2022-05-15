// EJEMPLO DE USO DEL TIPO DE DATOS COLA
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

struct punto2D {
    int x;
    int y;
};

void libera_punto2D(void *v) {
    // libera la estructura punto2D reservada mediante malloc
    free(v);
}
void inserta_punto2D(queue *c) {
    int x,y;
    struct punto2D *p;
    printf("Introduzca en una línea las coordenadas X e Y del punto a insertar:\n");
    scanf("%d%d", &x, &y); 
    p = malloc(sizeof(struct punto2D));
    p->x = x; p->y = y;
    queue_push_back(c, p);
}
void extrae_punto2D(queue *c) {
    struct punto2D *p;
    int error;
    p=queue_pop_front(c, &error);
    if (error < 0)
        printf("\ncola vacía\n");
    else
        printf("\npunto extraído: (%d,%d)\n", p->x, p->y);
    free(p);
}
void imprime_punto2D(void *v) {
    struct punto2D *p = v;
    printf("punto: (%d,%d)\n", p->x, p->y);
}
int main(int argc, char *argv[]) {
    queue *cola;

    // ejemplo que guarda en cada elemento de la lista una referencia
    // a un punto 2D
    cola = queue_create(0);
    inserta_punto2D(cola);
    inserta_punto2D(cola);
    inserta_punto2D(cola);
    printf("\nContenido de la cola (%d entradas):\n", queue_length(cola));
    queue_visit(cola, imprime_punto2D);
    extrae_punto2D(cola);
    extrae_punto2D(cola);
    printf("\nContenido de la cola (%d entradas):\n", queue_length(cola));
    queue_visit(cola, imprime_punto2D);
    queue_destroy(cola, libera_punto2D);

    return 0;
}
