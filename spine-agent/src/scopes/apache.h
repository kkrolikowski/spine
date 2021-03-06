#ifndef SPINE_AGENT_SRC_APACHE_H_
#define SPINE_AGENT_SRC_APACHE_H_
#include "core.h"
#include "sysconfigdata.h"
#include "commondata.h"

/*			FUNKCJE			*/

// tworzenie pliku htpasswd
resp * createHtpasswdFile(htpasswdData * htp, char * path, FILE * lf, resp * rdata);

resp * HtpasswdSetup(htpasswdData * htp, char * os, FILE * lf, resp * rdata);

int updateHtgroupFile(char * authDir, vhostData * vhd);

// funkcja konfiguruje konta i grupy do uwierzytelaninia userow poprzez apacza
void apacheAuthConfig(char * os, vhostData * vhd, FILE * lf);

// funkcja konfiguruje opcje order apacza na podstawie informacji z bazy
char * accessOrder(char * str);

// funkcja wywoluje skrypt do przeladowania konfiguracji apacza
void reloadApache(char * os);

// ogolna funkcja do konfiguracji apacza
resp * updateApacheSetup(httpdata www, char * os, FILE * lf);

// funkcja tworzy pliki htaccess w katalogach stron www jesli wartosc htaccess jest rozna
// od NaN
void createHtaccess(char * htaccessPath, char * hta_content);

// funkcja tworzy Katalogi w ktorych beda znajdowac sie strony www
void createWebsiteDir(vhostData * vh, FILE * lf);

// funkcja tworzy pliki z konfiguracja virtualek apacza. Polozenie
// plikow jest uzaleznione od dystrybucji linuksa
int createVhostConfig(char * distro, vhostData * vhd, FILE * lf);

// function removes authdir only if there's no htpasswd and htgroup files inside
void clearAuthData(char * os);

// funkcja kasuje konfiguracje oraz zawartosc stron www. Zwraca liczbe skasowanych witryn
void removeVhost(char * os, vhostData * vhd);

// removing entry fron .htgroup
int removeFromHtGroupFile(char * path, char * entry);

// function converts string (example: 192.168.1.1:1#all:0) into apache
// allow/deny from string.
char * vhostACL(char * str);

// function creates new entry at the end of .htpasswd file
int createHtpasswdEntry(htpasswdData * data, char * path);

// function updates .htpasswd file with a given data
int updateHtpasswdEntry(htpasswdData * data, char * path);

// function removes given entry from .htpasswd file
int deleteHtpasswdEntry(htpasswdData * data, char * path);

#endif /* SPINE_AGENT_SRC_APACHE_H_ */
