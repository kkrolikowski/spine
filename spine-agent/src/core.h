#ifndef SPINE_AGENT_CORE_H_
#define SPINE_AGENT_CORE_H_

#define BUFSIZE 128		// bufor do odczytu pliku konfiguracyjnego

/* 			STRUKTURY		*/

// dane konfiguracyjne
typedef struct config_data {
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
#endif /* SPINE_AGENT_CORE_H_ */