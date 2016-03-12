#ifndef SPINE_AGENT_CORE_H_
#define SPINE_AGENT_CORE_H_

// funkcja ogolnego przeznaczenia, ktora sluzy do
// laczenia stringow. Ostatnim argumentem musi byc NULL
char * mkString(char * qstr, ...);

// funkcja zwraca aktualny czas w formacie: YYY-MM-DD hh:mm:ss
char * timestamp(void);

#endif /* SPINE_AGENT_CORE_H_ */
