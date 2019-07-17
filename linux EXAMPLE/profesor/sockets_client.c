#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(void) {

	int socket_desc;
	char buff[1024] = "GET\n";
	struct sockaddr_in server;
	char* ip = "216.58.222.196";
	int port = 80;

	//creacion del socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_desc == -1)
	{
		printf("Error al crear el socket");
		return(1);
	}

	server.sin_addr.s_addr = inet_addr(ip); //IP google.com
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	if(connect(socket_desc,(struct sockaddr *)&server, sizeof(server))<0)
	{
		printf("error de conexion\r\n");
		return(1);
	}

	if(send(socket_desc, "GET\n", sizeof("GET\n"), 0) < 0){
		printf("error al enviar\n");
	}
	else{
		memset(buff, 0, sizeof(buff));
		printf("El mensaje se envió\n");
		recv(socket_desc, buff, sizeof(buff), 0);
		printf("%s", buff);
	}
	close(socket_desc);
	return EXIT_SUCCESS;
}
