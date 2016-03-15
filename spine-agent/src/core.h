#ifndef SPINE_AGENT_CORE_H_
#define SPINE_AGENT_CORE_H_
#include "sysconfigdata.h"

#define BUFSIZE 128			// bufor do odczytu pliku konfiguracyjnego
#define PACKAGE_SIZE 1024 	// bufor dla informacji konfiguracyjnych

/* 			STRUKTURY		*/

// dane konfiguracyjne
typedef struct config_data {
	char * mode;		// tryb pracy agenta
	char * logpath;		// sciezka do pliku z logami
	char * host;		// nazwa DNS procesu odbierajacego dane
	int port;			// port TCP na ktorym dziala usluga
} config_data;


/* 			FUNKCJE			*/

// Inicjalizacja struktury config_data
void InitConfigData(config_data * cfd);

// funkcja ogolnego przeznaczenia, ktora sluzy do
// laczenia stringow. Ostatnim argumentem musi byc NULL
char * mkString(char * qstr, ...);

// funkcja zwraca aktualny czas w formacie: YYY-MM-DD hh:mm:ss
char * timestamp(void);

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
void RetrieveData(int port, FILE *lf);

// funckja wysyła dane. Jest w stanie przeslac dane zarowno od serwera
// jak i od klienta
void SendData(char * mode, char * server, int port, FILE * lf);

// funkcja buduje string w formacie json zawierajacy dane systemowe klienta
char * BuildPackage(systeminfo * info);

/* zwalnia pamiec po skladowych jsona stanowiacego pakiet danych klienta
   pierwszy argument to tablica przechowujaca poszczegolne czesci stringa
   druga to liczba elementow tablicy */
void cleanChunks(char * parts[], int n);

#endif /* SPINE_AGENT_CORE_H_ */
