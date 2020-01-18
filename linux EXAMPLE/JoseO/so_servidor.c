#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <string.h>

int main(){
    system("clear");
    int socket_Desc, client_Sock;
    int c, read_Size;

    struct sockaddr_in server;
    struct sockaddr_in client;
    unsigned char buffer[100];
    
    //creamos el socker del servidor
    socket_Desc = socket(PF_INET, SOCK_STREAM, 0);

    //llenamos la estructura.
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY; //esta funcion me dice que escaja cualquier ip INADDR_ANY
    server.sin_port = htons(12000);

    //UNIR
    bind(socket_Desc,  (struct sockaddr*) &server, sizeof(server));
    //ahora escucha
    listen(socket_Desc, 3);
    //creo otro socket
    client_Sock = accept(socket_Desc, &client, &c);
    // hasta aqui acepte la conexion, ahora hago la comunicacion.
    while(recv(client_Sock, buffer, sizeof(buffer), 0)>0){
        printf("%s\r\n", buffer);
        fflush(stdout); //limpiar el buffer del printf
        memset(buffer, 0, sizeof(buffer)); //limpiar el buffer. es decir, setea la memoria en 0.
    }
    return EXIT_SUCCESS;
}

//comando: telnet 127.0.0.1  puerto