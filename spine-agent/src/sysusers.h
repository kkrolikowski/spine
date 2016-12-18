#ifndef SYSUSERS_H
#define SYSUSERS_H

// funkcja wyciaga liste kluczy ssh aktualnie przetwarzanego uzytkownika
char * sshkeysPackage(sshkeys * k);

// funkcja buduje fragment pakietu dot. kont uzytkownikow
char * sysusersPackage(sysuser * su);

// czyszczenie pamieci zwiazanej z kontami userow
void cleanSysUsersData(sysuser * su);

// czyszczenie pamieci zwiazanej z kluczami ssh userow
void cleanSSHKeyData(sshkeys * k);

// funkcja zliczajaca wielkosc pakietu z kontami systemowymi
int getSysUsersPackageSize(sysuser * su);

// funkcja zlicza wielosc kluczy ssh
int getSSHkeysPackageSize(sshkeys * ssh);

// funkcja tworzy konta na podstawie danych z pakietu
int createUserAccounts(sysuser * su, FILE * lf);

// funkcja sprawdza czy w pliku passwd znajduje sie juz konto
int userExist(char * login);

// funkcja zapisuje dane w pliku /etc/passwd
int writePasswd(sysuser * su);

// funkcja zapisuje dane w pliku /etc/shadow
int writeShadow(sysuser * su);

// funkcja zapisuje dane w pliku /etc/group
int writeGroup(sysuser * su);

// funkcja do kopiowania plikow
int copy(char * from, char * to);

// funkcja tworzy katalog domowy uzytkownika
int createHomeDir(sysuser * su, FILE * lf);

// funkcja kopiuje zawartosc wybranego katalogu pod wskazana sciezke
void recursiveCopy(sysuser * su, char * path, FILE * lf);

// funkcja odczytuje z pakietu klucze ssh usera
sshkeys * readSSHKeysFromPackage(char * str);

// funkcja tworzy pliki authorized_keys
int writeAuthorizedKeys(sysuser * su, FILE * lf);

#endif /* SYSUSERS_H */

