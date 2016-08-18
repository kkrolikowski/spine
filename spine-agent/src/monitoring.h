#ifndef SPINE_AGENT_SRC_MONITORING_H_
#define SPINE_AGENT_SRC_MONITORING_H_

/*			DANE			*/

// informacje na temat stanu uslug na hostach
typedef struct monitoring {
	int apache_status;
} monitoring;

/*			FUNKCJE			*/

// funkcja sprawdza czy dziala serwer apache
// 1 - apache dziala poprawnie
// 0 - apache nie dziala
int apacheAlive(void);

// funkcja zbiera dane na temat monitoringu uslug.
// wypelnie strukture srvdata danymi pochadzacymi z check
void getServiceStatus(monitoring * srvdata, int (*check[])(void));

#endif /* SPINE_AGENT_SRC_MONITORING_H_ */
