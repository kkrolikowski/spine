/* network.c -- funkcje do obslugi sieci */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "network.h"
#include "core.h"

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
int connector(char * host, int port) {
	int sockfd;
	struct sockaddr_in dsthost;
	struct hostent * dstip;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;
	if((dstip = gethostbyname(host)) == NULL) {
		close(sockfd);
		return -2;
	}
	bzero((char *) &dsthost, sizeof(dsthost));
	dsthost.sin_family = AF_INET;
	bcopy((char *) dstip->h_addr_list[0], (char *) &dsthost.sin_addr.s_addr, dstip->h_length);
	dsthost.sin_port = htons(port);

	if(connect(sockfd, (struct sockaddr *) &dsthost, sizeof(dsthost)) < 0) {
		close(sockfd);
		return -3;
	}

	return sockfd;
}
netinfo clientConnection(int sockfd) {
	struct sockaddr_in client;
	socklen_t clilen;
	netinfo net;
	struct in_addr ip;				// adres IP w formie binarnej
	size_t ip_len = 0;				// dlugosc stringu z adresem ip
	char tmp[16];					// tymczasowo schowamy tutaj adres IP
	memset(tmp, '\0', sizeof(tmp));

	initNetinfo(&net);
	clilen = sizeof(client);
	net.sock = accept(sockfd, (struct sockaddr *) &client, &clilen);

	ip = client.sin_addr;
	strcpy(tmp, inet_ntoa(ip));

	ip_len = strlen(tmp) + 1;
	net.ipaddr = (char * ) malloc(ip_len * sizeof(char));
	strcpy(net.ipaddr, tmp);

	return net;
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
	char * clientresp = NULL;		// odpowiedz klienta
	size_t resplen = 0;                             // dlugosc stringu przeslanego przez klienta
	int i;						// index bufora

	// przygotowujemy bufor
	char buff[NET_BUFFER];
	memset(buff, '\0', NET_BUFFER);

	if(read(sockfd, buff, sizeof(buff)) > 0) {
		resplen = strlen(buff) + 1;
		clientresp = (char *) malloc(resplen * sizeof(char));
		memset(clientresp, '\0', resplen);

		for(i = 0; i < resplen; i++)
			clientresp[i] = buff[i];
		clientresp[i-1] = '\0';		// pozbywam sie nowej linii

	}
	return clientresp;
}
int SendPackage(int sockfd, char * message) {
  int bytesSent = 0;

  // Przygotowanie bufora i skopiowanie do niego danych
  char buff[NET_BUFFER];
  memset(buff, '\0', NET_BUFFER);
  strncpy(buff, message, NET_BUFFER);

  if((bytesSent = write(sockfd, buff, NET_BUFFER)) < 1)
    return 0;

  return bytesSent;
}
int waitForHEllo(int sockfd) {
	int status = 0;
	char buff[NET_BUFFER];
	memset(buff, '\0', NET_BUFFER);

	if(read(sockfd, buff, NET_BUFFER) > 1) {
		if(strstr(buff, "200 Go Ahead") != NULL)
			status = 1;
	}
	return status;
}
void initNetinfo(netinfo * net) {
	net->ipaddr = NULL;
	net->sock = 0;
}
char * getExternalIP(void) {
	char * extip = NULL;
	char * extip_pos = NULL;
	char * strpos = NULL;
	size_t iplen = 0;
	char * apihost = "api.angrybits.pl";
	int apiport = 80;
	char buff[256];
	memset(buff, '\0', 256);

	char * request = mkString("GET /ip.php HTTP/1.1\n",
							"Host: ", apihost, "\n\n", NULL);
	char * lastHeader = "Content-Type: text/html";

	int apifd = connector(apihost, apiport);

	if(apifd < 0)
		return NULL;

	memset(buff, '\0', 256);
	strcpy(buff,request);

	if(write(apifd, buff, 256) < 0)
		return NULL;

	memset(buff, '\0', 256);
	if(read(apifd, buff, 256) < 0)
		return NULL;

	close(apifd);

	strpos = strstr(buff, lastHeader);
	strpos = strpos + strlen(lastHeader);
	iplen = strlen(strpos);
	extip = (char *) malloc(iplen * sizeof(char));
	extip_pos = extip;
	memset(extip, '\0', iplen);
	while(*strpos) {
		if(*strpos != '\n' && *strpos != '\r') {
				*extip_pos = *strpos;
				extip_pos++;
		}
		strpos++;
	}
	return extip;
}
