#ifndef SPINE_AGENT_SRC_NETWORK_H_
#define SPINE_AGENT_SRC_NETWORK_H_

#define CONN_QUEUE 10		// dlugosc kolejki polaczen do obsluzenia

/*			FUNKCJE			*/

// funkcja prowadzi nasluch na zadanym porcie.
// zwraca deskryptor polaczenia.
int listener(int port);

#endif /* SPINE_AGENT_SRC_NETWORK_H_ */
