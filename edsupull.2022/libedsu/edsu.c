//Puede que sobre algun include
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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
///////////VARIABLES//////////
	int sockfd;
/////////////////////////////
// operaciones que implementan la funcionalidad del proyecto
int begin_clnt(void){

	int bytenum;
	char* host = getenv("BROKER_HOST");
	char* puerto = getenv("BROKER_PORT");
	char buff[100], buffaux[100], buffAux2[100];
	struct sockaddr_in servaddr, clientaddr;
	struct hostent *host_info;

	//Creacion del socket
	sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sockfd ==-1){
		perror("[ERROR] cliente no ha podido crear el socket\n");
		return -1;
	}
	printf("Socket creado ok\n");

	host_info = gethostbyname(host);
	//Asignamos host y puerto
	servaddr.sin_family = PF_INET;
	memcpy(&servaddr.sin_addr.s_addr, host_info->h_addr, host_info->h_length);
	servaddr.sin_port = htons(atoi(puerto));

	//intentamos la conexion de cliente a socket y de socket a servidor
	if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
		perror("[ERROR CLIENTE] La conexion al servidor ha fallado\n");
		close(sockfd);
		return -1;
	}
	printf("Conexion ok\n");


	//preparamos el envio del uuid del cliente
	UUID_t uuid;
	generate_UUID(uuid);
	printf("%s", uuid);
	//enviamos el uid del cliente
	if(send(sockfd, uuid, strlen(uuid), 0)<0){
		perror("[ERROR CLIENTE] no se ha podido enviar el uuid cliente\n");
		close(sockfd);
		return -1;
	}

	//recibimos la respuesta del broker
	if(recv(sockfd, buff, 100, 0)<0){
		perror("[ERROR CLIENTE] no se ha recibido respuesta del broker\n");
		close(sockfd);
		return -1;
	}
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
	int nclientes;
	char aux[1024];
	if(recv(sockfd, aux, 1024, 0)<0){
		perror("[ERROR CLIENTE] no se ha realizado la llamada a clients correctamente\n");
		return -1;
	}
	nclientes = atoi(aux);
	return nclientes;
}
int subscribers(const char *tema){ // cuántos subscriptores tiene este tema
    return 0;
}
int events() { // nº eventos pendientes de recoger por este cliente
    return 0;
}
