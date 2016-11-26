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
	char * status;					// flaga: A - vhost aktywny, D - do skasowania
	char * purgedir;					// flaga: Y - kasujemy pliki aplikacji, N - zostawiamy
} wwwdata;

typedef struct htpasswdData {
	char * entry;
	struct htpasswdData * next;
} htpasswdData;

// struktura przechowujaca klucze ssh uzytkownika
typedef struct sshkeys {
    char * key;             // string z kluczem ssh
    struct sshkeys * next;  // wskaznik do kolejnego klucza
} sshkeys;

// struktura przechowujaca wszystkie wlasciwosci konta uzytkownika w systemie
typedef struct sysuser {
    char * login;           // login uzytkownika
    char * sha512;          // haslo zaszyfrowane algorytmem SHA-512
    char * gecos;           // opis konta (GECOS)
    int active;             // czy konto jest aktywne
    int uidgid;             // UID oraz GID uzytkownika
    int shellaccess;        // czy konto ma miec dostep do shella
    int expiration;         // data expiracji konta (epoch + dni)
    sshkeys * sshkey;       // wszystkie klucze ssh uzytkownika
    struct sysuser * next;  // wskaznik do kolejnego konta
} sysuser;

// nowe struktury danych opisujace konfiguracje serwera www

// konfiguracja vhostow
typedef struct vhostdata {
    char * ServerName;          // glowny adres witryny
    char * ServerAlias;		// Dodatkowe adresy witryny
    char * DocumentRoot;	// katalog witryny na serwerze
    char * apacheOpts;		// opcje katalogu
    char * htaccess;		// definicja pliku htaccess
    char * vhost_access_order;	// kolejnosc przetwarzania regol dostepu
    char * vhost_access_list;	// lista hostow skad mozna laczyc sie z witryna
    int password_access;	// czy witryna ma byc zabezpieczona haslem
    char * htusers;		// lista kont, ktora ma dostep do witryny
    char * user;		// wlasciciel witryny
    char * status;		// flaga: A - vhost aktywny, D - do skasowania
    char * purgedir;		// flaga: Y - kasujemy pliki aplikacji, N - zostawiamy
    struct vhostdata * next;    // wskaznik do kolejnego vhosta
} vhostdata;

typedef struct httpdata {
    vhostdata * vhost;          // vhosty apacza
    htpasswdData * htpasswd;    // konta htpasswd
} httpdata;

// struktura, ktora bedzie przechowywac wskazniki
// do poszczegolnych elementow pakietu.
typedef struct hostconfig {
    struct wwwdata vhost[VHOST_MAX];    // definicja vhostow
    int vhost_num;                      // liczba skonfigurowanych vhostow
    htpasswdData * htpasswd;            // zawartosc pliku htpasswd
    sysuser * sysUsers;                 // konta uzytkownikow w systemie
    httpdata httpd;                     // konfiguracja serwera www
    int htusers_count;                  // liczba kont htpasswd
    int confVer;                        // wersja konfiguracji
    char * datatype;                    // typ konfiguracji
} hostconfig;

#endif /* SPINE_AGENT_SRC_COMMONDATA_H_ */
