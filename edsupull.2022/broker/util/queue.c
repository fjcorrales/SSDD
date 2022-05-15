//
// IMPLEMENTACIÓN DEL TIPO DE DATOS QUE GESTIONA UNA COLA.
// NO PUEDE MODIFICAR ESTE FICHERO.
// NO ES NECESARIO QUE CONOZCA LOS DETALLES DE LA IMPLEMENTACIÓN PARA USARLO.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "queue.h"

// definición de tipos
struct entry {
    const void *value;
    struct entry *next;
};
#define MAGIC (('Q' << 24) + ('U' << 16) + ('E' << 8) + 'U')
struct queue {
    int magic;
    int nentries;
    struct entry *first;
    struct entry *last;
    int locking;
    pthread_mutex_t mut;
};

// especificación de funciones internas
static void iter_queue(const queue *q, func_entry_queue_t iter, int destroy);
static int check_queue(const queue *q);
    
// implementación de funciones externas
queue *queue_create(int locking) {
    queue *q = malloc(sizeof(queue));
    if (!q) return NULL;
    q->magic=MAGIC;
    q->nentries=0;
    q->first=q->last=NULL;
    q->locking=locking;
    if (locking) pthread_mutex_init(&q->mut, NULL);
    return q;
}
int queue_destroy(queue *q, func_entry_queue_t release_entry){
    if (check_queue(q)) return -1;
    if (release_entry)
        iter_queue(q, release_entry, 1);
    q->magic=0;
    if (q->locking) pthread_mutex_destroy(&q->mut);
    free(q);
    return 0;
}
int queue_visit(const queue *q, func_entry_queue_t visit_entry){
    if (check_queue(q)) return -1;
    if (visit_entry) {
        if (q->locking) pthread_mutex_lock((pthread_mutex_t *)&q->mut);
        iter_queue(q, visit_entry, 0);
        if (q->locking) pthread_mutex_unlock((pthread_mutex_t *)&q->mut);
    }
    return 0;
}
int queue_push_back(queue *q, const void *value){
    if (check_queue(q))  return -1;
    struct entry *e;
    e=malloc(sizeof(struct entry));
    if (!e) return -1;
    if (q->locking) pthread_mutex_lock(&q->mut);
    q->nentries++;
    e->value=value;
    e->next=NULL;
    if (q->last) q->last->next=e;
    q->last=e;
    if (!q->first) q->first=q->last;
    if (q->locking) pthread_mutex_unlock(&q->mut);
    return 0;
}
void * queue_pop_front(queue *q, int *error){
    int err=0;
    void *value;
    if (q->locking) pthread_mutex_lock(&q->mut);
    if ((check_queue(q)) || !q->nentries) err=-1;
    else {
        struct entry *first=q->first;
        q->nentries--;
        value = (void *)first->value;
        q->first=first->next;
	free(first);
        if (!q->first) q->last=NULL;
    }
    if (error) *error=err;
    if (q->locking) pthread_mutex_unlock(&q->mut);
    return (err==-1?NULL:value);
}
int queue_length(const queue *q){
    if (check_queue(q))  return -1;
    return q->nentries;
}

// implementación de funciones internas
static void iter_queue(const queue *q, func_entry_queue_t iter, int destroy){
    struct entry *e, *next;
    for (e=q->first; e; e=next) {
        iter((void *)e->value);
        next=e->next;
	if (destroy) free(e);
    }
}
static int check_queue(const queue *q){
    int res=0;
    if (q==NULL || q->magic!=MAGIC){
        res=-1; fprintf(stderr, "la cola especificada no es válida\n");
    }
    return res;
}
