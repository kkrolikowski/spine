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

#endif /* SYSUSERS_H */

