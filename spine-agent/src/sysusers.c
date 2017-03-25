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
    char * s_cfgver = NULL;             // wersja konfiguracji (string)
    char * s_sudo_val = NULL;           // czy konto ma miec dostep do roota
    char * s_id_val = NULL;             // ID rekordu z bazy
    sysuser * su_begin = su;            // poczatek wezla
    sysuser * su_curr = su;             // aktualnie przetwarzane konto
    size_t package_len = 0;             // calkowity rozmiar pakietu
    
    // klucze dla wartosci w pakiecie i inne stale elementy
    char * header = "{scope:sysusers,";
    char * s_version = "config_ver:";
    
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
        s_id_val = int2String(su_curr->dbid);
        s_expval_val = int2String(su_curr->expiration);
        s_uidgid_val = int2String(su_curr->uidgid);
        s_active_val = int2String(su_curr->active);
        s_shell_val = int2String(su_curr->shellaccess);
        s_sudo_val = int2String(su_curr->sudo);
        keyentry = sshkeysPackage(su_curr->sshkey);
        
        entry = mkString(
                        "user_",        s_index,           ":{",
                        "dbid:",        s_id_val,           ",",
                        "username:",    su_curr->login,     ",",
                        "password:",    su_curr->sha512,    ",",
                        "gecos:",       su_curr->gecos,     ",",
                        "expire:",      s_expval_val,       ",",
                        "uidgid:",      s_uidgid_val,       ",",
                        "active:",      s_active_val,       ",",
                        "shell:",       s_shell_val,        ",",
                        "sudo:",        s_sudo_val,         ",",
                        "status:",      su_curr->status,    ",",
                        keyentry,                           NULL
                    );
        strncat(package, entry, strlen(entry) + 1);
        strncat(package, "},", 3);
        if(su_curr->next == NULL)
            s_cfgver = int2String(su_curr->version);
              
        if(keyentry != NULL)     free(keyentry);
        if(entry != NULL)        free(entry);
        if(s_index != NULL)      free(s_index);
        if(s_expval_val != NULL) free(s_expval_val);
        if(s_uidgid_val != NULL) free(s_uidgid_val);
        if(s_active_val != NULL) free(s_active_val);
        if(s_shell_val != NULL)  free(s_shell_val);
        if(s_sudo_val != NULL)   free(s_sudo_val);
        if(s_id_val != NULL)     free(s_id_val);
        
        index++;
        su_curr = su_curr->next;
    }
    strncat(package, s_version, strlen(s_version));
    strncat(package, s_cfgver, strlen(s_cfgver));
    strncat(package, "}", 2);
    
    free(s_cfgver);
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
    free(curr->status);
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
    sysuser * curr = su;    // current node
    int size = 0;           // overrall data size
    int keySize = 0;        // key names size
    int nodeCount = 0;      // processed nodes count
    char * tmp = NULL;      // temporary string
    
    // package header 
    char * header = "{scope:sysusers,},";  
    // package keys names
    const char * keys[] = { "dbid:," "username:,", "password:,", "gecos:,", "expire:,",
                            "uidgid:,", "active:,", "purgedir:,", "shell:,",
                            "user_:", "sudo:,", "status:,", "config_version:,", "{},", NULL
                          };
    const char ** key = keys;
    
    while(*key)
        keySize += strlen(*key++);
    
    while(curr) {
        // string data
        size += strlen(curr->gecos);
        size += strlen(curr->login);
        size += strlen(curr->sha512);
        size += strlen(curr->status);
        
        // numeric data
        tmp = int2String(curr->dbid);
        size += strlen(tmp);
        free(tmp);
        tmp = int2String(curr->active);
        size += strlen(tmp);
        free(tmp);
        tmp = int2String(curr->expiration);
        size += strlen(tmp);
        free(tmp);
        tmp = int2String(curr->shellaccess);
        size += strlen(tmp);
        free(tmp);
        tmp = int2String(curr->uidgid);
        size += strlen(tmp);
        free(tmp);
        tmp = int2String(curr->sudo);
        size += strlen(tmp);
        free(tmp);
        tmp = int2String(nodeCount);
        size += strlen(tmp);
        free(tmp);
        
        if(curr->next == NULL) {
            tmp = int2String(curr->version);
            size += strlen(tmp);
            free(tmp);
        }
        // ssh keys
        size += getSSHkeysPackageSize(curr->sshkey);
        
        nodeCount++;
        curr = curr->next;
    }
    size += strlen(header);
    size += keySize * nodeCount;

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
int createUserAccounts(sysuser * su, char * os, FILE * lf) {
    int status = 1;
    sysuser * curr = su;
    char * msg = NULL;
    
    if(!userExist(curr->uidgid)) {
        if(!writePasswd(curr)) {
            msg = mkString("[WARNING] (reciver) Blad zapisu ", curr->login, " w /etc/passwd", NULL);
            writeLog(lf, msg);
        }
        if(!writeShadow(curr)) {
            msg = mkString("[WARNING] (reciver) Blad zapisu ", curr->login, " w /etc/shadow", NULL);
            writeLog(lf, msg);
        }
        if(!curr->active)
            if(disableAccount(curr->login)) {
                msg = mkString("[INFO] (reciver) Account ", curr->login, " is disabled", NULL);
                writeLog(lf, msg);
            }
        if(!writeGroup(curr)) {
            msg = mkString("[WARNING] (reciver) Blad zapisu ", curr->login, " w /etc/group", NULL);
            writeLog(lf, msg);
        }
        if(!createHomeDir(curr, lf)) {
            msg = mkString("[WARNING] (reciver) Blad tworzenia katalogu domowego: /home/", curr->login, NULL);
            writeLog(lf, msg);
        }
        if(writeAuthorizedKeys(curr, lf)) {
            msg = mkString("[INFO] (reciver) Klucze ssh dla ", curr->login, " zostaly zainstalowane", NULL);
            writeLog(lf, msg);
        }
        msg = mkString("[INFO] (reciver) Konto: ", curr->login, " zostalo poprawnie utworzone", NULL);
        writeLog(lf, msg);
        if(curr->sudo) {
            if(!grantSuperUser(curr->login, os)) {
                msg = mkString("[ERROR] (reciver) Nie udalo sie przyznac sudo dla konta ", curr->login, NULL);
                writeLog(lf, msg);
            }
        }
    }
    else {
        msg = mkString("[WARNING] (reciver) Konto: ", curr->login, " juz istnieje", NULL);
        writeLog(lf, msg);
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
int userExist(int uid) {
    FILE * fpasswd;
    char * s_uid = int2String(uid);
    char buff[256];
    int status = 0;
    
    if((fpasswd = fopen("/etc/passwd", "r")) == NULL) {
        free(s_uid);
        return -1;
    }
    memset(buff, '\0', 256); 
    while(fgets(buff, 256, fpasswd) != NULL) {
        if(strstr(buff, s_uid) != NULL) {
            status = 1;
            break;
        }
        memset(buff, '\0', 256);
    }
    fclose(fpasswd);
    free(s_uid);
    
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
    char * end = NULL;                       // adres konca przetwarzania
    int step = strlen("sshkey_:") + 1;      // dlugosc klucza
    char buff[512];                         // bufor w ktorym przechowamy oczytany klucz ssh
    int i = 0;                              // index bufora
    
    // lista kluczy ssh odczytanych z pakietu
    sshkeys * head = NULL;
    sshkeys * curr = NULL;
    sshkeys * prev = NULL;
    
    end = strstr(str, "},user_");
    if(end == NULL)
        end = strstr(str, "},config_ver");
    
    memset(buff, '\0', 512);
    while((pos = strstr(pos, "sshkey_")) != NULL && pos < end) {
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
                if(strcmp(curr->key, "NaN"))
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
int grantSuperUser(char * login, char * os) {
    FILE * group;               // handle to /etc/group
    FILE * tmp;                 // handle to tempfile
    char * entry = NULL;        // changed group entry
    char * groupName = NULL;    // name of group
    char buff[512];             // buffer for file operations
    
    if(!strcmp(os, "Ubuntu"))
        groupName = "sudo";
    else
        groupName = "wheel";
    
    memset(buff, '\0', 512);
    if((group = fopen("/etc/group", "r")) == NULL)
        return 0;
    if((tmp = tmpfile()) == NULL) {
        fclose(group);
        return 0;
    }
    // lets create tempfile with changed content from /etc/group
    while(fgets(buff, 512, group) != NULL) {
        if(strstr(buff, groupName) != NULL) {
            entry = updateGroup(buff, login);
            fputs(entry, tmp);
            free(entry);
        }
        else
            fputs(buff, tmp);
        memset(buff, '\0', 512);
    }
    fclose(group);
    
    // now lets write new /etc/group file
    if((group = fopen("/etc/group", "w")) == NULL)
        return 0;
    rewind(tmp);
    memset(buff, '\0', 512);
    while(fgets(buff, 512, tmp) != NULL) {
        fputs(buff, group);
        memset(buff, '\0', 512);
    }
    fclose(group);
    fclose(tmp);
    
    return 1;
}
char * updateGroup(char * buff, char * login) {
    char * entry = NULL;        // entry containing updated sudo group
    size_t len = 0;             // memory size for new entry
    
    // memory size with coma separator and \0 sign and newline
    len = strlen(buff) + strlen(login) + 3;
    entry = (char *) malloc(len * sizeof(char));
    memset(entry, '\0', len);

    // copy string without newline
    strncpy(entry, buff, strlen(buff) - 1);
    if(*(entry+strlen(entry) - 1) != ':')
        strncat(entry, ",", 1);
    // appendd login with newline at the end
    strncat(entry, login, strlen(login));
    *(entry + strlen(entry)) = '\n';
    
    return entry;
}
resp * updateUserAccounts(sysuser * su, char * os, FILE * lf, resp * respdata) {
    sysuser * curr = su;
    char * msg = NULL;
    char * old = NULL;
    char * homedir = NULL;
    
    // response to server
    resp * rhead = respdata;
    resp * rcurr = NULL;
    resp * rprev = NULL;
    
    // moving to the end of the list
    while(rhead != NULL)
        rhead = rhead->next;
    
    while(curr) {
        if(!strcmp(curr->status, "N")) {
            createUserAccounts(curr, os, lf);
            rcurr = respStatus("sysusers", 'A', curr->dbid);
            if(rhead == NULL)
                rhead = rcurr;
            else
                rprev->next = rcurr;
            rprev = rcurr;
        }
        if(!strcmp(curr->status, "U")) {
            if((old = oldlogin(curr->uidgid, curr->login)) != NULL) {
                if(renameHomeDir(old, curr->login)) {
                  msg = mkString("[INFO] (reciver) Changed homedir from: /home/", old, " to: /home/", curr->login, NULL);
                  writeLog(lf, msg);  
                }
                if(updateGroupFile(curr, old)) {
                    msg = mkString("[INFO] (reciver) Changed group info from user: ", old, " to: ", curr->login, NULL);
                    writeLog(lf, msg);
                }
            } 
            else
                old = curr->login;
            if(updatePasswd(curr)) {
                if(updateShadow(curr, old)) {
                    msg = mkString("[INFO] (reciver) Passwd info updated for user: ", curr->login, NULL);
                    writeLog(lf, msg);
                }
            }
            if(!curr->active) {
                if(disableAccount(curr->login)) {
                    msg = mkString("[INFO] (reciver) Account: ", curr->login, " is locked", NULL);
                    writeLog(lf, msg);
                }
            }
            if(curr->sudo) {
                if(!isAdmin(os, curr->login)) {
                    if(!grantSuperUser(curr->login, os)) {
                        msg = mkString("[ERROR] (reciver) Nie udalo sie przyznac sudo dla konta ", curr->login, NULL);
                        writeLog(lf, msg);
                    }
                }
            }
            else {
                if(revokeSudoAccess(curr->login, os))
                    msg = mkString("[INFO] (reciver) Admin access disabled for ", curr->login, NULL);
                else
                    msg = mkString("[ERROR] (reciver) Error disabling admin access for ", curr->login, NULL);
                writeLog(lf, msg);
            }
            if(updateSSHKeys(curr, lf))
                msg = mkString("[INFO] (reciver) SSH keys for ", curr->login, NULL);
            else
                msg = mkString("[INFO] (reciver) Disabled SSH keys for ", curr->login, NULL);
            writeLog(lf, msg);
            
            rcurr = respStatus("sysusers", 'A', curr->dbid);
            if(rhead == NULL)
                rhead = rcurr;
            else
                rprev->next = rcurr;
            rprev = rcurr;
        }
        if(!strcmp(curr->status, "D")) {
            if(!removeFromSystemFile(curr->login, "/etc/passwd")) {
                msg = mkString("[WARNING] (reciver) Problem with removing ", curr->login, " from /etc/passwd", NULL);
                writeLog(lf, msg);
            }
            if(!removeFromSystemFile(curr->login, "/etc/shadow")) {
                msg = mkString("[WARNING] (reciver) Problem with removing ", curr->login, " from /etc/shadow", NULL);
                writeLog(lf, msg);
            }
            if(revokeSudoAccess(curr->login, os)) {
               if(!removeFromSystemFile(curr->login, "/etc/group")) {
                   msg = mkString("[WARNING] (reciver) Problem with removing ", curr->login, " from /etc/group", NULL);
                   writeLog(lf, msg);
               } 
            }
            else {
                msg = mkString("[WARNING] (reciver) Problem with revoking super privileges from ", curr->login, NULL);
                writeLog(lf, msg);
            }
            homedir = mkString("/home/", curr->login, "/", NULL);
            purgeDir(homedir);
            msg = mkString("[INFO] (reciver) Account ", curr->login, " deleted.", NULL);
            writeLog(lf, msg);
            free(homedir);
            
            rcurr = respStatus("sysusers", 'D', curr->dbid);
            if(rhead == NULL)
                rhead = rcurr;
            else
                rprev->next = rcurr;
            rprev = rcurr;
        }
        curr = curr->next;
    }
    return rhead;
}
int updatePasswd(sysuser * su) {
    int status = 1;                         // exit code: 1 - success, 0 - failure
    FILE * passwd = NULL;                   // /etc/passwd file handle
    FILE * tmp = NULL;                      // temp. file handle
    char * s_uid = int2String(su->uidgid);    // UID in string form
    char buff[512];                         // buffer for text processing
    
    // opening passwd file for reading
    if((passwd = fopen("/etc/passwd", "r")) == NULL) {
        free(s_uid);
        return 0;
    }
    // opening tmpfile for writing
    if((tmp = tmpfile()) == NULL) {
        fclose(passwd);
        free(s_uid);
        return 0;
    }
    
    memset(buff, '\0', 512);
    while(fgets(buff, 512, passwd) != NULL) {
        if(strstr(buff, s_uid) != NULL) {
            if(su->shellaccess)
                fprintf(tmp, "%s:x:%d:%d:%s:/home/%s:/bin/bash\n", su->login, su->uidgid, su->uidgid, su->gecos, su->login);
            else
                fprintf(tmp, "%s:x:%d:%d:%s:/home/%s:/bin/false\n", su->login, su->uidgid, su->uidgid, su->gecos, su->login);
        }
        else
            fputs(buff, tmp);
        
        memset(buff, '\0', 512);
    }
    free(s_uid);
    fclose(passwd);
    
    rewind(tmp);
    memset(buff, '\0', 512);
    if((passwd = fopen("/etc/passwd", "w")) == NULL) {
        fclose(tmp);
        return 0;
    }
    while(fgets(buff, 512, tmp) != NULL) {
        fputs(buff, passwd);
        memset(buff, '\0', 512);
    }
    fclose(tmp);
    fclose(passwd);
    
    return status;
}
char * oldlogin(int uid, char * new) {
    FILE * passwd = NULL;
    char * s_uid = int2String(uid);
    char buff[512];
    char buff2[512];
    char * old = NULL;
    size_t oldlen = 0;
    int i = 0;
    
    if((passwd = fopen("/etc/passwd", "r")) == NULL) {
        free(s_uid);
        return NULL;
    }
    memset(buff, '\0', 512);
    memset(buff2, '\0', 512);
    while(fgets(buff, 512, passwd) != NULL) {
        if(strstr(buff, s_uid) != NULL) {
            while(buff[i] != ':') {
                buff2[i] = buff[i];
                i++;
            }
            break;
        }
        memset(buff, '\0', 512);
    }
    fclose(passwd);
    free(s_uid);
    
    oldlen = strlen(buff2) + 1;
    old = (char *) malloc(oldlen * sizeof(char));
    memset(old, '\0', oldlen);
    strncpy(old, buff2, oldlen);
    
    if(!strcmp(old, new))
        return NULL;
    else
        return old;
}
int updateShadow(sysuser * su, char * login) {
    FILE * shadow = NULL;
    FILE * tmp = NULL;
    char buff[512];
    time_t now = time(NULL);
    int pass_change = now / 86400;
    int h_exp = 0;
    
    if((shadow = fopen("/etc/shadow", "r")) == NULL)
        return 0;
    if((tmp = tmpfile()) == NULL)
        return 0;

    memset(buff, '\0', 512);
    while(fgets(buff, 512, shadow) != NULL) {
        if(strstr(buff, login) != NULL) {
            if(su->expiration > 0) {
                h_exp = su->expiration / 86400;
                fprintf(tmp, "%s:%s:%d:0:99999:7::%d:\n", su->login, su->sha512, pass_change, h_exp);
            }
            else
                fprintf(tmp, "%s:%s:%d:0:99999:7:::\n", su->login, su->sha512, pass_change);
        }
        else
            fputs(buff, tmp);
        
        memset(buff, '\0', 512);
    }
    fclose(shadow);
    
    rewind(tmp);
    if((shadow = fopen("/etc/shadow", "w")) == NULL) {
        fclose(tmp);
        return 0;
    }
    memset(buff, '\0', 512);
    while(fgets(buff, 512, tmp) != NULL) {
        fputs(buff, shadow);
        memset(buff, '\0', 512);
    }
    fclose(tmp);
    fclose(shadow);
    
    return 1;
}
int updateGroupFile(sysuser * su, char * oldlogin) {
    FILE * group = NULL;
    FILE * tmp = NULL;
    char * newentry = NULL;
    char buff[512];
    
    if((group = fopen("/etc/group", "r")) == NULL)
        return 0;
    if((tmp = tmpfile()) == NULL) {
        fclose(group);
        return 0;
    }
    
    memset(buff, '\0', 512);
    while(fgets(buff, 512, group) != NULL) {
        if(strstr(buff, oldlogin) != NULL) {
            newentry = changeLogin(buff, oldlogin, su->login);
            fputs(newentry, tmp);
            free(newentry);
        }
        else
            fputs(buff, tmp);
        
        memset(buff, '\0', 512);
    }
    fclose(group);
    
    rewind(tmp);
    memset(buff, '\0', 512);
    if((group = fopen("/etc/group", "w")) == NULL) {
        fclose(tmp);
        return 0;
    }
    while(fgets(buff, 512, tmp) != NULL) {
        fputs(buff, group);
        memset(buff, '\0', 512);
    }
    fclose(group);
    fclose(tmp);
    
    return 1;
}
char * changeLogin(char * entry, char * oldlogin, char * newlogin) {
    // local buffer
    const int BuffSize = 512;
    char buff[BuffSize];
    // begining of group entry string
    char * curr = entry;
    // begining of old login string
    char * oldlogin_beg = strstr(entry, oldlogin);
    // ending of old login string
    char * oldlogin_end = oldlogin_beg + strlen(oldlogin);
    // new group entry
    char * newentry = NULL;
    size_t newentry_len = 0;
    // buffer index
    int i = 0;

    // initializing the buffer
    memset(buff, '\0', BuffSize);
    
    // changing login string and building new entry in a buffer
    while(*curr) {
        if(curr != oldlogin_beg)
            buff[i++] = *curr++;
        else {
            strncat(buff, newlogin, strlen(newlogin));
            curr = oldlogin_end;
            i += strlen(newlogin);
        }
    }
    
    // returning new entry
    newentry_len = strlen(buff) + 1;
    newentry = (char *) malloc(newentry_len * sizeof(char));
    memset(newentry, '\0', newentry_len);
    strncpy(newentry, buff, newentry_len);
    
    return newentry;
}
int revokeSudoAccess(char * login, char * os) {
    FILE * grp = NULL;
    FILE * tmp = NULL;
    char * admGrp = NULL;
    char * newentry = NULL;
    const int BufSize = 512;
    char buff[BufSize];
    
    if(!strcmp(os, "Ubuntu"))
        admGrp = "sudo";
    else
        admGrp = "wheel";
    
    if((grp = fopen("/etc/group", "r")) == NULL)
        return 0;
    if((tmp = tmpfile()) == NULL) {
        fclose(grp);
        return 0;
    }
    
    memset(buff, '\0', BufSize);
    while(fgets(buff, BufSize, grp) != NULL) {
        if(strstr(buff, admGrp) != NULL) {
            if((newentry = rmFromGrp(buff, login)) != NULL) {
                fputs(newentry, tmp);
                free(newentry);
            }
            else
                fputs(buff, tmp);
        }
        else
            fputs(buff, tmp);
        memset(buff, '\0', BufSize);
    }
    fclose(grp);
    rewind(tmp);
    memset(buff, '\0', BufSize);
    if((grp = fopen("/etc/group", "w")) == NULL) {
        fclose(tmp);
        return 0;
    }
    while(fgets(buff, BufSize, tmp) != NULL) {
        fputs(buff, grp);
        memset(buff, '\0', BufSize);
    }
    fclose(grp);
    fclose(tmp);
    
    return 1;
}
char * rmFromGrp(char * entry, char * login) {
    char * newentry = NULL;         // new group entry
    char * lbeg = NULL;             // begin of login string
    char * lend = NULL;             // end of login string
    char * curr = entry;            // start of entry
    const int BufSize = 512;
    char buff[BufSize];             // local buffer
    int i = 0;                      // buffer index
    size_t len = 0;                 // entry length
    
    // verify if entry contains such user
    if((lbeg = strstr(entry, login)) != NULL)
        lend = lbeg + strlen(login);
    else
        return NULL;
    
    // processing entry with exclusion of user string
    memset(buff, '\0', BufSize);
    while(*curr) {
        if(curr < lbeg || curr > lend)
            buff[i++] = *curr;
        curr++;
    }
    
    // cleanup: sometimes unwanted comma stays in buffer
    if(buff[strlen(buff)-1] == ',')
        buff[strlen(buff)-1] = '\0';
    
    // processing buffer
    len = strlen(buff) + 2;
    newentry = (char *) malloc(len * sizeof(char));
    memset(newentry, '\0', len);
    strncpy(newentry, buff, len);
    *(newentry+strlen(newentry)) = '\n';
    
    return newentry;
}
int renameHomeDir(char * olduser, char * newuser) {
    char * oldpath = mkString("/home/", olduser, NULL);
    char * newpath = mkString("/home/", newuser, NULL);
    
    if(!rename(oldpath, newpath))
        return 1;
    else
        return 0;
}
int isAdmin(char * os, char * login) {
    FILE * grp = NULL;
    char * admGroup = NULL;
    const int BufSize = 512;
    char buff[BufSize];
    
    if(!strcmp(os, "Ubuntu"))
        admGroup = "sudo";
    else
        admGroup = "wheel";
    
    if((grp = fopen("/etc/group", "r")) == NULL)
        return 0;
    
    memset(buff, '\0', BufSize);
    while(fgets(buff, BufSize, grp) != NULL) {
        if(strstr(buff, admGroup) != NULL)
            break;
        memset(buff, '\0', BufSize);
    }
    fclose(grp);
    
    if(strstr(buff, login) != NULL)
        return 1;
    else
        return 0;
}
int disableAccount(char * login) {
    FILE * shadow = NULL;
    FILE * tmp = NULL;
    char * newentry = NULL;
    const int BufSize = 512;
    char buff[BufSize];
    
    if((shadow = fopen("/etc/shadow", "r")) == NULL)
        return 0;
    if((tmp = tmpfile()) == NULL) {
        fclose(shadow);
        return 0;
    }
    
    memset(buff, '\0', BufSize);
    while(fgets(buff, BufSize, shadow) != NULL) {
        if(strstr(buff, login) != NULL) {
            newentry = lockEntry(buff);
            fputs(newentry, tmp);
            free(newentry);
        }
        else
            fputs(buff, tmp);
        memset(buff, '\0', BufSize);
    }
    fclose(shadow);
    rewind(tmp);
    
    if((shadow = fopen("/etc/shadow", "w")) == NULL) {
        fclose(tmp);
        return 0;
    }
 
    memset(buff, '\0', BufSize);
    while(fgets(buff, BufSize, tmp) != NULL) {
        fputs(buff, shadow);
        memset(buff, '\0', BufSize);
    }
    fclose(shadow);
    fclose(tmp);
    
    return 1;
}
char * lockEntry(char * entry) {
    char * pos = entry;
    char * colon = strchr(entry, ':');
    char * newentry = NULL;
    char * ne_pos = NULL;
    size_t entry_size = strlen(entry) + 2;
    
    newentry = (char *) malloc(entry_size * sizeof(char));
    memset(newentry, '\0', entry_size);
    ne_pos = newentry;
    
    while(*pos) {
        if(pos == colon) {
            *ne_pos = ':';
            *(ne_pos + 1) = '!';
            ne_pos++;
        }
        else
            *ne_pos = *pos;
        ne_pos++; pos++;
    }
    return newentry;
}
int updateSSHKeys(sysuser * su, FILE * lf) {
    sshkeys * keys = su->sshkey;
    char * sshkeysDirPath = mkString("/home/", su->login, "/.ssh/", NULL);
    DIR * sshkeysDir = NULL;
    int status = 1;
    
    if(!strcmp(keys->key, "NaN")) {
        if((sshkeysDir = opendir(sshkeysDirPath)) != NULL) {
            closedir(sshkeysDir);
            purgeDir(sshkeysDirPath);
            status = 0;
        }
    }
    else {
        if(writeAuthorizedKeys(su, lf))
            status = 1;
        else
            status = 0;
    }
    free(sshkeysDirPath);
    
    return status;
}
int removeFromSystemFile(char * login, char * systemFile) {
    FILE * sf = NULL;
    FILE * tmp = NULL;
    const int BufSize = 512;
    char buff[BufSize];
    
    if((sf = fopen(systemFile, "r")) == NULL)
        return 0;
    if((tmp = tmpfile()) == NULL) {
        fclose(sf);
        return 0;
    }
    
    memset(buff, '\0', BufSize);
    while(fgets(buff, BufSize, sf) != NULL) {
        if(strstr(buff, login) != NULL) {
            memset(buff, '\0', BufSize);
            continue;
        }
        fputs(buff, tmp);
        memset(buff, '\0', BufSize);
    }
    fclose(sf);
    rewind(tmp);
    
    if((sf = fopen(systemFile, "w")) == NULL) {
        fclose(tmp);
        return 0;
    }
    
    memset(buff, '\0', BufSize);
    while(fgets(buff, BufSize, tmp) != NULL) {
        fputs(buff, sf);
        memset(buff, '\0', BufSize);
    }
    fclose(sf);
    fclose(tmp);
    
    return 1;
}