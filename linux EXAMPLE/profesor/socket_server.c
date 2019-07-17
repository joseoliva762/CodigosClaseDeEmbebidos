#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(){

	int socket_desc, client_sock;
	int c, read_size;
	struct sockaddr_in server;
	struct sockaddr client;
	unsigned char buffer[100];

	//creamos el socket del servidor
	socket_desc = socket(PF_INET, SOCK_STREAM, 0);

	//llenamos la estructura del servidor
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(12000);

	bind(socket_desc, (struct sockaddr *)&server, sizeof(server));
	listen(socket_desc, 3);
	client_sock = accept(socket_desc, &client, &c);

	while(recv(client_sock, buffer, sizeof(buffer), 0) > 0){
		printf("%s\n", buffer);
		fflush(stdout);
		memset(buffer, 0, sizeof(buffer));
	}
	return(0);
}
