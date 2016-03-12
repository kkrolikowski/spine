#ifndef SPINE_AGENT_SRC_NETWORK_H_
#define SPINE_AGENT_SRC_NETWORK_H_

#define CONN_QUEUE 10		// dlugosc kolejki polaczen do obsluzenia
#define NET_BUFFER 1024		// bufor do wysylania oraz odbierania danych

/*			FUNKCJE			*/

// funkcja prowadzi nasluch na zadanym porcie.
// zwraca deskryptor polaczenia.
int listener(int port);

// funkcja zwraca deskryptor polaczenia
// klienta lub -1 w przyapdku bledu
int clientConnection(int sockfd);

// wysyla do klienta wiadomosc powitalna,
// zwraca liczbe bajtow, ktora udalo sie przeslac
int GreetClient(int sockfd);

// funkcja odczytujaca dane z socketu.
// zwraca string z bufora
char * readClientData(int sockfd);
#endif /* SPINE_AGENT_SRC_NETWORK_H_ */
