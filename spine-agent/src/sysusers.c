#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commondata.h"
#include "sysusers.h"
#include "core.h"

char * sysusersPackage(sysuser * su) {
    char * package = NULL;              // wskaznik do wynikowego stringu
    char * entry = NULL;                // wkskaznik do pojedynczego elementu
    char * keyentry = NULL;             // wskaznik do kluczy ssh
    int index = 0;                      // licznik kont systemowych
    char * s_index = NULL;              // licznik kont systemowych jako string
    char * s_expval_val = NULL;         // expiracja konta w formie stringu
    char * s_uidgid_val = NULL;         // UID/GID w formie stringu
    char * s_active_val = NULL;         // flaga statusu konta w formie stringu
    char * s_shell_val = NULL;          // flaga dostepu do shella w formie stringu
    sysuser * su_begin = su;            // poczatek wezla
    sysuser * su_curr = su;             // aktualnie przetwarzane konto
    size_t package_len = 0;             // calkowity rozmiar pakietu
    
    // klucze dla wartosci w pakiecie i inne stale elementy
    char * header = "{datatype:sysusers,";
    char * s_user = "user_";
    char * s_username = "username:";
    char * s_password = "password:";
    char * s_gecos = "gecos:";
    char * s_active = "active:";
    char * s_uidgid = "uidgid:";
    char * s_shell = "shell:";
    char * s_expire = "expire:";
    
    if(su == NULL)
        return NULL;
    
    // obliczanie ilosci pamieci potrzebnej do przechowania pakietu
    package_len = getSysUsersPackageSize(su);
    package = (char *) malloc(package_len * sizeof(char));
    memset(package, '\0', package_len);
    
    // resetujemy zmienne i zaczynamy budowac pakiet
    index = 0;
    su_curr = su_begin;
    strncpy(package, header, strlen(header));
    
    while(su_curr) {
        s_index = int2String(index);
        s_expval_val = int2String(su_curr->expiration);
        s_uidgid_val = int2String(su_curr->uidgid);
        s_active_val = int2String(su_curr->active);
        s_shell_val = int2String(su_curr->shellaccess);
        keyentry = sshkeysPackage(su_curr->sshkey);
        
        entry = mkString(s_user, s_index, ":{",
                         s_username, su_curr->login, ",",
                         s_password, su_curr->sha512, ",",
                         s_gecos, su_curr->gecos, ",",
                         s_expire, s_expval_val, ",",
                         s_uidgid, s_uidgid_val, ",",
                         s_active, s_active_val, ",",
                         s_shell, s_shell_val, ",",
                         keyentry, NULL);
        strncat(package, entry, strlen(entry) + 1);
        if(su_curr->next == NULL)
            strncat(package, "}", 2);
        else
            strncat(package, "},", 3);
        
        if(keyentry != NULL)     free(keyentry);
        if(entry != NULL)        free(entry);
        if(s_index != NULL)      free(s_index);
        if(s_expval_val != NULL) free(s_expval_val);
        if(s_uidgid_val != NULL) free(s_uidgid_val);
        if(s_active_val != NULL) free(s_active_val);
        if(s_shell_val != NULL)  free(s_shell_val);
        index++;
        su_curr = su_curr->next;
    }
    strncat(package, "}", 2);
    
    cleanSysUsersData(su);
    return package;
}                                               
                                                
