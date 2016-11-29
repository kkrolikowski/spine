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

#endif /* SYSUSERS_H */

