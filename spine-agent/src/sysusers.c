#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commondata.h"
#include "sysusers.h"
#include "core.h"

void sysUsersInit(sysuser * su) {
    su->active = 0;
    su->expiration = 0;
    su->gecos = NULL;
    su->login = NULL;
    su ->next = NULL;
    su->sha512 = NULL;
    su->shellaccess = 0;
    su->sshkey->key = NULL;
    su->sshkey->next = NULL;
    su->uidgid = 0;
}
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
    size_t package_keys_len = 0;        // rozmiar kluczy pakietu
    size_t package_vals_len = 0;        // rozmiar wartosci kluczy w pakiecie
    size_t package_len = 0;             // calkowity rozmiar pakietu
    
    // klucze dla wartosci w pakiecie i inne stale elementy
    char * header = "[{datatype:sysusers,";
    char * s_user = "user_";
    char * s_username = "username:";
    char * s_password = "password:";
    char * s_gecos = "gecos:";
    char * s_active = "active:";
    char * s_uidgid = "uidgid:";
    char * s_shell = "shell:";
    char * s_expire = "expire:";
    char * tailer1 = "},";
    char * tailer2 = "}},";
    
    // obliczanie ilosci pamieci potrzebnej do przechowania pakietu
    package_keys_len = strlen(s_user) + strlen(s_username) +
            strlen(s_password) + strlen(s_gecos) + strlen(s_active) +
            strlen(s_uidgid) + strlen(s_shell) + strlen(s_expire);
    
    while(su) {
        package_vals_len += strlen(su->gecos);
        package_vals_len += strlen(su->login);
        package_vals_len += strlen(su->sha512);
        
        s_expval_val = int2String(su->expiration);
        package_vals_len += strlen(s_expval_val);
        free(s_expval_val);
        
        s_uidgid_val = int2String(su->uidgid);
        package_vals_len += strlen(s_uidgid_val);
        free(s_uidgid_val);
        
        s_active_val = int2String(su->active);
        package_vals_len += strlen(s_active_val);
        free(s_active_val);
        
        s_shell_val = int2String(su->shellaccess);
        package_vals_len += strlen(s_shell_val);
        free(s_shell_val);
        
        keyentry = sshkeysPackage(su->sshkey);
        package_vals_len += strlen(keyentry);
        free(keyentry);
        
        index++;
        su = su->next;
    }
    
    // obliczamy potrzebna ilosc pamieci a nastepnie ja alokujemy 
    package_len = package_vals_len +            // dlugosc wszystkich wartosci
            index * (package_keys_len + 5) +    // dlugosc wszystkich kluczy + przecinek i klamry
            strlen(header) + 2;                 // dlugosc naglowka (header) + klamra zamykajaca pakiet
    package = (char *) malloc(package_len * sizeof(char));
    memset(package, '\0', package_len);
    
    // resetujemy zmienne i zaczynamy budowac pakiet
    index = 0;
    su = su_begin;
    strncpy(package, header, strlen(header));
    while(su) {
        s_index = int2String(index);
        s_expval_val = int2String(su->expiration);
        s_uidgid_val = int2String(su->uidgid);
        s_active_val = int2String(su->active);
        s_shell_val = int2String(su->shellaccess);
        keyentry = sshkeysPackage(su->sshkey);
        
        entry = mkString(s_user, s_index, ":{",
                         s_username, su->login, ",",
                         s_password, su->sha512, ",",
                         s_gecos, su->gecos, ",",
                         s_expire, s_expval_val, ",",
                         s_uidgid, s_uidgid_val, ",",
                         s_active, s_active_val, ",",
                         s_shell, s_shell_val, ",",
                         keyentry, NULL);
        strncat(package, entry, strlen(entry) + 1);
        if(su->next == NULL)
            strncat(package, "}", 2);
        else
            strncat(package, "},", 3);
        
        free(keyentry);
        free(entry);
        free(s_index);
        index++;
        su = su->next;
    }
    strncat(package, "}", 2);
    
    return package;
}                                               
                                                
char * sshkeysPackage(sshkeys * k) {
    char * package = NULL;              // wskaznik do wynikowego stringu
    char * entry = NULL;                // wskaznik do pojedynczego wpisu
    int index = 0;                      // licznik kluczy ssh
    char * s_index = int2String(index); // licznik kluczy ssh w formie stringu
    sshkeys * k_begin = k;              // poczatek wezla przechowujacego klucze
    size_t len = 0;                     // rozmiar stringow
    size_t keys_len = 0;                // rozmiar stringu wszystkich kluczy
    char * keystr_prefix = "sshkey_";   // prefix stringu zawierajacego klucz
    
    // obliczamy ile bedzie potrzeba pamieci do przechowania wszystkich kluczy
    while(k) {
        keys_len += strlen(k->key);
        index++;
        k = k->next;
    }
    
    // obliczamy calkowity rozmiar pamieci przechowujacej string
    // a nastepnie przygotowujemy obszar pamieci
    len = keys_len + (strlen(keystr_prefix) + 2) * index + 1;
    package = (char *) malloc(len * sizeof(char));
    memset(package, '\0', len);
    
    // wracamy na poczatek wezla i resetujemy licznik
    k = k_begin;
    index = 0;
    
    // budujemy pakiet z kluczami ssh
    while(k) {
        if(k->next == NULL)
            entry = mkString(keystr_prefix, s_index, ":", k->key, NULL);
        else
            entry = mkString(keystr_prefix, s_index, ":", k->key, ",", NULL);
        
        // zrzucamy wpis do pamieci i przygotowujemy sie do kolejnej iteracji
        strncat(package, entry, strlen(entry) + 1);
        free(entry);
        free(s_index);
        
        index++;
        s_index = int2String(index);
        k = k->next;
    }
    return package;
}