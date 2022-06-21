#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <map.h>
#include <set.h>
#include <queue.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include "comun.h"

//Cuando se crea el thread, llama a esta funcion de rutina "el main"

//////////////////VATIABLES//////////////////////

//	set* temasList;			//set en el cual guardo los temas del fichero temas
//	temasList = set_create(0);

//	map* subscripciones;		//mapa en el que asocio a un cliente con un conjunto de sus subscripciones
//	subscripciones = map_create(key_string, 0); //la key sera el uuid del cliente en cuestion y su valor asociado el set

	map *clientes;			//mapa para guardar los clientes
////////////////////////////////////////////////

struct cliente{
	char* id;
	set* subscrito;
	queue* eventos;
};

void *servicio(void *arg){

	int sockfd, leido;		//nclient sera el numero de cliente
	char tam[100], buff[100];
        sockfd=(long) arg;
	if(leido = recv(sockfd, tam, 100, 0)){
		//Si he conseguido leer un uuid, lo añado al mapa en su posicion
		struct cliente* nuevo = malloc(sizeof(struct cliente));
		nuevo->id = tam;
		nuevo->subscrito = set_create(0);
		nuevo->eventos = queue_create(0);
		map_put(clientes, nuevo->id, nuevo);		//en el mapa de clientes guardo el id del cliente como key y valor un objeto de tipo cliente
	}
	//enviar un 1 es respuesta ok
	printf("UID cliente: %s", tam);
	char* resp = "1";
        if(write(sockfd, resp, strlen(resp))<0){
		perror("[ERROR THREAD BROKER] no ha podido mandar respuesta\n");
	}
	while(1){//espera activa para peticiones
		//para tema peticiones y clients...
		recv(sockfd, buff, 100, 0);//guardar peticion en el buffer
		//switch para ditinguir que peticiones
		int pet = atoi(buff);
		memset(buff, 0, 100);//reinicio el buffer
		switch(pet){
			case 1:	{		//caso en que me pidan el nº de clientes
				int nclientes = map_size(clientes);
				sprintf(buff, "%d", nclientes);
				send(sockfd, buff, 100, 0);
			}

		}
	}
	close(sockfd);
	return NULL;
}

//para mas adelante
struct cabecera {
	int long1;
	int long2;
};

int main(int argc, char *argv[]){

	if(argc!=3) {
		fprintf(stderr, "Uso: %s puerto fichero_temas\n", argv[0]);
		return 1;
	}

	clientes = map_create(key_string, 0);

	//creamos el socket
	int sockfd, connfd;				//descriptores tanto de la conexion como de la conexion
	unsigned int len;				//es la longitud de la direccion del cliente
	//int rx_long, tx_long = 0;			//la longitud en bytes de lo que llega y de lo que se envia
	//int nclients = clients();			//para saber el nº de clientes que hay en este momento en el sistema SEGURAMENTE NO FUNCIONE
	struct sockaddr_in servaddr, clientaddr;
	char* host = getenv("BROKER_HOST");
	char* puerto = argv[1];
	int op = 1;

	//creo socket
	sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sockfd == -1){
		perror("[ERROR SERVER] broker no ha podido crear el socket\n");
		return -1;
	}
	printf("Socket creado correctamente\n");
	//Para reutilizar el puerto inemdiatamente
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(op)) < 0){
                perror("[ERROR SERVER] error en setsockopt");
                return 1;
        }
	printf("Setsockopt ok\n");

	//Asignamos host y puertos
	servaddr.sin_family = PF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port=htons(atoi(puerto));

	//Hacemos el bind del socket
	if((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) < 0){
		perror("[ERROR SERVER] no se ha podido realizar el bind del socket\n");
		close(sockfd);
		return -1;
	}
	printf("Bind ok\n");

	//Hacemos el listen, escucha 5 peticiones y a partir de ahi, libera uno y rellena con la proxima
	if((listen(sockfd, 5) < 0)){
		perror("[ERROR SERVER] no se ha podido inicializar el listen del socket\n");
		close(sockfd);
		return -1;
	}
	printf("Listen ok\n");

	len = sizeof(clientaddr);
	pthread_t thid;
    	pthread_attr_t atrib_th;
    	pthread_attr_init(&atrib_th); // evita pthread_join
    	pthread_attr_setdetachstate(&atrib_th, PTHREAD_CREATE_DETACHED);

	//leemos el fichero de temas y nos llenamos un conjunto con todos los temas
//	FILE *f;
//	f = fopen("temas", "r");
//	char temasAux[2048];
//	while(fgets(temasAux, 2048, f) != NULL){
//		set_add(temas, temasAux);
//	}

	//guardamos la cantidad de temas que tenemos y la enviamos al cliente
//	int ntemas = set_size(temas);
//	if(write(sockfd, &ntemas, sizeof(ntemas))<0){
//		perror("[ERROR BROKER] error al enviar el numero de temas que tenemos");
//		return -1;
//	}

	while(1){
		//Realizo el accept
		connfd = accept(sockfd, (struct sockaddr *)&clientaddr, &len);
		if(connfd < 0){
			perror("ERROR SERVER no se ha podido aceptar la conexion\n");
			return -1;
		}else{
			if(pthread_create(&thid, &atrib_th, servicio, (void *)(long)connfd)<0){
				perror("[ERROR SERVER] no ha sido posible crear el thread\n");
				return -1;
			}
			printf("thread creado con exito\n");
		}
	}
	close(sockfd);
	return 0;
}


