#ifndef SPINE_AGENT_SRC_COMMONDATA_H_
#define SPINE_AGENT_SRC_COMMONDATA_H_

/****************************************************/
/*              Sekcja kont systemowych             */
/****************************************************/

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

/****************************************************/
/*              Sekcja konfiguracji serwera www     */
/****************************************************/

// lista zawierajaca konta htpasswd
typedef struct htpasswdData {
	char * entry;                   // wpis w formie: login:skrothasla 
	struct htpasswdData * next;     // nastepne konto na liscie
} htpasswdData;

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

// ogolna struktura danych przechowujaca konfiguracje serwera www
typedef struct httpdata {
    vhostdata * vhost;          // vhosty apacza
    htpasswdData * htpasswd;    // konta htpasswd
} httpdata;

/****************************************************/
/*          Ogolna konfiguracja systemu             */
/****************************************************/

// struktura przechowujaca konfiguracje hosta
typedef struct hostconfig {
    sysuser * sysUsers;                 // konta uzytkownikow w systemie
    httpdata httpd;                     // konfiguracja serwera www
    int confVer;                        // wersja konfiguracji
} hostconfig;

#endif /* SPINE_AGENT_SRC_COMMONDATA_H_ */
