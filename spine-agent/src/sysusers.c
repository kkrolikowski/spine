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