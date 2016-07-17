#ifndef SPINE_AGENT_SRC_SYSCONFIGDATA_H_
#define SPINE_AGENT_SRC_SYSCONFIGDATA_H_
#include "apache.h"
#include "commondata.h"

#define SPOOL_DIR "/var/spool/spine"
#define VERSION_FILE "/var/spool/spine/spine-agent.dat"

/*			DANE			*/

// podstawowe informacje o systemie
typedef struct systeminfo {
	unsigned long uptime;
	unsigned long hdd_total;
	unsigned long hdd_free;
	unsigned long ram_total;
	unsigned long ram_free;
	char * net_hwaddr;
	char * hostname;
	char * os;
	char * ip;
	char * extip;
	int config_version;
} systeminfo;

/*			FUNKCJE			*/

// funkcja zwraca aktualny uptime lub -1 w przypadku bledu
unsigned long getuptime(void);

// funkcja zwraca 1 jesli udalo sie odczytac chociaz jeden
// parametr z systemu
int getSystemInformation(systeminfo * sys, unsigned long (*SysInfo[])(void), int n);

// funkcja inicjuje strukture przechowujaca dane systemowe
void InitSystemInformation(systeminfo * sys);

// funkcja zwalnia pamiec gdzie przechowywane sa informacje systemowe
void ClearSystemInformation(systeminfo * sys);

// funkcja zwraca sformatowany string zawierajacy mac-adres
char * getMacAddress(void);

// funkcja zwraca hostname klienta
char * getHostname(void);

// calkowity rozmiar partycji
unsigned long DiskSizeTotal();

// wolna przestrzena na dysku
unsigned long DiskSizeFree();

// wolna pamiec RAM
unsigned long ramFree();

// calkowita pamiec RAM
unsigned long ramTotal();

// funkcja zwraca nazwe pierwszego interfejsu w systemie
char * getNetInterfaceName();

// funkcja zapisuje do pliku aktualna wersje konfiguracji
// zwraca 1 w przypadku sukcesu
int writeLocalConfigVersion(int ver);

// funkcja odczytuje lokalna wersje konfiguracji. Wartosci zwracane:
// 0 - nowy plik
// > 0 - lokalna wersja konfiguracji
// -1 - nie udalo sie stworzyc nowego pliku
int readLocalConfigVersion(void);

// funkcja na podstawie danych z jsona wypelnia strukture konfiguracyjna
hostconfig ParseConfigData(char * json);

// funkcja zwraca nazwe dystrybucji Linuksa
char * linuxDistro(void);

// funkcja tworzy strukture katalogow na podstawie podanej sciezki
void mkdirtree(char * path);

#endif /* SPINE_AGENT_SRC_SYSCONFIGDATA_H_ */
