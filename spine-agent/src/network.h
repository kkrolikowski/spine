#ifndef SPINE_AGENT_SRC_NETWORK_H_
#define SPINE_AGENT_SRC_NETWORK_H_

#define CONN_QUEUE 10		// dlugosc kolejki polaczen do obsluzenia
#define NET_BUFFER 1024		// bufor do wysylania oraz odbierania danych

/*			STRUKTURY		*/

typedef struct netinfo {
	int sock;
	char * ipaddr;
} netinfo;

/*			FUNKCJE			*/

// funkcja prowadzi nasluch na zadanym porcie.
// zwraca deskryptor polaczenia.
int listener(int port);

// funkcja zwraca strukture polaczenia klienta
netinfo clientConnection(int sockfd);

// wysyla do klienta wiadomosc powitalna,
// zwraca liczbe bajtow, ktora udalo sie przeslac
int GreetClient(int sockfd);

// funkcja odczytujaca dane z socketu.
// zwraca string z bufora
char * readClientData(int sockfd);

// funkcja nawiazuje polaczenie z hostem docelowym
int connector(char * host, int port);

// funkcja przesyla zebrane informacje do oddbiorcy
int SendPackage(int sockfd, char * message);

// funkcja sprawdza, czy serwer jest gotowy
int waitForHEllo(int sockfd);

// inicjujemy strukture polaczenia klienta
void initNetinfo(netinfo * net);

#endif /* SPINE_AGENT_SRC_NETWORK_H_ */
