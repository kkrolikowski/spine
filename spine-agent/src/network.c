/* network.c -- funkcje do obslugi sieci */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "network.h"

int listener(int port) {
	int sockfd;
	struct sockaddr_in iface;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		return -1;
	bzero((char *) &iface, sizeof(iface));

	iface.sin_family = AF_INET;
	iface.sin_addr.s_addr = INADDR_ANY;
	iface.sin_port = htons(port);

	if(bind(sockfd, (struct sockaddr *) &iface, sizeof(iface)) < 0)
		return -1;
	listen(sockfd, CONN_QUEUE);

	return sockfd;
}
int clientConnection(int sockfd) {
	struct sockaddr_in client;
	socklen_t clilen;
	int clisockfd;

	clilen = sizeof(client);
	clisockfd = accept(sockfd, (struct sockaddr *) &client, &clilen);

	return clisockfd;
}
int GreetClient(int sockfd) {
	int sendbytes = 0;
	char * greet = "Spine Agent v1.0\n\r200 Go Ahead\n\r";

	// przygotowujemy bufor
	char buff[NET_BUFFER];
	memset(buff, '\0', NET_BUFFER);

	strcpy(buff, greet);
	sendbytes = write(sockfd, buff, sizeof(buff));

	return sendbytes;
}
char * readClientData(int sockfd) {
	int readbytes = 0;
	char * clientresp = NULL;		// odpowiedz klienta
	size_t resplen = 0;				// dlugosc stringu przeslanego przez klienta
	int i = 0;						// index bufora

	// przygotowujemy bufor
	char buff[NET_BUFFER];
	memset(buff, '\0', NET_BUFFER);

	readbytes = read(sockfd, buff, sizeof(buff));
	if(readbytes > 0) {
		resplen = strlen(buff) + 1;
		clientresp = (char *) malloc(resplen * sizeof(char));
		memset(buff, '\0', NET_BUFFER);
		while(buff[i]) {
			*clientresp = buff[i];
			clientresp++;
			i++;
		}
	}
	return clientresp;
}