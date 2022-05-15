// USA LOS 3 TIPOS DE DATOS PARA CONSTRUIR UN ESCENARIO SIMILAR AL DE
// LA PRÁCTICA: ENVÍO DE MENSAJES A LAS PERSONAS QUE PERTENECEN
// A UN GRUPO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include "map.h"
#include "set.h"
#include "queue.h"

typedef struct grupo {
    const char *nombre; // clave de acceso
    set *miembros;      // personas en ese grupo
    // ....
} grupo;

typedef struct persona {
    const char *nombre; // clave de acceso
    set *grupos;        // grupos a los que pertenece
    queue *mensajes;    // mensajes encolados a esa persona
    // ....
} persona;

typedef struct mensaje {
    char *texto;
    clock_t fecha;
    int contador;
    // ....
} mensaje;

// crea un grupo
grupo * crea_grupo(map *mg, const char *nombre) {
    grupo *g = malloc(sizeof(grupo));
    g->nombre = nombre;
    g->miembros = set_create(0);
    // se inserta en el mapa de grupos para poder ser accedido por nombre
    map_put(mg, g->nombre, g);
    return g;
}
// crea una persona
persona * crea_persona(map *mp, const char *nombre) {
    persona *p = malloc(sizeof(persona));
    p->nombre = nombre;
    p->grupos = set_create(0);
    // inicialmente, no tiene mensajes
    p->mensajes = queue_create(0);
    // se inserta en el mapa de personas para poder ser accedido por nombre
    map_put(mp, p->nombre, p);
    return p;
}
// crea un mensaje
mensaje * crea_mensaje(char *texto) {
    mensaje *m = malloc(sizeof(mensaje));
    m->texto=texto;
    m->fecha=times(NULL);
    m->contador=0;
    return m;
}
// añade una persona a grupo
int incluye_persona_en_grupo(grupo *g, persona *p) {
    int ret = -1;
    if (set_add(g->miembros, p)<0)
        fprintf(stderr, "error: persona ya pertenece al grupo\n");
    else if (set_add(p->grupos, g)<0)
        fprintf(stderr, "error: grupo ya incluye a esa persona\n");
    else ret = 0;
    return ret;
}

int encola_mensaje(map *mg, char *nombre_grupo, mensaje *m){
    int error;
    grupo * g = map_get(mg, nombre_grupo, &error);
    if (error==-1) return -1;

    set_iter *i = set_iter_init(g->miembros);
    for ( ; set_iter_has_next(i); set_iter_next(i)) {
	++m->contador;
        persona *p = ((persona *)(set_iter_value(i)));
        queue_push_back(p->mensajes, m);
    }
    set_iter_exit(i);
    return 0;
}

