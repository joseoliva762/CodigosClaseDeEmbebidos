#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <fcntl.h>

int client_sock;
unsigned char buffer[100] = {0};
//procesando la señal

void handler_signal(int sig){
	printf("procesando la señal %d\n", sig);
	recv(client_sock, buffer, sizeof(buffer), 0);
	printf("%s\n", buffer);
	fflush(stdout);
	memset(buffer, 0, sizeof(buffer));
}

int main(){
	int socket_desc;
	int c, read_size;
	struct sockaddr_in server;
	struct sockaddr client;

	struct sigaction signal;
	signal.sa_handler = handler_signal;
	sigaction(SIGIO, &signal, 0);

	//creamos el socket del servidor
	socket_desc = socket(PF_INET, SOCK_STREAM, 0);

	//llenamos la estructura del servidor
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(12000);

	bind(socket_desc, (struct sockaddr *)&server, sizeof(server));
	listen(socket_desc, 3);
	client_sock = accept(socket_desc, &client, &c);

	fcntl(client_sock, F_SETFL, O_ASYNC);
	fcntl(client_sock, F_SETOWN, getpid());

	/*while(recv(client_sock, buffer, sizeof(buffer), 0) > 0){
		printf("%s\n", buffer);
		fflush(stdout);
		memset(buffer, 0, sizeof(buffer));
	}*/

	sleep(60);

	return(0);
}
