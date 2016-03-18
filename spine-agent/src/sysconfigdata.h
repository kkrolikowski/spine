#ifndef SPINE_AGENT_SRC_SYSCONFIGDATA_H_
#define SPINE_AGENT_SRC_SYSCONFIGDATA_H_

/*			DANE			*/

// podstawowe informacje o systemie
typedef struct systeminfo {
	long uptime;
	char * net_hwaddr;
	char * hostname;
	char * ip;
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

// funkcja zwalnia pamiec gdzie przechowywane sa informacje systemowe
void ClearSystemInformation(systeminfo * sys);

// funkcja zwraca sformatowany string zawierajacy mac-adres
char * getMacAddress(void);

// funkcja zwraca hostname klienta
char * getHostname(void);
#endif /* SPINE_AGENT_SRC_SYSCONFIGDATA_H_ */
