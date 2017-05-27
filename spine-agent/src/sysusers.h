#ifndef SYSUSERS_H
#define SYSUSERS_H


//function creates user accounts based on package data from server agent
int createUserAccounts(sysuser * su, char * os, FILE * lf);

// funkcja sprawdza czy w pliku passwd znajduje sie juz konto
int userExist(int uid);

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

// function modifies /etc/group file and updated it with given login
// return 1 on success and 0 on failure
int grantSuperUser(char * login, char * os);

// function updated an entry of /etc/group file (buff) with given login
// returns pointer to modified string
char * updateGroup(char * buff, char * login);

// function check if particular account needs to to be updated and
// perform this action
resp * updateUserAccounts(sysuser * su, char * os, FILE * lf, resp * respdata);

// function updates /etc/passwd file with current data
int updatePasswd(sysuser * su);

// function return old login if new differs, if not - return NULL
char * oldlogin(int uid, char * new);

// function updates /etc/shadow file
int updateShadow(sysuser * su, char * login);

// function changes oldlogin to newlogin in a given entry
char * changeLogin(char * entry, char * oldlogin, char * newlogin);

// function changes oldlogin to newlogin in /etc/group file
int updateGroupFile(sysuser * su, char * oldlogin);

// removes login from group entry and returns new entry,
// return NULL if there is no login in a given entry
char * rmFromGrp(char * entry, char * login);

// revokes Admin access for given login
int revokeSudoAccess(char * login, char * os);

// functions renames homedir
int renameHomeDir(char * olduser, char * newuser);

// function checks if login is present in sudo (wheel) group
int isAdmin(char * os, char * login);

// Function disables given sysuser account
int disableAccount(char * login);

// function returns changed string with ! mark
char * lockEntry(char * entry);

// function upadtes information about sshkeys for particular user
int updateSSHKeys(sysuser * su, FILE * lf);

// function remove entry with a given login with a given system file
int removeFromSystemFile(char * login, char * systemFile);

#endif /* SYSUSERS_H */

