//Hilos para el servidor
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <string.h>
#include <pthread.h>

//estructura del usuario.
typedef struct{
    unsigned char zone[30];
    unsigned char id[12];
    unsigned char type[10];
    unsigned char data[100];
}packet;

void* task_c1();
void* task_c2();
//unsigned char buff_ack[] = "\tACK.\r\n";

// DECLARACION DE VARIABLES.

//datos del cliente uno.
packet* datos_c1;
int socket_desc_c1, client_sock_c1;
int c1 = 0, read_size_c1 = 0,  port_c1 = 3001;  //Puerto.
//server.
struct sockaddr_in server_c1;
struct sockaddr_in client_c1;
unsigned char buff_c1[200];
FILE *ptrFile = NULL;


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

    ///////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////
    printf("\tEscuchando:\r\n");
    //Unir.
    bind(socket_desc_c1, (struct sockaddr*) &server_c1, sizeof(server_c1));
    //Escucha.
    listen(socket_desc_c1, 3);
    //Creo el otro descriptor.
    client_sock_c1 = accept(socket_desc_c1, &client_c1, &c1);
    //send(client_sock_c1, buff_ack, strlen(buff_ack),0);
    int rev = 0;
    while(1){
        rev = 0;
        recv(client_sock_c1, buff_c1, sizeof(buff_c1),0);
        rev = 1;
        datos_c1 = (packet*)buff_c1;
        printf("Se registro %s, en la finca %s, en el aspersor: %s \r\n", datos_c1->type, datos_c1->zone,  datos_c1->id);
        //send(client_sock_c1, buff_ack, strlen(buff_ack),0);

        if(rev == 1){
            ptrFile = fopen("vibracion.txt", "a");
            if(ptrFile == NULL) printf("Archivo no encontrado.\r\n ");
            else{
                printf("Archivo creado.\r\n");
                //fprintf(ptrFile, " \r\n");
                fprintf(ptrFile, "Se registro %s, en la finca %s, en el aspersor: %s \r\n", datos_c1->type, datos_c1->zone,  datos_c1->id);
                fclose(ptrFile);
            }
        }
        
       
        fflush(stdout);
        memset(buff_c1, 0, sizeof(buff_c1));
    }
    ///////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////
    
   return EXIT_SUCCESS;
}