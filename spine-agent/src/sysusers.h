#ifndef SYSUSERS_H
#define SYSUSERS_H

// inicjalizacja struktury przechowujacej dane uzytkownikow
void sysUsersInit(sysuser * su);

// funkcja wyciaga liste kluczy ssh aktualnie przetwarzanego uzytkownika
char * sshkeysPackage(sshkeys * k);

// funkcja buduje fragment pakietu dot. kont uzytkownikow
char * sysusersPackage(sysuser * su);

#endif /* SYSUSERS_H */

