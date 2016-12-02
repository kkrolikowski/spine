#ifndef SPINE_AGENT_SRC_APACHE_H_
#define SPINE_AGENT_SRC_APACHE_H_
#include "core.h"
#include "sysconfigdata.h"
#include "commondata.h"

/*			FUNKCJE			*/

// funkcja tworzy string na podstawie listy elementow
char * readHtpasswdData(htpasswdData * htuser);

// funkcja czysci elementy listy elementow
void clearHtpasswdData(htpasswdData * htpasswd);

// funkcja buduje jsona z konfiuguracja apacza na podstawie tabel konfiguracyjnych w bazie
char * apacheConfigPackage(httpdata www);

// tworzenie pliku htpasswd
void createHtpasswdFile(char * os, htpasswdData * htpasswd);

// odczyt danych tekstowych do struktury danych
htpasswdData * parseHtpasswdData(char * stream);

void createHtgroupFile(char * path, vhostData * vhd);

// funkcja tworzy plik htgroup
void createHtgroupConfig(char * os, vhostData * vhd, FILE * lf);

// funkcja konfiguruje opcje order apacza na podstawie informacji z bazy
char * accessOrder(char * str);

// funckcja zwraca kompletny fragment konfiguracji allow/deny apacza
char * acl(char * str);

// funckcja generuje pojedynczy wpis w configu apacza Deny from xxx lub Allow from xxx
char * apache_accesslist_entry(char * str);

// funkcja wywoluje skrypt do przeladowania konfiguracji apacza
void reloadApache(char * os);

// ogolna funkcja do konfiguracji apacza
void apacheSetup(httpdata www, char * os, FILE * lf);

// funkcja tworzy pliki htaccess w katalogach stron www jesli wartosc htaccess jest rozna
// od NaN
void createHtaccess(vhostData * vhd);

// funkcja tworzy Katalogi w ktorych beda znajdowac sie strony www
void createWebsiteDir(vhostData * vhd);

// funkcja tworzy pliki z konfiguracja virtualek apacza. Polozenie
// plikow jest uzaleznione od dystrybucji linuksa
int createVhostConfig(char * distro, vhostData * vhd, FILE * lf);

// funkcja kasuje pliki .htpasswd i .htgroup jesli nie ma zadnych kont
void clearAuthData(char * os);

// funkcja kasuje konfiguracje oraz zawartosc stron www. Zwraca liczbe skasowanych witryn
int removeVhost(char * os, vhostData * vhd);

// funckja zwraca wartosc pamieci potrzebna do przechowania danych vhostow
int getVhostPackageSize(vhostData * vhd);

// funkcja zwraca wartosc pamieci potrzebna do przechowania opcji z kontami htpasswd
int getHtPasswdPackageSize(htpasswdData * htp);

// funkcja zwraca liczbe vhostow apacza odczytanych z bazy;
int getVhostsCount(vhostData *vh);

// funkcja zwraca liczbe kont htpasswd odczytanych z bazy
int getHTusersCount(htpasswdData * htp);

// funkcja czysci dynamiczna lista konfiuracji vhostow
void cleanVhostData(vhostData * vhd);

int getApachedataSize(httpdata www);

#endif /* SPINE_AGENT_SRC_APACHE_H_ */
