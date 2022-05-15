//
// IMPLEMENTACIÓN DEL TIPO DE DATOS QUE GESTIONA UN MAPA.
// NO PUEDE MODIFICAR ESTE FICHERO.
// NO ES NECESARIO QUE CONOZCA LOS DETALLES DE LA IMPLEMENTACIÓN PARA USARLO.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "map.h"

// definición de tipos
struct entry {
    const void *key;
    const void *value;
};
#define MAGIC (('M' << 24) + ('A' << 16) + ('P' << 8) + 'A')
struct map {
    int magic;
    int nentries;
    func_cmp_keys_t cmp_keys;
    struct entry *collection;
    int locking;
    pthread_mutex_t mut;
};

// especificación de funciones internas
static int search_entry(const map *m, const void *key);
static void iter_map(const map *m,  func_entry_map_t iter);
static int check_map(const map *m);
    
// funciones de compararación predefinidas para varios tipos de claves
int key_string(const void *k1, const void *k2) {
    return !strcmp(k1, k2);
}
int key_int(const void *k1, const void *k2) {
    return *(int *)k1 == *(int *)k2;
}

// implementación de funciones externas
map *map_create(func_cmp_keys_t cmp, int locking) {
    map *m = malloc(sizeof(map));
    if (!m) return NULL;
    m->magic=MAGIC;
    m->nentries=0;
    m->cmp_keys=cmp;
    m->collection=NULL;
    m->locking=locking;
    if (locking) pthread_mutex_init(&m->mut, NULL);
    return m;
}
int map_destroy(map *m, func_entry_map_t release_entry){
    if (check_map(m)) return -1;
    if (release_entry)
        iter_map(m, release_entry);
    free(m->collection);
    m->magic=0;
    if (m->locking) pthread_mutex_destroy(&m->mut);
    free(m);
    return 0;
}
int map_visit(const map *m, func_entry_map_t visitar_entry){
    if (check_map(m)) return -1;
    if (visitar_entry) {
        if (m->locking) pthread_mutex_lock((pthread_mutex_t *)&m->mut);
        iter_map(m, visitar_entry);
        if (m->locking) pthread_mutex_unlock((pthread_mutex_t *)&m->mut);
    }
    return 0;
}
int map_put(map *m, const void *key, const void *value) {
    int res=0;
    if (m->locking) pthread_mutex_lock(&m->mut);
    if (check_map(m) || !key || search_entry(m, key)!=-1)  res = -1;
    else {
        m->nentries++;
        m->collection=realloc(m->collection, m->nentries*sizeof(struct entry));
        if (m->collection) {
            struct entry e = {key, value};
            m->collection[m->nentries-1] =  e;
	}
	else res=-1;
    }
    if (m->locking) pthread_mutex_unlock(&m->mut);
    return res;
}
void * map_get(const map *m, const void *key, int *error){
    int ne, err=0;
    if (m->locking) pthread_mutex_lock((pthread_mutex_t *)&m->mut);
    if ((check_map(m)) || (!key || (ne=search_entry(m, key))==-1)) err=-1;
    if (error) *error=err;
    void *v = (void *)m->collection[ne].value;
    if (m->locking) pthread_mutex_unlock((pthread_mutex_t *)&m->mut);
    return (err==-1?NULL:v);
}
int map_remove_entry(map *m, const void *key, func_entry_map_t release_entry) {
    int ne, res=0;
    if (m->locking) pthread_mutex_lock(&m->mut);
    if ((check_map(m)) || !key || (ne=search_entry(m, key))==-1) res= -1;
    else {
        if (release_entry)
            release_entry((void *)m->collection[ne].key,
                (void *)m->collection[ne].value);
        m->nentries--;
        m->collection[ne] = m->collection[m->nentries];
        m->collection=realloc(m->collection, m->nentries*sizeof(struct entry));
    }
    if (m->locking) pthread_mutex_unlock(&m->mut);
    return res;
}

int map_size(const map *m) {
    if (check_map(m))  return -1;
    return m->nentries;
}

// implementación de funciones internas
static int search_entry(const map *m, const void *key){
    int i;

    if (!key) return -1;
    for (i=0; (i<m->nentries && !m->cmp_keys(key, m->collection[i].key)); i++);
    return (i==m->nentries?-1:i);
}
static void iter_map(const map *m,  func_entry_map_t iter){
    for (int i=0; i<m->nentries; i++)
        iter((void *)m->collection[i].key, (void *)m->collection[i].value);
}
static int check_map(const map *m){
    int res=0;
    if (m==NULL || m->magic!=MAGIC){
        res=-1; fprintf(stderr, "el mapa especificado no es válido\n");
    }
    return res;
}
