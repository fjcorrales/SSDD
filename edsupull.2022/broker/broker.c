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

//////////////////VARIABLES//////////////////////

map *temasList;			//set en el cual guardo los temas del fichero temas

//	map* subscripciones;		//mapa en el que asocio a un cliente con un conjunto de sus subscripciones
//	subscripciones = map_create(key_string, 0); //la key sera el uuid del cliente en cuestion y su valor asociado el set

map *clientes;			//mapa para guardar los clientes
struct cabecera head;		//cabecera que tenga actualmente

////////////////////////////////////////////////

struct cliente{
	char* id;
	set* subscrito;
	queue* eventos;
};

struct tema{
	char *nombre;
	set *subs;
};
struct cabecera{
	int h1;
	int h2;
	int h3;//tamanyo de petticion
};
//////////////////////////////////////////////
void *servicio(void *arg){

	int sockfd, leido;		//nclient sera el numero de cliente
	char tam[100], buff[sizeof(int)];
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
	printf("UID cliente: %s\n", tam);
	char* resp = "1";
        if(write(sockfd, resp, strlen(resp))<0){
		perror("[ERROR THREAD BROKER] no ha podido mandar respuesta\n");
	}

	while(1){//espera activa para peticiones
		//para tema peticiones y clients...
		recv(sockfd, &head, sizeof(head), 0);//recibo la cabecera
		int h3=ntohl(head.h3);//normalmente el tamanyo de la preticion
		char *pet = malloc(h3);
		recv(sockfd, pet, h3, 0);//guardar peticion
		//switch para ditinguir que peticiones
		int pet1 = atoi(pet);
		switch(pet1){
			case 1:	{//nº de clientes
				int nclientes = map_size(clientes);
				sprintf(buff, "%d", nclientes);
				send(sockfd, buff, sizeof(int), 0);
				memset(buff, 0, sizeof(int));//reinicio el buffer
				break;
			}

			case 2: {//nº de temas
				int nTemas = map_size(temasList);
				sprintf(buff, "%d", nTemas);
				send(sockfd, buff, sizeof(int), 0);
				memset(buff, 0, sizeof(int));//reinicio el buffer
				break;
			}

			case 3: {//subscripcion a un tema
				int error;
				int h1=ntohl(head.h1);//normalmente el tamayo del tema
				int h2=ntohl(head.h2);//normalmente el tamanyo del usuario (uuid)
				char *tema2 = malloc(h1+1);			//+1 para el caracter de fin de linea
				char *usuario = malloc(h2+1);
				recv(sockfd, tema2, h1, MSG_WAITALL);
				recv(sockfd, usuario, h2, MSG_WAITALL);
				tema2[h1]='\0';					//anyado los fin de linea
				usuario[h2]='\0';
				printf("El tema es: %s\n", tema2);
				printf("El cliente es: %s\n", usuario);
				struct cliente *cl = map_get(clientes, usuario, &error);//encuentro el cliente
				if(error==-1){
					printf("[ERROR BROKER] el cliente a subscribir no se ha encontrado\n");
					continue;
				}
				struct tema *tm = map_get(temasList, tema2, &error);//encuentro el tema
				if(error==-1){
					printf("[ERROR BROKER] el tema a subscribir no se ha encontrado\n");
					continue;
				}
				if((set_add(cl->subscrito, tm)==-1) || (set_add(tm->subs, cl)==-1)){//anyado el tema a las subscripciones del cliente
					printf("[ERROR BROKER] no se ha podido suscribir el cliente\n");
					sprintf(buff, "f");
					send(sockfd, buff, sizeof(int), 0);
					memset(buff, 0, sizeof(int));//reinicio el buffer
					continue;
				}else{
					sprintf(buff, "a");
					send(sockfd, buff, sizeof(int), 0);
					memset(buff, 0, sizeof(int));//reinicio el buffer
				}
				break;
			}

			case 4: {//nº de subs de un tema
				int error;
				int h1=ntohl(head.h1);//normalmente el tamayo del tema
				char *tema2 = malloc(h1+1);			//+1 para el caracter de fin de linea
				recv(sockfd, tema2, h1, MSG_WAITALL);
				tema2[h1]='\0';					//anyado los fin de linea
				printf("El tema es: %s\n", tema2);
				struct tema *tm = map_get(temasList, tema2, &error);//encuentro el tema
				if(error==-1){
					printf("[ERROR BROKER] el tema a subscribir no se ha encontrado\n");
					continue;
				}
				int nSubs = set_size(tm->subs);
				sprintf(buff, "%d", nSubs);
				send(sockfd, buff, sizeof(int), 0);
				memset(buff, 0, sizeof(int));//reinicio el buffer
				break;
			}
		}
	}
	close(sockfd);
	return NULL;
}

//para mas adelante


int main(int argc, char *argv[]){

	if(argc!=3) {
		fprintf(stderr, "Uso: %s puerto fichero_temas\n", argv[0]);
		return 1;
	}

	clientes = map_create(key_string, 0);
	temasList = map_create(key_string, 0);

	//creamos el socket
	char temaB[250];
	int sockfd, connfd;				//descriptores tanto de la conexion como de la conexion
	unsigned int len;				//es la longitud de la direccion del cliente
	struct sockaddr_in servaddr, clientaddr;
	char* host = getenv("BROKER_HOST");
	char* puerto = argv[1];
	int op = 1;
	printf("BROKER ABRE EL FICHERO TEMAS.TXT\n");
	//abro el fichero de temas, si no puedo envio mensaje de error
	FILE *f;
	f = fopen(argv[2], "r");
	if(f==NULL){
		perror("[ERROR SERVER] no se ha podido abrir el fichero de temas");
	}
	printf("BROKER ACABA DE ABRIR EL FICHERO TEMAS.TXT\n");

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



	printf("BROKER EMPIEZA A LEER EL FICHERO TEMAS\n");
	//antes de entrar al bucle de servicio, leo los temas del fichero temas
	while((fgets(temaB, 250, f))!=NULL){//while para ir sacando linea por linea los temas
		char *aux = strdup(temaB);
		struct tema *topic = malloc(sizeof(struct tema));
		topic->nombre = aux;
		topic->subs = set_create(0);
		if(map_put(temasList, topic->nombre, topic)!=0){
			printf("TEMA NO AÑADIDO\n");
		}else{
			printf("TEMA AÑADIDO-> %s", topic->nombre);
		}
	}
	fclose(f);
	printf("BORKER TERMINO DE LEER Y AÑADIR LOS TEMAS, CIERRA EL FICHERO\n");

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


