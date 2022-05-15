#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <map.h>
#include <set.h>
#include <group.h>

int main(int argc, char *argv[]){
	if(argc!=3) {
		fprintf(stderr, "Uso: %s puerto fichero_temas\n", argv[0]);
		return 1;
	}
/*
	en este fichero va el bind, el listen y accept
	Intento mio de realizar la conexion persistente, copia del progrma conecta visto en clase
*/

	if((s=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		perror("error creando socket\n");
		return 1;
	}

	struct addrinfo *res;
	if(getaddrinfo(argv[1], argv[2], NULL, &res) != 0){
		perror("error en getaddrinfo\n");
		close(s);
		return 1;
	}


	freeaddrinfo(res);
	printf("despues del connect\n");
	return 0;
}
