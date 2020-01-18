#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <string.h>

typedef struct{
    unsigned char descr[30];
    unsigned char id[10];
    float temp;
}packet;

int main(void){
    packet datos; 
    memcpy(datos.descr, "jose oliva", sizeof("jose_oliva"));
    memcpy(datos.id, "1103120463", sizeof("1103120463"));
    datos.temp = 37.1;

    int sock = 0; 
    int port = 12000;

    //Creo la conecxion. (socked)
    sock = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port);

    if (connect(sock, (struct sockaddr*) &server, sizeof(server))<0){
        printf("\tError de conexion.\r\n");
        return 1;
    }
    printf("\t Conexion OK.\r\n\n");

   if(send(sock, (const void *)&datos, sizeof(datos), 0) < 0){
		printf("error al enviar\n");
	}

    //Pedimos la pagina
    //send(sock, buff, strlen(buff), 0);
    //recv(sock, recv_buff, sizeof(recv_buff), 0);
    //printf("%s", recv_buff);
    //pause(5);
    

    //cerramos la conexion.
    close(sock);
    return  EXIT_SUCCESS;
}