#ifndef SPINE_AGENT_SRC_DATABASE_H_
#define SPINE_AGENT_SRC_DATABASE_H_
#include <mysql.h>

/*			STRUKTURY			*/

// struktura przechowujaca informacje potrzebne
// do wykonania polaczenia do bazy danych
typedef struct dbconn {
	char * dbhost;
	char * dbname;
	char * dblogin;
	char * dbpass;
} dbconn;

/*			FUNKCJE				*/

// funkcja inicjuje zmienne przechowujace parametry
// polaczenia z baza danych
void InitDBConnData(dbconn db);

// funkcja kasuje z pamieci parametry polaczenia
// do bazy danych
void clearDBConnData(dbconn db);

// funkcja nawiazuje polaczenie z baza danych.
// jesli sie jej uda zwraca uchwyt do bazy, jesli nie: NULL
MYSQL * dbConnect(dbconn cfg);

// funkcja sprawdza, czy host istnieje w bazie
int hostExist(char * hostid);

// funkcja aktuliazuje (badz dodaje) informacje przesylane przez
// klienta
int updateHostInfo(char * clientip, char * stream, FILE * lf);

// funkcja aktualizuje rekord w bazie na podstawie identyfikatora
// systemu (mac-adres). Zwraca 1 jesli update sie udal a 0 jesli
// wystapil problem
int updateItem(systeminfo * info);

// funkcja dodaje rekord do bazy. Zwraca 1 jesli sie udalo,
// 0 w przypadku niepowodzenia
int insertItem(systeminfo * info);

// funkcja zwraca wersje konfiguracji danego hosta
int checkDBConfigVer(char * systemid);

#endif /* SPINE_AGENT_SRC_DATABASE_H_ */
