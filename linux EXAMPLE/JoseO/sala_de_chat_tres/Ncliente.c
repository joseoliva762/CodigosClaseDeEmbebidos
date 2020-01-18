//cliente dos
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <string.h>
#include <signal.h>
#include <fcntl.h>

//estructura del usuario.
typedef struct{
    unsigned char user[30];
    unsigned char id[12];
    unsigned int port;
    unsigned char data[100];
}packet;

void handler_signal(int sig);

//declaro cariables.
char* ip = "127.0.0.1";
unsigned int port = 5000;
int sock_desk_c1;
//creo mi paquete de datos.
packet datos;
packet* datos_recv; 
struct sockaddr_in server_c1;
unsigned char buff_c1_recv[200];

int main(void){
    system("clear");
       //lleno mi paquete de datos.
    memcpy(datos.user, "carlosoliva13", sizeof("carlosoliva13"));
    memcpy(datos.id, "3104990797", sizeof("3104990797"));
    datos.port =   port;
    sock_desk_c1  = socket(PF_INET, SOCK_STREAM, 0);
    //llenamos la estructura del servidor.
    server_c1.sin_family = AF_INET;
    server_c1.sin_addr.s_addr = inet_addr(ip);
    server_c1.sin_port = htons(port);
    //estructura para señales,
    struct sigaction signal;
    //asignamos
    signal.sa_handler = handler_signal;
    sigaction(SIGIO, &signal, 0);

    //establezco conexion.
    if(connect(sock_desk_c1, (struct sockaddr*) &server_c1, sizeof(server_c1))<0) printf("\tError de conexion.\r\n");
    system("clear");
    printf("\t Conexion OK.\r\n\n");
    //send(sock_desk_c1, (const void *)&datos, sizeof(datos), 0);
    fcntl(sock_desk_c1, F_SETFL,  O_ASYNC);
    fcntl(sock_desk_c1, F_SETOWN,  getpid());
    while(1){
        printf("%s <- ", datos.user);
        do{
            gets(datos.data);
            if(strlen(datos.data)>1)break;
        } while (1);
        if(send(sock_desk_c1, (const void *)&datos, sizeof(datos), 0)<0) printf("Error al enviar.");
        fflush(stdout);
        memset(datos.data, 0, sizeof(datos.data));
    }
    close(sock_desk_c1);
    return EXIT_SUCCESS;
}

void handler_signal(int sig){ 
    //printf("\r\n");
    if(recv(sock_desk_c1, buff_c1_recv, sizeof(buff_c1_recv), 0)>0){
        datos_recv = (packet*)buff_c1_recv;
        printf("\r%s -> %s\n", datos_recv->user, datos_recv->data);        
        printf("%s <- ", datos.user);
        fflush(stdout); //limpiar el buffer del printf
        memset(buff_c1_recv, 0, sizeof(buff_c1_recv));
    }
} 