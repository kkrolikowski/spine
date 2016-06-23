#ifndef SPINE_AGENT_SRC_COMMONDATA_H_
#define SPINE_AGENT_SRC_COMMONDATA_H_

#define VHOST_MAX 200		// limit vhostow na serwerze

/*			DANE			*/
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

#endif /* SPINE_AGENT_SRC_COMMONDATA_H_ */
