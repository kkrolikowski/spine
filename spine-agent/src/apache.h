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

// funkcja zwalnia pamiec po prztworzeniu struktury do stringa
void clearConfigData(hostconfig * cfd);

// funckja inicjue strukture hostconfig
void initConfigData(hostconfig * cfd, long vhostnum);

// funkcja zwalnia pamiec pozostala po wczytaniu danych z bazy
void clearVhostData(struct wwwdata vhost[], int n);

// funkcja buduje jsona z konfiuguracja apacza na podstawie tabel konfiguracyjnych w bazie
char * apacheConfigPackage(hostconfig data);

// tworzenie pliku htpasswd
void createHtpasswdFile(char * os, htpasswdData * htpasswd);

// odczyt danych tekstowych do struktury danych
htpasswdData * parseHtpasswdData(char * stream);

void createHtgroupFile(char * path, wwwdata vhosts[], int n);

// funkcja tworzy plik htgroup
void createHtgroupConfig(char * os, wwwdata vhosts[], int n, FILE * lf);

// funkcja konfiguruje opcje order apacza na podstawie informacji z bazy
char * accessOrder(char * str);

// funckcja zwraca kompletny fragment konfiguracji allow/deny apacza
char * acl(char * str);

// funckcja generuje pojedynczy wpis w configu apacza Deny from xxx lub Allow from xxx
char * apache_accesslist_entry(char * str);

// funkcja wywoluje skrypt do przeladowania konfiguracji apacza
void reloadApache(char * os);

// ogolna funkcja do konfiguracji apacza
void apacheSetup(hostconfig cfg, char * os, FILE * lf);

// funkcja tworzy pliki htaccess w katalogach stron www jesli wartosc htaccess jest rozna
// od NaN
void createHtaccess(wwwdata vhosts[], int n);

// funkcja tworzy Katalogi w ktorych beda znajdowac sie strony www
void createWebsiteDir(wwwdata vhosts[], int n);

// funkcja tworzy pliki z konfiguracja virtualek apacza. Polozenie
// plikow jest uzaleznione od dystrybucji linuksa
int createVhostConfig(char * distro, wwwdata vhosts[], int n, FILE * lf);

// funkcja kasuje pliki .htpasswd i .htgroup jesli nie ma zadnych kont
void clearAuthData(char * os);

// funkcja kasuje konfiguracje oraz zawartosc stron www. Zwraca liczbe skasowanych witryn
int removeVhost(char * os, wwwdata vhosts[], int vhostCount);

// funckja zwraca wartosc pamieci potrzebna do przechowania danych vhostow
int getVhostPackageSize(vhostData * vhd);

// funkcja zwraca wartosc pamieci potrzebna do przechowania opcji z kontami htpasswd
int getHtPasswdPackageSize(htpasswdData * htp);

#endif /* SPINE_AGENT_SRC_APACHE_H_ */
