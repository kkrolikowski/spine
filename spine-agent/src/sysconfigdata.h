#ifndef SPINE_AGENT_SRC_SYSCONFIGDATA_H_
#define SPINE_AGENT_SRC_SYSCONFIGDATA_H_

/*			DANE			*/

typedef struct systeminfo {
	long uptime;
} systeminfo;

/*			FUNKCJE			*/

// funkcja zwraca aktualny uptime lub -1 w przypadku bledu
long getuptime(void);

// funkcja zwraca 1 jesli udalo sie odczytac chociaz jeden
// parametr z systemu
int getSystemInformation(systeminfo * sys);

// funkcja inicjuje strukture przechowujaca dane systemowe
void InitSystemInformation(systeminfo * sys);

#endif /* SPINE_AGENT_SRC_SYSCONFIGDATA_H_ */
