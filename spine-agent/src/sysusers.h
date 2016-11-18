#ifndef SYSUSERS_H
#define SYSUSERS_H

// inicjalizacja struktury przechowujacej dane uzytkownikow
void sysUsersInit(sysuser * su);

// funkcja wyciaga liste kluczy ssh aktualnie przetwarzanego uzytkownika
char * sshkeysPackage(sshkeys * k);

#endif /* SYSUSERS_H */

