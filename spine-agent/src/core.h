#ifndef SPINE_AGENT_CORE_H_
#define SPINE_AGENT_CORE_H_
#include "sysconfigdata.h"
#include "database.h"
#include "monitoring.h"

#define BUFSIZE 128			// bufor do odczytu pliku konfiguracyjnego
#define PACKAGE_SIZE 10240 	// bufor dla informacji konfiguracyjnych
#define PID_PATH "/var/run/spine-agent.pid"		// sciezka do pliku z pidem glownego procesu
#define HEARTBEAT 2			// interwal czasu pomiedzy kolejnymi iteracjami wykonywania funkcji

/* 			STRUKTURY		*/

// dane konfiguracyjne
typedef struct config_data {
	char * mode;		// tryb pracy agenta
	char * logpath;		// sciezka do pliku z logami
	char * host;		// nazwa DNS procesu odbierajacego dane
	int port;			// port TCP na ktorym dziala usluga
	dbconn dbinfo;	// namiary na baze danych
} config_data;

// wersje w poszczegolnych kategoriach
typedef struct ver {
    char * scope;       // obszar konfiguracyjny
    int version;        // wersja konfiguracji
    struct ver * next;  //  nastepny obszar konfugyracyjny
} ver;

/* 			FUNKCJE			*/

// funkcja zapisuje pid glownego procesu w pliku
int savePidFile(int pid);

// Inicjalizacja struktury config_data
void InitConfigData(config_data * cfd);

// funkcja zapisująca informacje do wskazanego przez uchwyt lf
// pliku z logami.
void writeLog(FILE *lf, char * message);

// funkcja sprawdza czy program jest uruchomiony z uprawnieniami roota
// zwraca 0 jesli uzytkownik nie jest rootem
int norootUser(void);

// Funkcja zapisuje w strukturze config_data dane z pliku cf
// Zwraca 1 jeśli uda się poprawnie odczytać chociaz jedna opcje
int ReadConfig(config_data * cfd, FILE * cf);

// Funkcja wyciaga wartosci opcji, ktore sa skonfigurowane w pliku
char * parseLine(char * line);

// Funkcja odbiera dane z sieci a nastepnie za pomoca innych funkcji
// bedzie je przetwarzac.
void RetrieveData(int port, char * mode, FILE *lf);

// funckja wysyła dane. Jest w stanie przeslac dane zarowno od serwera
// jak i od klienta
void SendData(char * mode, char * server, int port, FILE * lf);

// funkcja buduje string w formacie json zawierajacy dane systemowe klienta
char * BuildPackage(systeminfo * info, monitoring * s_state, netifstats * n_stats);

// funkcja sprawdza czy trzeba wyslac konfiguracje do klienta
int clientNeedUpdate(char * clientData);

// funkcja kumuluje konfigi w jednego jsona
char * buildConfigPackage(hostconfig * data);

// funkcja sluzy do pobierania z bazy konfiguracji hostow
int ReadHostConfig(char * hostid, hostconfig * conf, ver * cfgver, int clientver, FILE * lf);

// funkcja odczytuje wersje konfiguracji w poszczegolnych obszarach
ver * checkVersions(char * systemid);

// funkcja zwraca najwieksza wartosc w tablicy
int maxver(int vers[], int n);

// funkcja zwraca numer wersji, ktory zostanie zapisany przez klienta w pliku
int readPackageVersion(char * str);

// fucntion creates short string package containg information what ID to update in
// database
char * backMessage(resp * rsp);

// function reads to memory data from a given string
resp * parseClientMessage(char * str);

// memory cleanup
void cleanMSGdata(resp * rsp);

// function creates single node of response status message.
resp *  respStatus(char * scope, char status, int dbid);

// function reads key value from json string. Value should be enclosed between
// double quotes.
char * getOptVal(char * json, char * key);

#endif /* SPINE_AGENT_CORE_H_ */
