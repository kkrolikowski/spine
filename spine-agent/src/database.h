#ifndef SPINE_AGENT_SRC_DATABASE_H_
#define SPINE_AGENT_SRC_DATABASE_H_
#include <mysql.h>
#include "commondata.h"

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

// funkcja pobiera string wejsciowy alokuje pamiec i przepisuje jego
// zawartosc do zaalokowanego obszaru pamieci.
char * readData(char * input);

// funkcja zwraca ID rekordu z bazy zawierajacego okreslony mac-adres serwera
int getDBHostID(char * hwaddr);

// funkcja sprawdza, czy usluga na danym hoscie zostala zarejestrowana
// 0 - usluga nie zarejestrowana
// 1 - usluga istnieje
// -1 - wystapil blad bazy danych
int s_exist(char * s, char * systemid);

// funkcja wykonuje insert z nowym rekordem do bazy danych
int s_insert(char * srv, char * state, char * hostid);

// funkcja aktualizuje status uslug
int s_update(char * srv, char * state, char * hostid);

// funkcja aktualizuje status uslug
void updateServiceState(char * cliresp);

// funkcja sluzy do aktualizacji tabelki z danymi na temat ruchu sieciowego
void insertNetworkData(char * bytes_in, char * bytes_out, char * time_stmp, char * hostid);

// function updates db records with information provided from client
int applyStatusChange(resp * data);

// activate item in db table
void activate(char * scope, int id);

// deletes item from db table
void delete(char * scope, int id);

#endif /* SPINE_AGENT_SRC_DATABASE_H_ */
