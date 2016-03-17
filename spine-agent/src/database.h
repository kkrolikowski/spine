#ifndef SPINE_AGENT_SRC_DATABASE_H_
#define SPINE_AGENT_SRC_DATABASE_H_

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
void InitDBConnData(dbconn * db);

// funkcja kasuje z pamieci parametry polaczenia
// do bazy danych
void clearDBConnData(dbconn * db);

#endif /* SPINE_AGENT_SRC_DATABASE_H_ */
