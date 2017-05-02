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
    int dbid;               // id z bazy danych
    char * login;           // login uzytkownika
    char * sha512;          // haslo zaszyfrowane algorytmem SHA-512
    char * gecos;           // opis konta (GECOS)
    int active;             // czy konto jest aktywne
    int uidgid;             // UID oraz GID uzytkownika
    int shellaccess;        // czy konto ma miec dostep do shella
    int expiration;         // data expiracji konta (epoch + dni)
    sshkeys * sshkey;       // wszystkie klucze ssh uzytkownika
    int version;            // wersja konfiguracji
    char * status;          // status wpisu
    int sudo;               // czy konto ma miec dostep do roota
    struct sysuser * next;  // wskaznik do kolejnego konta
} sysuser;

/****************************************************/
/*              Sekcja konfiguracji serwera www     */
/****************************************************/

// lista zawierajaca konta htpasswd
typedef struct htpasswdData {
    char * login;                   // login
    char * pass;                    // password
    int dbid;                       // database ID
    char status;                    // account status
    int version;                    // config version
    struct htpasswdData * next;     // next entry
} htpasswdData;

// konfiguracja vhostow
typedef struct vhostData {
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
    int version;                // wersja konfiguracji
    int dbid;                   // id rekordu z bazy danych
    int uid;                    // UNIX UID wlasciciela vhosta
    struct vhostData * next;    // wskaznik do kolejnego vhosta
} vhostData;

// db grants info
typedef struct grants {
    int dbid;               // database uniqe ID
    char status;            // status (N.U.D.A. scheme)
    char * user;            // database login
    char * dbname;          // database name
    char * privs;           // granted privileges list
    struct grants * next;   // next privilege
} grants;

// database users information
typedef struct dbuser {
    int dbid;               // database uniqe ID
    char status;            // status (N.U.D.A. scheme)
    char * login;           // database login
    char * pass;            // database password
    struct dbuser * next;   // next 
} dbuser;

// database information
typedef struct dbinfo {
    int dbid;               // database unique ID
    char status;            // item status (N.U.D.A. scheme)
    char * dbname;          // name of database
    struct dbinfo * next;
} dbinfo;

// generic type for gather all database access aspects.
typedef struct rdbs {
    dbinfo * db;
    dbuser * dbusers;
    grants * dbgrants;
} rdbs;

// ogolna struktura danych przechowujaca konfiguracje serwera www
typedef struct httpdata {
    vhostData * vhost;          // vhosty apacza
    htpasswdData * htpasswd;    // konta htpasswd
    int version;                // wersja konfiguracji
} httpdata;

/****************************************************/
/*          Ogolna konfiguracja systemu             */
/****************************************************/

// struktura przechowujaca konfiguracje hosta
typedef struct hostconfig {
    sysuser * sysUsers;     // konta uzytkownikow w systemie
    httpdata httpd;         // konfiguracja serwera www
    rdbs sqldb;           // dostepy do baz danych
    char * datatype;        // typ: hostconfig (server) lub sysinfo (client)
} hostconfig;

/****************************************************/
/*             Comunication protocol                */
/****************************************************/

// update message to server agent. Provides information what to do
// in database with particular record ID. 
typedef struct resp {
    char status;            // status type: U - update, D - delete
    char * scope;           // possible values: sysusers, vhosts
    int dbid;               // ID of particular db record
    struct resp * next;     // next item
} resp;

#endif /* SPINE_AGENT_SRC_COMMONDATA_H_ */
