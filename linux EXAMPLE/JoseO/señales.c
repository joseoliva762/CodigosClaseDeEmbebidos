//señales.(interrupcion en el descriptor de archoivos con accept)
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <string.h>
#include <pthread.h>
//librerias para señales.
#include <signal.h>
#include <fcntl.h>

int  client_Sock = 0;
unsigned char buffer[100] ;

//procesando la señal
void handler_signal(int sig){
    printf("Dato Disponible.\n");
    printf("Señal rec %d\n", sig);
    recv(client_Sock, buffer, sizeof(buffer), 0);
    printf("%s\r\n", buffer);
    fflush(stdout); //limpiar el buffer del printf
    memset(buffer, 0, sizeof(buffer)); //limpiar el buffer. es decir, setea la memoria en 0.
} 

int main(void){
    system("clear");
    int socket_Desc;
    int c, read_Size;

    struct sockaddr_in server;
    struct sockaddr_in client;
    

    //se crea una estructura para la señal
    struct sigaction signal;

    //asignamos
    signal.sa_handler = handler_signal; //la funcion sin los parentesis.
    sigaction(SIGIO, &signal, 0);
    
    
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
    //creo otro socket (modigicacion)
    client_Sock = accept(socket_Desc, &client, &c);

    //abrirlo asciocronamente.
    fcntl(client_Sock, F_SETFL,  O_ASYNC);
    fcntl(client_Sock, F_SETOWN,  getpid());

    // hasta aqui acepte la conexion, ahora hago la comunicacion.
    /*
    while(recv(client_Sock, buffer, sizeof(buffer), 0)>0){
        printf("%s\r\n", buffer);
        fflush(stdout); //limpiar el buffer del printf
        memset(buffer, 0, sizeof(buffer)); //limpiar el buffer. es decir, setea la memoria en 0.
    }
    */


   sleep(30); // simula el while de escritura.
    return EXIT_SUCCESS;
}