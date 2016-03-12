#ifndef SPINE_AGENT_CORE_H_
#define SPINE_AGENT_CORE_H_

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

#endif /* SPINE_AGENT_CORE_H_ */
