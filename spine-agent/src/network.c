/* network.c -- funkcje do obslugi sieci */
#include <stdio.h>
#include <string.h>
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
