#ifndef SPINE_AGENT_SRC_SYSCONFIGDATA_H_
#define SPINE_AGENT_SRC_SYSCONFIGDATA_H_

#define VERSION_FILE "/var/spool/spine-agent.dat"
#define VHOST_MAX 200		// limit vhostow na serwerze

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

// konfiguracja serwera www
typedef struct wwwdata {
	char * ServerName;				// glowny adres witryny
	char * ServerAlias;				// Dodatkowe adresy witryny
	char * DocumentRoot;			// katalog witryny na serwerze
	char * apacheOpts;				// opcje katalogu
	char * htaccess;				// definicja pliku htaccess
	char * vhost_access_order;		// kolejnosc przetwarzania regol dostepu
	char * vhost_access_list;		// lista hostow skad mozna laczyc sie z witryna
	int password_access;			// czy witryna ma byc zabezpieczona haslem
	char * htusers;					// lista kont, ktora ma dostep do witryny
	char * user;					// wlasciciel witryny
} wwwdata;

typedef struct htpasswdData {
	char * entry;
	struct htpasswdData * next;
} htpasswdData;

// struktura, ktora bedzie przechowywac wskazniki
// do poszczegolnych elementow pakietu.
typedef struct hosconfig {
	struct wwwdata vhost[VHOST_MAX];	// definicja vhostow
	int vhost_num;						// liczba skonfigurowanych vhostow
	htpasswdData * htpasswd;			// zawartosc pliku htpasswd
	int htusers_count;					// liczba kont htpasswd
	int confVer;						// wersja konfiguracji
	char * datatype;					// typ konfiguracji
} hostconfig;

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

// funkcja tworzy pliki z konfiguracja virtualek apacza. Polozenie
// plikow jest uzaleznione od dystrybucji linuksa
int createVhostConfig(char * distro, wwwdata vhosts[], int n, FILE * lf);

// funkcja tworzy Katalogi w ktorych beda znajdowac sie strony www
void createWebsiteDir(wwwdata vhosts[], int n);

// funkcja tworzy strukture katalogow na podstawie podanej sciezki
void mkdirtree(char * path);

// ogolna funkcja do konfiguracji apacza
void apacheSetup(hostconfig cfg, char * os, FILE * lf);

// funkcja wywoluje skrypt do przeladowania konfiguracji apacza
void reloadApache(char * os);

// funkcja tworzy pliki htaccess w katalogach stron www jesli wartosc htaccess jest rozna
// od NaN
void createHtaccess(wwwdata vhosts[], int n);

// funckcja generuje pojedynczy wpis w configu apacza Deny from xxx lub Allow from xxx
char * apache_accesslist_entry(char * str);

// funckcja zwraca kompletny fragment konfiguracji allow/deny apacza
char * acl(char * str);

// funkcja konfiguruje opcje order apacza na podstawie informacji z bazy
char * accessOrder(char * str);

// funkcja tworzy plik htgroup
void createHtgroupConfig(char * os, wwwdata vhosts[], int n, FILE * lf);

void createHtgroupFile(char * path, wwwdata vhosts[], int n);

// odczyt danych tekstowych do struktury danych
htpasswdData * parseHtpasswdData(char * stream);

// tworzenie pliku htpasswd
void createHtpasswdFile(char * os, htpasswdData * htpasswd);

#endif /* SPINE_AGENT_SRC_SYSCONFIGDATA_H_ */
