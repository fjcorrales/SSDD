#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "edsu.h"

int main(int argc, char *argv[]) {
    int n, v;
    char *op, *tema;
    char *fich=NULL;
    int fd;
    struct stat st;
    void *f;
    uint32_t tam;
    char *ntema;
    void *evento;
    do {
        printf("\nSeleccione operación (ctrl-D para terminar)\n\tS: subscribe|U: unsubscribe|P: publish|G: get\n\tC: clientes en el sistema|T: temas en el sistema\n\tN: subscriptores en un tema|E: eventos pendientes del cliente\n");
        op=tema=NULL;
        n=scanf("%ms", &op);
        if (n!=1) continue;
	if ((op[0]=='S') || (op[0]=='U') || (op[0]=='P') ||(op[0]=='N')) {
                printf("Introduzca el nombre del tema: ");
		n=scanf("%ms", &tema);
	}
        if (n!=1) continue;
        switch(op[0]) {
            case 'S':
                if (subscribe(tema)<0)
                    printf("error en subscripción al tema %s\n", tema);
                else
                    printf("subscrito a tema %s\n", tema);
                break;
            case 'U':
                if (unsubscribe(tema)<0)
                    printf("error al darse de baja del tema %s\n", tema);
                else
                    printf("dado de baja de tema %s\n", tema);
                break;
            case 'P':
                do {
                    printf("Introduzca el nombre del fichero que contiene el evento: ");
                    n=scanf("%ms", &fich);
                    fd = open(fich, O_RDONLY);
                    free(fich);
                } while ((n==1) && (fd == -1));
                if (n!=1) continue;
                fstat(fd, &st);
                f=mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
                close(fd);
                if (publish(tema, f, st.st_size)<0)
                    printf("error publicando en el tema %s\n", tema);
                else
                    printf("publicación en el tema %s\n", tema);
                munmap(f, st.st_size);
                break;
            case 'T':
                if ((v=topics())<0)
                    printf("error obteniendo nº temas en el sistema\n");
                else
                    printf("en el sistema hay %d temas\n", v);
                break;
            case 'C':
                if ((v=clients())<0)
                    printf("error obteniendo nº clientes en el sistema\n");
                else
                    printf("en el sistema hay %d clientes\n", v);
                break;
            case 'N':
                if ((v=subscribers(tema))<0)
                    printf("error obteniendo el estado del tema %s\n", tema);
                else
                    printf("el tema %s tiene %d subscriptores\n", tema, v);
                break;
            case 'E':
                if ((v=events())<0)
                    printf("error obteniendo nº eventos pendientes del cliente\n");
                else
                    printf("el cliente tiene pendientes %d eventos\n", v);
                break;
            case 'G':
                if (get(&ntema, &evento, &tam)<0)
                    printf("error leyendo evento\n");
                else {
                    if (tam) {
                        printf("evento leído: tema %s\n", ntema);
                        char *fich=NULL;
                        int fd;
                        do {
                            printf("Introduzca el nombre del fichero para almacenar el evento: ");
                            n=scanf("%ms", &fich);
                            fd = creat(fich, 0644);
                            free(fich);
                        } while ((n==1) && (fd == -1));
                        if (n!=1) continue;
                        write(fd, evento, tam);
                        close(fd);
                        free(ntema);
                        free(evento);
                    }
                    else printf("no hay eventos\n");
                }
                break;
            default:
                    printf("operación no válida\n");
            }
        if (tema) free(tema);
        if (op) free(op);
    } while (n!=EOF);
    return 0;
}

