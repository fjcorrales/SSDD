//
// IMPLEMENTACIÓN DEL TIPO DE DATOS QUE GESTIONA UN CONJUNTO.
// ASEGURA QUE NO SE INCLUYEN DOS ENTRADAS QUE REFERENCIAN AL MISMO OBJETO.
//
// NO PUEDE MODIFICAR ESTE FICHERO.
// NO ES NECESARIO QUE CONOZCA LOS DETALLES DE LA IMPLEMENTACIÓN PARA USARLO.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "set.h"

// definición de tipos
struct entry {
    const void *elem;
};
#define MAGIC (('C' << 24) + ('J' << 16) + ('T' << 8) + 'O')
#define MAGIC_IT (('I' << 24) + ('T' << 16) + ('E' << 8) + 'R')

struct set {
    int magic;
    int nentries;
    struct entry *collection;
    int locking;
    pthread_mutex_t mut;
};
struct set_iter {
    int magic;
    set *s;
    int entry;
};

// especificación de funciones internas
static int lookup_entry(const set *s, const void *elem);
static void iter_set(const set *s,  func_entry_set_t iter);
static int check_set(const set *s);
    
// implementación de funciones externas
set *set_create(int locking) {
    set *s = malloc(sizeof(set));
    if (!s) return NULL;
    s->magic=MAGIC;
    s->nentries=0;
    s->collection=NULL;
    s->locking=locking;
    if (locking) pthread_mutex_init(&s->mut, NULL);
    return s;
}
int set_destroy(set *s, func_entry_set_t release_entry){
    if (check_set(s)) return -1;
    if (release_entry)
        iter_set(s, release_entry);
    free(s->collection);
    s->magic=0;
    if (s->locking) pthread_mutex_destroy(&s->mut);
    free(s);
    return 0;
}
int set_visit(const set *s, func_entry_set_t visit_entry){
    if (check_set(s)) return -1;
    if (visit_entry) {
        if (s->locking) pthread_mutex_lock((pthread_mutex_t *)&s->mut);
        iter_set(s, visit_entry);
        if (s->locking) pthread_mutex_unlock((pthread_mutex_t *)&s->mut);
    }
    return 0;
}
int set_add(set *s, const void *elem) {
    int res=0;
    if (s->locking) pthread_mutex_lock(&s->mut);
    if (check_set(s) || !elem || lookup_entry(s, elem)!=-1) res = -1;
    else {
        s->nentries++;
        s->collection=realloc(s->collection, s->nentries*sizeof(struct entry));
        if (s->collection) {
            struct entry e = {elem};
            s->collection[s->nentries-1] =  e;
	}
	else res=-1;
    }
    if (s->locking) pthread_mutex_unlock(&s->mut);
    return res;
}
int set_contains(const set *s, const void *elem){
    int res=1;
    if (s->locking) pthread_mutex_lock((pthread_mutex_t *)&s->mut);
    if ((check_set(s)) || (!elem || lookup_entry(s, elem)==-1)) res=0;
    if (s->locking) pthread_mutex_unlock((pthread_mutex_t *)&s->mut);
    return res;
}
int set_remove(set *s, const void *elem, func_entry_set_t release_entry){
    int res=0;
    int ne;
    if (s->locking) pthread_mutex_lock(&s->mut);
    if ((check_set(s)) || !elem || (ne=lookup_entry(s, elem))==-1) res = -1;
    else {
        if (release_entry)
            release_entry((void *)s->collection[ne].elem);
        s->nentries--;
        s->collection[ne] = s->collection[s->nentries];
        s->collection=realloc(s->collection, s->nentries*sizeof(struct entry));
    }
    if (s->locking) pthread_mutex_unlock(&s->mut);
    return res;
}
int set_size(const set *s){
    if (check_set(s))  return -1;
    return s->nentries;
}

// crea un iterador
set_iter *set_iter_init(set *s) {
    if (check_set(s)) return NULL;
    if (s->locking) pthread_mutex_lock(&s->mut);
    set_iter *i = malloc(sizeof(set_iter));
    i->magic=MAGIC_IT;
    i->s=s;
    i->entry=0;
    return i;
}

// comprueba si el iterador no ha llegado al final
int set_iter_has_next(set_iter *i){
    return ((i!=NULL) && (i->magic==MAGIC_IT) && (i->entry < i->s->nentries));
}

// avanza el iterador 
void set_iter_next(set_iter *i) {
    if ((i!=NULL) && (i->magic==MAGIC_IT))
    	++i->entry;
}

// obtiene el elemento apuntado por el iterador 
const void *set_iter_value(set_iter *i){
    if ((i!=NULL) && (i->magic==MAGIC_IT) && (i->entry < i->s->nentries))
	    return i->s->collection[i->entry].elem;
    else
	    return NULL;
}

// destruye un iterador
void set_iter_exit(set_iter *i) {
    if ((i!=NULL) && (i->magic==MAGIC_IT)) {
       i->magic=0;
       if (i->s->locking) pthread_mutex_unlock(&i->s->mut);
       free(i);
    }
}

// implementación de funciones internas
static int lookup_entry(const set *s, const void *elem){
    int i;

    if (!elem) return -1;
    for (i=0; (i<s->nentries && elem!=s->collection[i].elem); i++);
    return (i==s->nentries?-1:i);
}
static void iter_set(const set *s,  func_entry_set_t iter){
    for (int i=0; i<s->nentries; i++)
        iter((void *)s->collection[i].elem);
}
static int check_set(const set *s){
    int res=0;
    if (s==NULL || s->magic!=MAGIC){
        res=-1; fprintf(stderr, "el conjunto especificado no es válido\n");
    }
    return res;
}
