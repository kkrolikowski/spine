#ifndef SPINE_AGENT_SRC_SYSCONFIGDATA_H_
#define SPINE_AGENT_SRC_SYSCONFIGDATA_H_

/*			DANE			*/

// podstawowe informacje o systemie
typedef struct systeminfo {
	long uptime;
} systeminfo;

// konfiguracja serwera www
typedef struct wwwdata {
	char * ServerName;
	char * ServerAlias;
	char * DocumentRoot;
} wwwdata;

/*			FUNKCJE			*/

// funkcja zwraca aktualny uptime lub -1 w przypadku bledu
long getuptime(void);

// funkcja zwraca 1 jesli udalo sie odczytac chociaz jeden
// parametr z systemu
int getSystemInformation(systeminfo * sys);

// funkcja inicjuje strukture przechowujaca dane systemowe
void InitSystemInformation(systeminfo * sys);

#endif /* SPINE_AGENT_SRC_SYSCONFIGDATA_H_ */
