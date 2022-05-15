#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <map.h>
#include <set.h>
#include <group.h>
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
	int rx_long, tx_long = 0;			//la longitud en bytes de lo que llega y de lo que se envia
	int nclients = clients();			//para saber el nº de clientes que hay en este momento en el sistema SEGURAMENTE NO FUNCIONE
	struct sockaddr_in servaddr, client;
	char host[100] = getenv("BROKER_HOST");
	char puerto[10] = getenv("BROKER_PORT");
	char rx_buff[100];
	char tx_buff[100];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		perror("El broker no ha podido crear el socket\n");
		return -1;
	}

	//Asignamos host y puerto limpiando la estructura antes
	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(host);
	servaddr.sin_port = htons(puerto);

	//Hacemos el bind del socket
	if((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0){
		perror("ERROR SERVER no se ha podido realizar el bind del socket\n");
		return -1;
	}

	//Hacemos el listen
	if((listen(sockfd, nclients) != 0)){
		perror("ERROR SERVER no se ha podido inicializar el listen del socket\n");
	}

	len = sizeof(client);

	//Hacemos el accept de los datos que vengan del socket
	while(1){
		connfd = accept(sockfd, (struct sockaddr *)&client, &len);
		if(connfd < 0){
			perror("ERROR SERVER no se ha podido aceptar la conexion\n");
			return -1;
		}else{
			while(1){//Vamos a leer los datos proporcionados por el cliente hasta que se cierre
				rx_long = read(connfd, rx_buff, sizeof(rx_buff));
				if(rx_long == -1){
					perror("ERROR SERVER no se puede leer\n");
				}else if(rx_long == 0){
					printf("SERVER, se cerro el socket del cliente");
					close(connfd);
					break:
				}else{
					write(connfd, tx_buff, strlen(buff_tx));
					printf("SERVER: %s \n", rx_buff);
				}
			}
		}
	}
}