int consume_mensajes(map *mp, char *nombre_persona){
    // imprime mensajes recibidos por esa persona
    int error;
    persona *p = map_get(mp, nombre_persona, &error);
    if (error==-1) return -1;
    mensaje *m;
    printf("-----------------------------------\n");
    printf("Persona %s: mensajes encolados %d\n", nombre_persona, queue_length(p->mensajes));
    for ( ; queue_length(p->mensajes); ){
        m=queue_pop_front(p->mensajes, NULL);
        printf("%s ha recibido el mensaje (%s) en el momento %ld\n",
           nombre_persona, m->texto, m->fecha);
	if (--m->contador==0)
	    free(m);
    }
    return 0;
}
void imp_persona(void *e) {
    persona *p = e;
    printf("Persona: %s\n", p->nombre);
}
void imp_personas_en_grupo(grupo *g) {
    printf("-----------------------------------\n");
    printf("Grupo %s: %d miembros\n", g->nombre, set_size(g->miembros));
    set_visit(g->miembros, imp_persona);
}
void imp_grupo(void *e) {
    grupo *g = e;
    printf("Grupo: %s\n", g->nombre);
}
void imp_grupos_de_persona(persona *p) {
    printf("-----------------------------------\n");
    printf("Persona %s: %d grupos\n", p->nombre, set_size(p->grupos));
    set_visit(p->grupos, imp_grupo);
}
int baja_persona_en_grupo(map *mg, const char *ng, map *mp, const char *np) {
    int error;
    grupo * g = map_get(mg, ng, &error);
    if (error==-1) return -1;
    persona * p = map_get(mp, np, &error);
    if (error==-1) return -1;
    set_remove(g->miembros, p, NULL);
    set_remove(p->grupos, g, NULL);
    return 0;
}
int main(int argc, char *argv[]) {
    map *mapa_grupos = map_create(key_string, 0);
    map *mapa_personas = map_create(key_string, 0);

    printf("-----------------------------------\n");
    printf("Crea grupos G1 y G2\n");
    grupo *g1 = crea_grupo(mapa_grupos, "G1");
    grupo *g2 = crea_grupo(mapa_grupos, "G2");

    printf("-----------------------------------\n");
    printf("Crea persona Pa y la incluye en G1 y G2\n");
    persona *p1 = crea_persona(mapa_personas, "Pa");
    incluye_persona_en_grupo(g1, p1);
    incluye_persona_en_grupo(g2, p1);

    printf("-----------------------------------\n");
    printf("Crea persona Pb y la incluye dos veces en G2: error la 2ª vez\n");
    persona *p2 = crea_persona(mapa_personas, "Pb");
    incluye_persona_en_grupo(g2, p2);
    // debe dar error
    incluye_persona_en_grupo(g2, p2);
    
    // lista de miembros de "G2"
    imp_personas_en_grupo(g2);

    // lista de grupos de "Pa"
    imp_grupos_de_persona(p1);
    
    printf("-----------------------------------\n");
    printf("Envío de mensaje a G1\n");
    // envío de un mensaje a todos los miembros de "G1"
    // suponiendo que hay que hacerlo usando el nombre porque
    // no se dispone del descriptor del grupo
    mensaje *m1 = crea_mensaje("bienvenidos a G1");
    encola_mensaje(mapa_grupos, "G1", m1);

    printf("-----------------------------------\n");
    printf("Envío de mensaje a G2\n");
    // envío de un mensaje a todos los miembros de "G2"
    mensaje *m2 = crea_mensaje("bienvenidos a G2");
    encola_mensaje(mapa_grupos, "G2", m2);

    printf("-----------------------------------\n");
    printf("Pa consume todos sus mensajes\n");
    // "Pa" consume todos sus mensajes
    consume_mensajes(mapa_personas, "Pa");

    printf("-----------------------------------\n");
    printf("Pb consume todos sus mensajes\n");
    // "Pb" consume todos sus mensajes
    consume_mensajes(mapa_personas, "Pb");

    printf("-----------------------------------\n");
    printf("Pb se da de baja de G2\n");
    // "Pb" deja de pertenecer a "G2"
    // suponiendo que hay que hacerlo usando los nombres porque
    // no se dispone de los descriptores de grupo y persona
    baja_persona_en_grupo(mapa_grupos, "G2", mapa_personas, "Pb");

    // lista de miembros de "G2"
    imp_personas_en_grupo(g2);

    // lista de grupos de "Pb"
    imp_grupos_de_persona(p2);

    printf("-----------------------------------\n");
    printf("Envío de otro mensaje a G2\n");
    // envío de un mensaje a todos los miembros de "G2"
    mensaje *m3 = crea_mensaje("otro mensaje a G2");
    encola_mensaje(mapa_grupos, "G2", m3);

    printf("-----------------------------------\n");
    printf("Pa consume todos sus mensajes\n");
    // "Pa" consume todos sus mensajes
    consume_mensajes(mapa_personas, "Pa");

    printf("-----------------------------------\n");
    printf("Pb consume todos sus mensajes\n");
    // "Pb" consume todos sus mensajes
    consume_mensajes(mapa_personas, "Pb");

    return 0;
}
