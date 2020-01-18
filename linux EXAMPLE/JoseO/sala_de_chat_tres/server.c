//Hilos para el servidor
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <string.h>
#include <pthread.h>

//estructura del usuario.
typedef struct{
    unsigned char user[30];
    unsigned char id[12];
    unsigned int port;
    unsigned char data[100];
}packet;

void* task_c1();
void* task_c2();
//unsigned char buff_ack[] = "\tACK.\r\n";

// DECLARACION DE VARIABLES.

//datos del cliente uno.
packet* datos_c1;
int socket_desc_c1, client_sock_c1;
int c1 = 0, read_size_c1 = 0,  port_c1 = 5000;
//server.
struct sockaddr_in server_c1;
struct sockaddr_in client_c1;
unsigned char buff_c1[200];


// datos del Cliente dos.
packet* datos_c2;
int socket_desc_c2, client_sock_c2;
int c2 = 0, read_size_c2 = 0,  port_c2 = 5001;
//server.
struct sockaddr_in server_c2;
struct sockaddr_in client_c2;
unsigned char buff_c2[200];


int main(void){ 
    system("clear");
    //creo el socket del servidor
    do{
        socket_desc_c1 = socket(PF_INET, SOCK_STREAM, 0); 
        if(socket_desc_c1 == -1) printf("Error al crear el socket");
	}while(socket_desc_c1 == -1);
	system("clear");
    //llenamos la estructura del servidor para cliente uno.
    server_c1.sin_family = AF_INET;
    server_c1.sin_addr.s_addr = INADDR_ANY;
    server_c1.sin_port = htons(port_c1);

    //creo el socket del servidor
    socket_desc_c2 = socket(PF_INET, SOCK_STREAM, 0); 
	system("clear");
    //llenamos la estructura del servidor para cliente dos.
    server_c2.sin_family = AF_INET;
    server_c2.sin_addr.s_addr = INADDR_ANY;
    server_c2.sin_port = htons(port_c2);

    pthread_t hilo1, hilo2;
    pthread_create(&hilo1, NULL, task_c1, NULL);
    pthread_create(&hilo2, NULL, task_c2, NULL);
    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);
   return EXIT_SUCCESS;
}

void* task_c1(){
    printf("\tEscuchando:\r\n");
    //Unir.
    bind(socket_desc_c1, (struct sockaddr*) &server_c1, sizeof(server_c1));
    //Escucha.
    listen(socket_desc_c1, 3);
    //Creo el otro descriptor.
    client_sock_c1 = accept(socket_desc_c1, &client_c1, &c1);
    //send(client_sock_c1, buff_ack, strlen(buff_ack),0);
    while(1){
        recv(client_sock_c1, buff_c1, sizeof(buff_c1),0);
        datos_c1 = (packet*)buff_c1;
        printf("%s: %s.\r\n", datos_c1->user, datos_c1->data);
        //send(client_sock_c1, buff_ack, strlen(buff_ack),0);
        send(client_sock_c2, buff_c1, sizeof(buff_c1),0);
        
        fflush(stdout);
        memset(buff_c1, 0, sizeof(buff_c1));
    }
}

void* task_c2(){
    printf("\tEscuchando:\r\n");
    //Unir.
    bind(socket_desc_c2, (struct sockaddr*) &server_c2, sizeof(server_c2));
    //Escucha.
    listen(socket_desc_c2, 3);
    //Creo el otro descriptor.
    client_sock_c2 = accept(socket_desc_c2, &client_c2, &c2);
    //send(client_sock_c1, buff_ack, strlen(buff_ack),0);
    //send(client_sock_c2, buff_ack, strlen(buff_ack),0);
    while(1){
        recv(client_sock_c2, buff_c2, sizeof(buff_c2),0);
        datos_c2 = (packet*) buff_c2;
        printf("%s: %s.\r\n", datos_c2->user, datos_c2->data);
        //send(client_sock_c1, buff_ack, strlen(buff_ack),0);
        send(client_sock_c1, buff_c2, sizeof(buff_c2),0);
        
        fflush(stdout);
        memset(buff_c2, 0, sizeof(buff_c2));
    }
}
