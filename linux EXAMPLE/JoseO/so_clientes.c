//Sockets clientes
#include <stdio.h>
#include <stdlib.h>
//librerias para sockets
#include <sys/socket.h> //crear sockets
#include <netinet/in.h> //al macena las estructuras de tipo servidor
#include <string.h>



int main(void){
    int sock = 0;
    unsigned char buff[30] = "GET\n"; // tengo que mandar el verbo y el \n para que me entienda
    unsigned char recv_buff[500]; //buffer de almacenamiento.
    //funcion para crear un socket, me devuelve un numero entero, hay un limite de puertos 2e16
    //me pide 3 cosas, El protocolo(internet),  la forma de coneccio o que voy a transmitir y  seleccion del protocolo, 0 para internet
    sock = socket(PF_INET, SOCK_STREAM, 0);
    //protocol family internet
    //SOCK_STREAM es para TCP, tambien puedo utilizar SOCK_DGRAM para UDP.
    //creo una estructura:

    struct sockaddr_in server;
    server.sin_family = AF_INET; //para las estructuras se utiliza AF y para sockest PF
    //sin.family pra la familia que vamos a utilizar .
    server.sin_addr.s_addr = inet_addr("172.217.28.100");
    //funcion que me convierta a un numero de 32 bits inet_addr(" ");
    server.sin_port = htons(80);
    //funcion que me convierta a un numero de 16 bits htons( );

    //hacemos la coneccion.

    if (connect(sock, (struct sockaddr*) &server, sizeof(server))<0){
        printf("\tError de conexion.\r\n");
        return 1;
    } //server recive un sockaddr no un sock addr_in asi que se le hace un casting.
    printf("\t Conexion OK.\r\n\n");

    //tcp maneja las concexion. keep alive para mantener la coneccion aunque no haya transferencia, lo contrario es save alive.
    //ahora le pido la pagina.

    //pero la pagina esta en http:
    //get para pedir cosas, post, put.
    send(sock, buff, strlen(buff), 0);// ahora la comunicacion es con el socket no con el servidor, el buffer tiene 30 datos y solo utiliza GET para pedir la pagina asi que utlizo strlen() para que me de el tamaño exacto aunque el buyffer tenga 30.
    //ahora recibimos los paquetes que me envio, recuerda que el get me pide el index de la ip que seleccione.
    recv(sock, recv_buff, sizeof(recv_buff), 0);//donde lo va a recibir, donde lo va a almacenar
    //imprimimos en str todo lo que recivamos.
    printf("%s", recv_buff);
    pause(5);
    //investigar el local host.
    //si yo apunto al 127.0.0.1 me apunto a mi mismo.
    

    close(sock); // cerrar el socket importante.



    return  EXIT_SUCCESS;
}