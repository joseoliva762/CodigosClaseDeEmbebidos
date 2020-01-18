#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <string.h>

int main(void){
    int sock = 0;
    unsigned char buff[30]; 
    unsigned char recv_buff[500]; 
    printf("ingresa texto_ ");
    gets(buff);
    printf("\r\n\t%s", buff);
    //Creo la conecxion. (socked)
    sock = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("172.16.11.207"); //127.0.0.1
    server.sin_port = htons(12000);

    if (connect(sock, (struct sockaddr*) &server, sizeof(server))<0){
        printf("\tError de conexion.\r\n");
        return 1;
    }
    printf("\t Conexion OK.\r\n\n");


    //Pedimos la pagina
    send(sock, buff, strlen(buff), 0);
    recv(sock, recv_buff, sizeof(recv_buff), 0);
    printf("%s", recv_buff);
    pause(5);

    //cerramos la conexion.
    close(sock);
    return  EXIT_SUCCESS;
}