char * sshkeysPackage(sshkeys * k) {
    char * package = NULL;              // wskaznik do wynikowego stringu
    char * entry = NULL;                // wskaznik do pojedynczego wpisu
    int index = 0;                      // licznik kluczy ssh
    char * s_index = NULL;              // licznik kluczy ssh w formie stringu
    sshkeys * k_begin = k;              // poczatek wezla przechowujacego klucze
    sshkeys * k_current = k;            // aktualnie odczytywana wartosc
    size_t len = 0;                     // rozmiar stringow
    size_t keys_len = 0;                // rozmiar stringu wszystkich kluczy
    char * keystr_prefix = "sshkey_";   // prefix stringu zawierajacego klucz
    
    // obliczamy ile bedzie potrzeba pamieci do przechowania wszystkich kluczy
    while(k_current) {
        keys_len += strlen(k_current->key);
        index++;
        k_current = k_current->next;
    }
    
    // obliczamy calkowity rozmiar pamieci przechowujacej string
    // a nastepnie przygotowujemy obszar pamieci
    len = keys_len + (strlen(keystr_prefix) + 2) * index + 1;
    package = (char *) malloc(len * sizeof(char));
    memset(package, '\0', len);
    
    // wracamy na poczatek wezla i resetujemy licznik
    k_current = k_begin;
    index = 0;
    
    // budujemy pakiet z kluczami ssh
    while(k_current) {
        s_index = int2String(index);
        if(k_current->next == NULL)
            entry = mkString(keystr_prefix, s_index, ":", k_current->key, NULL);
        else
            entry = mkString(keystr_prefix, s_index, ":", k_current->key, ",", NULL);
        
        // zrzucamy wpis do pamieci i przygotowujemy sie do kolejnej iteracji
        strncat(package, entry, strlen(entry) + 1);
        if(entry != NULL) free(entry);
        if(s_index != NULL) free(s_index);
        
        index++;
        k_current = k_current->next;
    }
    return package;
}
void cleanSysUsersData(sysuser * su) {
    sysuser * curr = su;
    sysuser * next = NULL;
    
    free(su->gecos);
    free(su->login);
    free(su->sha512);
    cleanSSHKeyData(su->sshkey);
    
    next = curr->next;
    if(next != NULL)
        cleanSysUsersData(next);
    free(curr);
}
void cleanSSHKeyData(sshkeys * k) {
    sshkeys * curr = k;
    sshkeys * next = NULL;
    
    free(curr->key);
    next = curr->next;
    if(next != NULL)
        cleanSSHKeyData(next);
    free(curr);
}
int getSysUsersPackageSize(sysuser * su) {
    int size = 0;           // licznik bajtow
    int keysize = 0;        // rozmiar kluczy w pakiecie
    sysuser * pos = su;     // aktualna pozycja w pamieci
    char * tmp = NULL;      // tymczasowa zmienna do przechowania
                            // wartosci numerycznych w formie stringu
    int userCount = 0;      // zliczanie liczby userow
    
    // nazwy kluczy w pakiecie;
    const char * keys[] = { "username:,", "password:,", "gecos:,", "expire:,",
                            "uidgid:,", "active:,", "purgedir:,", "shell:,",
                            "user_:", "{},", NULL};
    const char ** key = keys;
    while(*key) {
        keysize += strlen(*key);
        key++;
    }
    
    while(pos) {
        // Dane tekstowe
        size += strlen(pos->gecos);
        size += strlen(pos->login);
        size += strlen(pos->sha512);
        
        // Dane numeryczne;
        tmp = int2String(pos->active);
        size += strlen(tmp);
        free(tmp);
        tmp = int2String(pos->expiration);
        size += strlen(tmp);
        free(tmp);
        tmp = int2String(pos->shellaccess);
        size += strlen(tmp);
        free(tmp);
        tmp = int2String(pos->uidgid);
        size += strlen(tmp);
        free(tmp);
        
        tmp = int2String(userCount);
        size += strlen(tmp);
        free(tmp);
        
        // klucze ssh
        size += getSSHkeysPackageSize(pos->sshkey);
        
        userCount++;
        pos = pos->next;
    }
    size += keysize * userCount;
    size += strlen("{datatype:sysusers,}");
    
    return size;
}
int getSSHkeysPackageSize(sshkeys * ssh) {
    int size = 0;
    sshkeys * pos = ssh;
    int entryCount = 0;
    char * tmp = NULL;
    
    char * key = "sshkey_:,";
    while(pos) {
        tmp = int2String(entryCount);
        size += strlen(tmp);
        free(tmp);
        size += strlen(pos->key);
        
        entryCount++;
        pos = pos->next;
    }
    size += (strlen(key) * entryCount);
    
    return size;
}