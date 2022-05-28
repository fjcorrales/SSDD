#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <map.h>
#include <set.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
	if(argc!=3) {
		fprintf(stderr, "Uso: %s puerto fichero_temas\n", argv[0]);
		return 1;
	}

	//creamos el socket
	int sockfd, connfd;				//descriptores tanto de la conexion como de la conexion
	unsigned int len;				//es la longitud de la direccion del cliente
	//int rx_long, tx_long = 0;			//la longitud en bytes de lo que llega y de lo que se envia
	//int nclients = clients();			//para saber el nº de clientes que hay en este momento en el sistema SEGURAMENTE NO FUNCIONE
	struct sockaddr_in servaddr, clientaddr;
	char* host = getenv("BROKER_HOST");
	char* puerto = getenv("BROKER_PORT");
	int op = 1;

	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sockfd == -1){
		perror("[ERROR] broker no ha podido crear el socket\n");
		return -1;
	}

	//Para reutilizar el puerto inemdiatamente
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(op)) < 0){
                perror("error en setsockopt");
                return 1;
        }

	//Asignamos host y puertos
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port=htons(atoi(argv[1]));

	//Hacemos el bind del socket
	if((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) < 0){
		perror("[ERROR SERVER] no se ha podido realizar el bind del socket\n");
		close(sockfd);
		return -1;
	}

	//Hacemos el listen, escucha 5 peticiones y a partir de ahi, libera uno y rellena con la proxima
	if((listen(sockfd, 5) < 0)){//tal vez hay que cambiarlo por los nclientes
		perror("[ERROR SERVER] no se ha podido inicializar el listen del socket\n");
		close(sockfd);
		return -1;
	}

	len = sizeof(clientaddr);
	pthread_t thid;
    	pthread_attr_t atrib_th;
    	pthread_attr_init(&atrib_th); // evita pthread_join
    	pthread_attr_setdetachstate(&atrib_th, PTHREAD_CREATE_DETACHED);
	while(1){
		//Realizo el accept
		connfd = accept(sockfd, (struct sockaddr *)&client, &len);
		if(connfd < 0){
			perror("ERROR SERVER no se ha podido aceptar la conexion\n");
			return -1;
		}else{
			pthread_create(&thid, &atrib_th, servicio, (void *)(long)connfd);
		}
	}
	close(sockfd);
	return 0;
}


//Cuando se crea el thread, llama a esta funcion de rutina "el main"
void *servicio(void *arg){
        int s_srv, tam;
        s_srv=(long) arg;
        //while (recv(s_srv, &tam, sizeof(tam), MSG_WAITALL)>0) {
        recv(s_srv, &tam, sizeof(tam), MSG_WAITALL); //Puede ser buena idea cambiart el MSG por 0
        int tamn=ntohl(tam);
        char *dato = malloc(tamn);
        send(s_srv, dato, tamn, 0);
	//send(s_srv, "buenas tardes", tamn, 0); //Para debugguear
        close(s_srv);
	return NULL;
}

//para mas adelante
struct cabecera {
	int long1;
	int long2;
};
