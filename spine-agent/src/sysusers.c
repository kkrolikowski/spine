#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
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
    char * header = "{scope:sysusers,";
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
    
    free(curr->gecos);
    free(curr->login);
    free(curr->sha512);
    cleanSSHKeyData(curr->sshkey);
    
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
    size += strlen("{scope:sysusers,}");
    
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
int createUserAccounts(sysuser * su, FILE * lf) {
    int status = 1;
    sysuser * curr = su;
    char * msg = NULL;
    
    while(curr) {
        if(!userExist(curr->login)) {
            if(!writePasswd(curr)) {
                msg = mkString("[WARNING] (reciver) Blad zapisu ", curr->login, " w /etc/passwd", NULL);
                writeLog(lf, msg);
                curr = curr->next;
                continue;
            }
            if(!writeShadow(curr)) {
                msg = mkString("[WARNING] (reciver) Blad zapisu ", curr->login, " w /etc/shadow", NULL);
                writeLog(lf, msg);
                curr = curr->next;
                continue;
            }
            if(!writeGroup(curr)) {
                msg = mkString("[WARNING] (reciver) Blad zapisu ", curr->login, " w /etc/group", NULL);
                writeLog(lf, msg);
                curr = curr->next;
                continue;
            }
            if(!createHomeDir(curr, lf)) {
                msg = mkString("[WARNING] (reciver) Blad tworzenia katalogu domowego: /home/", curr->login, NULL);
                writeLog(lf, msg);
                curr = curr->next;
                continue;
            }
            if(writeAuthorizedKeys(curr, lf)) {
                msg = mkString("[INFO] (reciver) Klucze ssh dla ", curr->login, " zostaly zainstalowane", NULL);
                writeLog(lf, msg);
            }
            msg = mkString("[INFO] (reciver) Konto: ", curr->login, " zostalo poprawnie utworzone", NULL);
            writeLog(lf, msg);
        }
        else {
            msg = mkString("[WARNING] (reciver) Konto: ", curr->login, " juz istnieje", NULL);
            writeLog(lf, msg);
        }
        curr = curr->next;
    }
    return status;
}
int writePasswd(sysuser * su) {
    FILE * passwd;
    
    if((passwd = fopen("/etc/passwd", "a")) == NULL)
        return 0;
    
    if(su->shellaccess)
        fprintf(passwd, "%s:x:%d:%d:%s:/home/%s:/bin/bash\n", su->login, su->uidgid, su->uidgid, su->gecos, su->login);
    else
        fprintf(passwd, "%s:x:%d:%d:%s:/home/%s:/bin/false\n", su->login, su->uidgid, su->uidgid, su->gecos, su->login);
    
    fclose(passwd);
    
    return 1;
}
int writeShadow(sysuser * su) {
    FILE * shadow;
    time_t now;
    int pass_change = 0;
    int h_exp = 0;
    
    if((shadow = fopen("/etc/shadow", "a")) == NULL)
        return 0;
    
    now = time(NULL);
    pass_change = now / 86400;
    if(su->expiration > 0) {
        h_exp = su->expiration / 86400;
        fprintf(shadow, "%s:%s:%d:0:99999:7::%d:\n", su->login, su->sha512, pass_change, h_exp);
    }
    else
        fprintf(shadow, "%s:%s:%d:0:99999:7:::\n", su->login, su->sha512, pass_change);
    
    fclose(shadow);
    return 1;
}
int writeGroup(sysuser * su) {
    FILE * group;
    
    if((group = fopen("/etc/group", "a")) == NULL)
        return 0;
    
    fprintf(group, "%s:x:%d:\n", su->login, su->uidgid);
    
    fclose(group);
    return 1;
}
int userExist(char * login) {
    FILE * fpasswd;
    char buff[256];
    int status = 0;
    
    if((fpasswd = fopen("/etc/passwd", "r")) == NULL)
        return -1;
    memset(buff, '\0', 256); 
    while(fgets(buff, 256, fpasswd) != NULL) {
        if(strstr(buff, login) != NULL) {
            status = 1;
            break;
        }
        memset(buff, '\0', 256);
    }
    fclose(fpasswd);
    return status;
}
int createHomeDir(sysuser * su, FILE * lf) {
    char * homedir = mkString("/home/", su->login, NULL);
    char * skel = "/etc/skel/";
    
    if(!mkdir(homedir, 0711)) {
        if(chown(homedir, (uid_t) su->uidgid, (gid_t) su->uidgid)) {
            free(homedir);
            return 0;
        }
        recursiveCopy(su, skel, lf);
    }
    free(homedir);
    
    return 1;
}
void recursiveCopy(sysuser * su, char * path, FILE * lf) {
    DIR * d;
    struct dirent * item;
    struct stat itemType;
    char * to = NULL;
    const int MaxPath = 128;
    char sourcePath[MaxPath];               // sciezka zrolowa    
    static char relativePath[128];          // sciezka wzgledna do /home/user/
    char oldRelPath[MaxPath];               // poprzednia wartosc sciezki wzglednej
    char * msg = NULL;
    
    memset(oldRelPath, '\0', MaxPath);
    memset(sourcePath, '\0', MaxPath);
    strncpy(sourcePath, path, strlen(path));
    d = opendir(path);
    
    while((item = readdir(d)) != NULL) {
        if(!strcmp(item->d_name, ".") || !strcmp(item->d_name, ".."))
            continue;
        
        strncat(sourcePath, item->d_name, strlen(item->d_name));
        stat(sourcePath, &itemType);
        if(relativePath[0] != '\0')
            strncpy(oldRelPath, relativePath, strlen(relativePath));
        strncat(relativePath, item->d_name, strlen(item->d_name));  
        to = mkString("/home/", su->login, "/", relativePath, NULL);
        if(S_ISDIR(itemType.st_mode)) {
            mkdir(to, 0711); 
            if(chown(to, su->uidgid, su->uidgid)) {
                msg = mkString("[WARNING] Zmiana uprawnien do katalogu ", to, " nie powiodla sie.", NULL);
                writeLog(lf, msg);
            }
            strncat(sourcePath, "/", 1);
            strncat(relativePath, "/", 1);
            recursiveCopy(su, sourcePath, lf);
        }
        else {
            copy(sourcePath, to);
            if(chown(to, su->uidgid, su->uidgid)) {
                msg = mkString("[WARNING] Zmiana wlasciciela ", to, " nie powiodla sie.", NULL);
                writeLog(lf, msg);
            }
            if(chmod(to, 0644)) {
                msg = mkString("[WARNING] Zmiana uprawnien do ", to, " nie powiodla sie.", NULL);
                writeLog(lf, msg);
            }
            memset(sourcePath, '\0', MaxPath);
        }
        memset(sourcePath, '\0', MaxPath);
        memset(relativePath, '\0', MaxPath);
        strncpy(sourcePath, path, strlen(path));
        strncpy(relativePath, oldRelPath, strlen(oldRelPath));
        if(to != NULL) free(to);
    }
    closedir(d);
}
int copy(char * from, char * to) {
   FILE * src;
   FILE * dst;
   char buff[512];
   
   if((src = fopen(from, "r")) == NULL)
       return 0;
   if((dst = fopen(to, "w")) == NULL)
       return 0;
   
   memset(buff, '\0', 512);
   while(fgets(buff, 512, src) != NULL) {
       fputs(buff, dst);
       memset(buff, '\0', 512);
   }
   
   fclose(src);
   fclose(dst);
   return 1;
}
sshkeys * readSSHKeysFromPackage(char * str) {
    char * pos = str;                       // aktualna pozycja w stringu
    char * end = strstr(str, "},user_");    // adres konca przetwarzania
    char * end2 = strstr(str, "}}{scope:"); // adres konca
    int step = strlen("sshkey_:") + 1;      // dlugosc klucza
    char buff[512];                         // bufor w ktorym przechowamy oczytany klucz ssh
    int i = 0;                              // index bufora
    
    // lista kluczy ssh odczytanych z pakietu
    sshkeys * head = NULL;
    sshkeys * curr = NULL;
    sshkeys * prev = NULL;
    
    memset(buff, '\0', 512);
    while((pos = strstr(pos, "sshkey_")) != NULL && ((pos < end) || (pos < end2))) {
        pos += step;
        while(*pos != ',' && *pos != '}') {
            buff[i] = *pos;
            i++; pos++;
        }
        curr = malloc(sizeof(sshkeys));
        curr->key = malloc((strlen(buff) + 1) * sizeof(char));
        memset(curr->key, '\0', strlen(buff) + 1);
        strncpy(curr->key, buff, strlen(buff));
        memset(buff, '\0', 512);
        i = 0;
        curr->next = NULL;
        
        if(head == NULL)
            head = curr;
        else
            prev->next = curr;
        prev = curr;
    }
    return head;
}
int writeAuthorizedKeys(sysuser * su, FILE * lf) {
    int ok = 1;
    sshkeys * curr = su->sshkey;
    char * msg = NULL;
    FILE * authKeys;
    char * sshDirPath = mkString("/home/", su->login, "/.ssh", NULL);
    char * authKeysPath = mkString("/home/", su->login, "/.ssh/authorized_keys", NULL);
    
    if(!strcmp(curr->key, "NaN"))
        return 0;
    
    if(mkdir(sshDirPath, 0700) < 0) {
        if(errno == EEXIST)
            msg = mkString("[WARNING] Katalog /home/", su->login, "/.ssh juz istnieje.", NULL);
        else {
            msg = mkString("[ERROR] Nie udalo sie stworzyc katalogu /home/", su->login, ".ssh.", NULL);
            ok = 0;
        }
        writeLog(lf, msg);
    }
    if(chown(sshDirPath, su->uidgid, su->uidgid) < 0) {
        msg = mkString("[WARNING] Nie udalo sie zmienic wlasciciela dla ", sshDirPath, NULL);
        writeLog(lf, msg);
    }
    if(ok) {
        if((authKeys = fopen(authKeysPath, "w")) == NULL) {
            msg = mkString("[ERROR] Blad tworzenia pliku ", authKeysPath, NULL);
            writeLog(lf, msg);
            ok = 0;
        }
        else {
            while(curr) {
                fprintf(authKeys, "%s\n", curr->key);
                curr = curr->next;
            }
            fclose(authKeys);
            if(chown(authKeysPath, su->uidgid, su->uidgid) < 0) {
                msg = mkString("[WARNING] Nie udalo sie zmienic wlasciciela dla ", authKeysPath, NULL);
                writeLog(lf, msg);
            }
            if(chmod(authKeysPath, 0600) < 0) {
               msg = mkString("[WARNING] Nie udalo sie zmienic uprawnien do ", authKeysPath, NULL);
               writeLog(lf, msg); 
            }
        }
    }
    return ok;
}