#include <stdio.h>
#include <unistd.h>
#include "edsu.h"
#include "comun.h"

// se ejecuta antes que el main de la aplicación
__attribute__((constructor)) void inicio(void){
    if (begin_clnt()<0) {
        fprintf(stderr, "Error al iniciarse aplicación\n");
        // terminamos con error la aplicación antes de que se inicie
	// en el resto de la biblioteca solo usaremos return
        _exit(1);
    }
}

// se ejecuta después del exit de la aplicación
__attribute__((destructor)) void fin(void){
    if (end_clnt()<0) {
        fprintf(stderr, "Error al terminar la aplicación\n");
        // terminamos con error la aplicación
	// en el resto de la biblioteca solo usaremos return
        _exit(1);
    }
}

// operaciones que implementan la funcionalidad del proyecto
int begin_clnt(void){
	int bytenum;
	int c;
	char host[100];
	char puerto[10];
	char buff[100];
//Tema de las variables de entorno
	if(getenv("BROKER_HOST")){
		snprintf(host, 100, "%s", getenv("BROKER_HOST"));
	}else{
		perror("no se ha especificado el host");
		return(-1);
	}

	if(getenv("BROKER_PORT")){
		snprintf(puerto, 10, "%s", getenv("BROKER_PORT"));
	}else{
		perror("no se ha especificado el puerto");
		return(-1);
	}

	printf("%s\n", host);
	printf("%s\n", puerto);

	return 0;
}
int end_clnt(void){
    return 0;
}
int subscribe(const char *tema){
    return 0;
}
int unsubscribe(const char *tema){
    return 0;
}
int publish(const char *tema, const void *evento, uint32_t tam_evento){
    return 0;
}
int get(char **tema, void **evento, uint32_t *tam_evento){
    return 0;
}

// operaciones que facilitan la depuración y la evaluación
int topics(){ // cuántos temas existen en el sistema
    return 0;
}
int clients(){ // cuántos clientes existen en el sistema
    return 0;
}
int subscribers(const char *tema){ // cuántos subscriptores tiene este tema
    return 0;
}
int events() { // nº eventos pendientes de recoger por este cliente
    return 0;
}
