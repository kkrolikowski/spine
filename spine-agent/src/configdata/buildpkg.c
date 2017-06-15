#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buildpkg.h"
#include "core.h"
#include "commondata.h"

int getVhostPackageSize(vhostData * vhd, char * scope) {
    vhostData * curr = vhd;     // current node
    int size = 0;               // overrall data size
    int keySize = 0;            // key names size
    int nodeCount = 0;          // processed nodes count
    int dqCount   = 2;          // double quotes count in each position
    char * tmp = NULL;          // temporary string
    
    // package header 
    char * header = mkString("{scope:", scope, ",},", NULL);
    // package keys names
    const char * keys[] = { "DocumentRoot:,", "ServerAlias:,", "ServerName:,", "ApacheOpts:,",
                            "htaccess:,", "htusers:,", "purgedir:,", "vhoststatus:,", "sysuser:,",
                            "VhostAccessOrder:,", "VhostAccessList:,", "config_ver:", "uid:,",
                            "vhost_:", "dbid:,", "{},", "authbasic:,", NULL};
    const char ** key = keys;    
    
    while(*key)
        keySize += strlen(*key++);
    
    while(curr) {
        // string data
        size += strlen(curr->DocumentRoot)          + dqCount;
        size += strlen(curr->ServerAlias)           + dqCount;
        size += strlen(curr->ServerName)            + dqCount;
        size += strlen(curr->apacheOpts)            + dqCount;
        size += strlen(curr->htaccess)              + dqCount;
        size += strlen(curr->htusers)               + dqCount;
        size += strlen(curr->purgedir)              + dqCount;
        size += strlen(curr->status)                + dqCount;
        size += strlen(curr->user)                  + dqCount;
        size += strlen(curr->vhost_access_list)     + dqCount;
        size += strlen(curr->vhost_access_order)    + dqCount;

        // numeric data
        tmp = int2String(curr->password_access);
        size += strlen(tmp)                         + dqCount;
        free(tmp);
        
        tmp = int2String(nodeCount);
        size += strlen(tmp);
        free(tmp);
        
        tmp = int2String(curr->dbid);
        size += strlen(tmp)                         + dqCount;
        free(tmp);
        
        tmp = int2String(curr->uid);
        size += strlen(tmp)                         + dqCount;
        free(tmp);
        
        if(curr->next == NULL) {
            tmp = int2String(curr->version);
            size += strlen(tmp)                     + dqCount;
            free(tmp);
        }
                
        nodeCount++;
        curr = curr->next;
    }
    size += strlen(header);
    size += keySize * nodeCount;
    free(header);
    
    return size;
}
int htusersDataSize(htpasswdData * htpass, char * scope) {
    htpasswdData * curr = htpass;       // current node
    int size = 0;                       // overrall data size
    int keySize = 0;                    // key names size
    int nodeCount = 0;                  // processed nodes count
    int dqCount   = 2;                  // double quotes count in each position
    char * tmp = NULL;                  // temporary string

    // package header
    char * header = mkString("{scope:", scope, ",},", NULL);    
    // package keys names
    char * keys[] = { "user_:" "login:,", "password:,", "dbid:,", 
                      "status:,", "config_version:,", NULL 
                    };
    char ** key = keys;
    
    while(*key)
        keySize += strlen(*key++);
    
    while(curr) {
        size += strlen(curr->login)             + dqCount;
        size += strlen(curr->pass)              + dqCount;
        
        tmp = int2String(curr->dbid);
        size += strlen(tmp)                     + dqCount;
        free(tmp);
        
        // string length of each user index number
        tmp = int2String(nodeCount);
        size += strlen(tmp);
        free(tmp);
        
        size += 1;          // one byte for status flag in each item
        if(curr->next == NULL) {
            tmp = int2String(curr->version);
            size += strlen(tmp)                 + dqCount;
            free(tmp);
        }
        nodeCount++;
        curr = curr->next;
    }
    size += strlen(header);
    size += keySize * nodeCount;
    free(header);
    
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
int getSysUsersPackageSize(sysuser * su, char * scope) {
    sysuser * curr = su;    // current node
    int size = 0;           // overrall data size
    int keySize = 0;        // key names size
    int nodeCount = 0;      // processed nodes count
    int dqCount   = 2;      // double quotes count in each position
    char * tmp = NULL;      // temporary string
    
    // package header 
    char * header = mkString("{scope:", scope, ",},", NULL);
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
        size += strlen(curr->gecos)                 + dqCount;
        size += strlen(curr->login)                 + dqCount;
        size += strlen(curr->sha512)                + dqCount;
        size += strlen(curr->status)                + dqCount;
        
        // numeric data
        tmp = int2String(curr->dbid);
        size += strlen(tmp)                         + dqCount;
        free(tmp);
        tmp = int2String(curr->active);
        size += strlen(tmp)                         + dqCount;
        free(tmp);
        tmp = int2String(curr->expiration);
        size += strlen(tmp)                         + dqCount;
        free(tmp);
        tmp = int2String(curr->shellaccess);
        size += strlen(tmp)                         + dqCount;
        free(tmp);
        tmp = int2String(curr->uidgid);
        size += strlen(tmp)                         + dqCount;
        free(tmp);
        tmp = int2String(curr->sudo);
        size += strlen(tmp)                         + dqCount;
        free(tmp);
        tmp = int2String(nodeCount);
        size += strlen(tmp)                         + dqCount;
        free(tmp);
        
        if(curr->next == NULL) {
            tmp = int2String(curr->version);
            size += strlen(tmp)                     + dqCount;
            free(tmp);
        }
        // ssh keys
        size += getSSHkeysPackageSize(curr->sshkey);
        
        nodeCount++;
        curr = curr->next;
    }
    size += strlen(header);
    size += keySize * nodeCount;
    free(header);

    return size;
}
int DBnamesDataSize(dbinfo * db, char * scope) {
    dbinfo * curr = db;                 // current node
    int size = 0;                       // overrall data size
    int keySize = 0;                    // key names size
    int nodeCount = 0;                  // processed nodes count
    int dqCount   = 2;                  // double quotes count in each position
    char * tmp = NULL;                  // temporary string

    // package header
    char * header = mkString("{scope:", scope, ",},", NULL);
    // package keys names
    char * keys[] = { "dbnum_:", "dbname:,", "dbid:,", "status:,", 
                      "config_version:,", NULL 
                    };
    char ** key = keys;
    
    while(*key)
        keySize += strlen(*key++);
    
    while(curr) {
        size += strlen(curr->dbname)            + dqCount;
        
        tmp = int2String(curr->dbid);
        size += strlen(tmp)                     + dqCount;
        free(tmp);
        
        // string length of each user index number
        tmp = int2String(nodeCount);
        size += strlen(tmp);
        free(tmp);
        
        size += 1;          // one byte for status flag in each item
        if(curr->next == NULL) {
            tmp = int2String(curr->version);
            size += strlen(tmp)                 + dqCount;
            free(tmp);
        }
        nodeCount++;
        curr = curr->next;
    }
    size += strlen(header);
    size += keySize * nodeCount;
    free(header);
    
    return size;
}
int DBusersDataSize(dbuser * db, char * scope) {
    dbuser * curr = db;                 // current node
    int size = 0;                       // overrall data size
    int keySize = 0;                    // key names size
    int nodeCount = 0;                  // processed nodes count
    int dqCount   = 2;                  // double quotes count in each position
    char * tmp = NULL;                  // temporary string

    // package header
    char * header = mkString("{scope:", scope, ",},", NULL);    
    // package keys names
    char * keys[] = { "dbusernum_:", "dblogin:,", "dbid:,", "status:,", 
                      "dbpass:,", "config_version:,", NULL 
                    };
    char ** key = keys;
    
    while(*key)
        keySize += strlen(*key++);
    
    while(curr) {
        size += strlen(curr->login)            + dqCount;
        size += strlen(curr->pass)             + dqCount;
        
        tmp = int2String(curr->dbid);
        size += strlen(tmp)                     + dqCount;
        free(tmp);
        
        // string length of each user index number
        tmp = int2String(nodeCount);
        size += strlen(tmp);
        free(tmp);
        
        size += 1;          // one byte for status flag in each item
        if(curr->next == NULL) {
            tmp = int2String(curr->version);
            size += strlen(tmp)                 + dqCount;
            free(tmp);
        }
        nodeCount++;
        curr = curr->next;
    }
    size += strlen(header);
    size += keySize * nodeCount;
    free(header);
    
    return size;
}
int DBgrantsDataSize(grants * db, char * scope) {
    grants * curr = db;                 // current node
    int size = 0;                       // overrall data size
    int keySize = 0;                    // key names size
    int nodeCount = 0;                  // processed nodes count
    int dqCount   = 2;                  // double quotes count in each position
    char * tmp = NULL;                  // temporary string

    // package header
    char * header = mkString("{scope:", scope, ",},", NULL);    
    // package keys names
    char * keys[] = { "dbprivnum_:", "dbid:,", "dblogin:,", "dbname:,", 
                      "grants:,", "status:,", "config_version:,", NULL 
                    };
    char ** key = keys;
    
    while(*key)
        keySize += strlen(*key++);
    
    while(curr) {
        size += strlen(curr->dbname)            + dqCount;
        size += strlen(curr->user)              + dqCount;
        size += strlen(curr->privs)             + dqCount;
        
        tmp = int2String(curr->dbid);
        size += strlen(tmp)                     + dqCount;
        free(tmp);
        
        // string length of each user index number
        tmp = int2String(nodeCount);
        size += strlen(tmp);
        free(tmp);
        
        size += 1;          // one byte for status flag in each item
        if(curr->next == NULL) {
            tmp = int2String(curr->version);
            size += strlen(tmp)                 + dqCount;
            free(tmp);
        }
        nodeCount++;
        curr = curr->next;
    }
    size += strlen(header);
    size += keySize * nodeCount;
    free(header);
    
    return size;
}
char * apacheConfigPackage(vhostData * www, char * scope) {
    // common variables
    int size               = 0;         // package size
    int idx                = 0;		// vhost index
    char * package         = NULL;      // output package
    char * numstr          = NULL;	// vhost index as a string
    char * entry           = NULL;      // particular vhost definition
    char * s_dbid          = NULL;      // DB ID in a form of string
    char * s_config_ver    = NULL;      // configuration version

    // specific data
    vhostData * curr       = www;       // node traversing pointer
    char * uid             = NULL;      // UNIX uid of the owner of vhost
    char * authbasic       = NULL;	// authbasic flag

    // package header
    char * header = mkString("{scope:", scope, ",", NULL);

    if(www == NULL)
        return NULL;
    
    // preparing memory
    size = getVhostPackageSize(www, scope) + 1;
    package = (char *) malloc(size * sizeof(char));
    memset(package, '\0', size);

    // package building
    strncpy(package, header, strlen(header)); // configuration type
    while(curr) {
        numstr = int2String(idx);
        authbasic = int2String(curr->password_access);
        s_dbid = int2String(curr->dbid);
        uid = int2String(curr->uid);
        entry = mkString(
                        "vhost_", numstr, ":{",
                        "dbid:\"",             s_dbid,                   "\",",
                        "ServerName:\"",       curr->ServerName,         "\",",
                        "ServerAlias:\"",      curr->ServerAlias,        "\",",
                        "DocumentRoot:\"",     curr->DocumentRoot,       "\",",
                        "ApacheOpts:\"",       curr->apacheOpts,         "\",",
                        "VhostAccessOrder:\"", curr->vhost_access_order, "\",",
                        "VhostAccessList:\"",  curr->vhost_access_list,  "\",",
                        "htaccess:\"",         curr->htaccess,           "\",",
                        "authbasic:\"",        authbasic,                "\",",
                        "htusers:\"",          curr->htusers,            "\",",
                        "vhoststatus:\"",      curr->status,             "\",",
                        "purgedir:\"",         curr->purgedir,           "\",",
                        "uid:\"",              uid,                      "\",",
                        "sysuser:\"",          curr->user,               "\"}",
                        ",", NULL);
        strncat(package, entry, strlen(entry) + 1);
        if(curr->next == NULL)
            s_config_ver = int2String(curr->version);

        // zwalniamy pamiec i przygotowujemy zmienne do kolejnej iteracji
        free(s_dbid);
        free(entry);
        free(numstr);
        free(authbasic);
        free(uid);
        if(strcmp(curr->ServerName, "NaN"))
            idx++;
        curr = curr->next;
    }

    // dodatkowe dane: liczba vhostow, ktore zostaly odczytane
    strncat(package, "config_ver:", strlen("config_ver:"));
    strncat(package, s_config_ver, strlen(s_config_ver));
    strncat(package, "},", 2);

    // czyscimy niepotrzebne dane
    free(s_config_ver);
    free(header);
    cleanVhostData(www);

    return package;
}
char * htpasswdConfigPackage(htpasswdData * htpass, char * scope) {
    // common variables
    int size               = 0;         // package size
    int idx                = 0;		// vhost index
    char * package         = NULL;      // output package
    char * numstr          = NULL;	// vhost index as a string
    char * entry           = NULL;      // particular vhost definition
    char * s_dbid          = NULL;      // DB ID in a form of string
    char * s_config_ver    = NULL;      // configuration version
    
    // specific data
    htpasswdData * curr     = htpass;   // node traversing pointer
    char status[2];                     // status flags can be: NUDA

    // package header
    char * header = mkString("{scope:", scope, ",", NULL);

    if(htpass == NULL)
        return NULL;
    
    // preparing memory
    size = htusersDataSize(htpass, scope) + 1;
    package = (char *) malloc(size * sizeof(char));
    memset(package, '\0', size);
  
    // building a package
    strncpy(package, header, strlen(header));
    while(curr) {
        numstr = int2String(idx);
        s_dbid = int2String(curr->dbid);
        memset(status, '\0', 2);
        status[0] = curr->status;
        
        entry = mkString(
                        "user_",              numstr,         ":{",
                        "dbid:\"",            s_dbid,         "\",",
                        "login:\"",           curr->login,    "\",",
                        "password:\"",        curr->pass,     "\",",
                        "status:\"",          status,         "\",",
                        NULL);
        
        strncat(package, entry, strlen(entry) + 1);
        if(curr->next == NULL)
            s_config_ver = int2String(curr->version);

        // zwalniamy pamiec i przygotowujemy zmienne do kolejnej iteracji
        free(s_dbid);
        free(entry);
        free(numstr);
        idx++;
        curr = curr->next;
    }

    // dodatkowe dane: liczba vhostow, ktore zostaly odczytane
    strncat(package, "config_ver:", strlen("config_ver:"));
    strncat(package, s_config_ver, strlen(s_config_ver));
    strncat(package, "},", 2);

    // czyscimy niepotrzebne dane
    free(s_config_ver);
    free(header);
    clearHtpasswdData(htpass);

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
char * sysusersPackage(sysuser * su, char * scope) {
    // common variables
    int size               = 0;         // package size
    int idx                = 0;		// vhost index
    char * package         = NULL;      // output package
    char * numstr          = NULL;	// vhost index as a string
    char * entry           = NULL;      // particular vhost definition
    char * s_dbid          = NULL;      // DB ID in a form of string
    char * s_config_ver    = NULL;      // configuration version
    
    // specific data
    sysuser * curr          = su;       // node traversing pointer
    char * keyentry         = NULL;     // sshkeys belonging to user
    char * s_expval_val     = NULL;     // account expiration
    char * s_uidgid_val     = NULL;     // UID/GID
    char * s_active_val     = NULL;     // 1: account is active, 0: inactive
    char * s_shell_val      = NULL;     // 1: account have shell access, 0: doesn't
    char * s_sudo_val       = NULL;     // 1: admin privileges, 0: regular user
    
    // package header
    char * header = mkString("{scope:", scope, ",", NULL);
    
    if(su == NULL)
        return NULL;
    
    // preparing memory
    size = getSysUsersPackageSize(su, scope);
    package = (char *) malloc(size * sizeof(char));
    memset(package, '\0', size);
    
    // building a package
    strncpy(package, header, strlen(header));
    while(curr) {
        numstr       = int2String(idx);
        s_dbid       = int2String(curr->dbid);
        s_expval_val = int2String(curr->expiration);
        s_uidgid_val = int2String(curr->uidgid);
        s_active_val = int2String(curr->active);
        s_shell_val  = int2String(curr->shellaccess);
        s_sudo_val   = int2String(curr->sudo);
        keyentry     = sshkeysPackage(curr->sshkey);
        
        entry = mkString(
                        "user_",          numstr,          ":{",
                        "dbid:\"",        s_dbid,          "\",",
                        "username:\"",    curr->login,     "\",",
                        "password:\"",    curr->sha512,    "\",",
                        "gecos:\"",       curr->gecos,     "\",",
                        "expire:\"",      s_expval_val,    "\",",
                        "uidgid:\"",      s_uidgid_val,    "\",",
                        "active:\"",      s_active_val,    "\",",
                        "shell:\"",       s_shell_val,     "\",",
                        "sudo:\"",        s_sudo_val,      "\",",
                        "status:\"",      curr->status,    "\",",
                        keyentry,                        NULL
                    );
        strncat(package, entry, strlen(entry) + 1);
        strncat(package, "},", 3);
        if(curr->next == NULL)
            s_config_ver = int2String(curr->version);
              
        if(keyentry != NULL)     free(keyentry);
        if(entry != NULL)        free(entry);
        if(numstr != NULL)       free(numstr);
        if(s_expval_val != NULL) free(s_expval_val);
        if(s_uidgid_val != NULL) free(s_uidgid_val);
        if(s_active_val != NULL) free(s_active_val);
        if(s_shell_val != NULL)  free(s_shell_val);
        if(s_sudo_val != NULL)   free(s_sudo_val);
        if(s_dbid != NULL)       free(s_dbid);
        
        idx++;
        curr = curr->next;
    }
    strncat(package, "config_ver:", strlen("config_ver:"));
    strncat(package, s_config_ver, strlen(s_config_ver));
    strncat(package, "}", 2);
    
    free(s_config_ver);
    free(header);
    cleanSysUsersData(su);
    
    return package;
}
char * DBNamesConfigPackage(dbinfo * db, char * scope) {
    // common variables
    int size               = 0;         // package size
    int idx                = 0;		// vhost index
    char * package         = NULL;      // output package
    char * numstr          = NULL;	// vhost index as a string
    char * entry           = NULL;      // particular vhost definition
    char * s_dbid          = NULL;      // DB ID in a form of string
    char * s_config_ver    = NULL;      // configuration version
    
    // specific data
    dbinfo * curr          = db;        // node traversing pointer
    char status[2];                     // status flags can be: NUDA
    
    // package header
    char * header = mkString("{scope:", scope,",", NULL);

    if(db == NULL)
        return NULL;
    
    // preparing memory
    size = DBnamesDataSize(db, scope) + 1;
    package = (char *) malloc(size * sizeof(char));
    memset(package, '\0', size);

    // package building
    strncpy(package, header, strlen(header)); // configuration type
    while(curr) {
        numstr = int2String(idx);
        s_dbid = int2String(curr->dbid);
        memset(status, '\0', 2);
        status[0] = curr->status;
        entry = mkString(
                        "dbnum_", numstr, ":{",
                        "dbid:\"",             s_dbid,         "\",",
                        "dbname:\"",           curr->dbname,   "\",",
                        "status:\"",           status,         "\"}",
                        ",", NULL);
        strncat(package, entry, strlen(entry) + 1);
        if(curr->next == NULL)
            s_config_ver = int2String(curr->version);

        // zwalniamy pamiec i przygotowujemy zmienne do kolejnej iteracji
        free(s_dbid);
        free(entry);
        free(numstr);
        idx++;
        curr = curr->next;
    }

    // dodatkowe dane: liczba vhostow, ktore zostaly odczytane
    strncat(package, "config_ver:", strlen("config_ver:"));
    strncat(package, s_config_ver, strlen(s_config_ver));
    strncat(package, "},", 2);

    // czyscimy niepotrzebne dane
    free(s_config_ver);
    free(header);
    cleanDBinfoData(db);

    return package;
}
char * DBusersConfigPackage(dbuser * db, char * scope) {
    // common variables
    int size               = 0;         // package size
    int idx                = 0;		// vhost index
    char * package         = NULL;      // output package
    char * numstr          = NULL;	// vhost index as a string
    char * entry           = NULL;      // particular vhost definition
    char * s_dbid          = NULL;      // DB ID in a form of string
    char * s_config_ver    = NULL;      // configuration version
    
    // specific data
    dbuser * curr          = db;        // node traversing pointer
    char status[2];                     // status flags can be: NUDA
    
    // package header
    char * header = mkString("{scope:", scope,",", NULL);

    if(db == NULL)
        return NULL;
    
    // preparing memory
    size = DBusersDataSize(db, scope) + 1;
    package = (char *) malloc(size * sizeof(char));
    memset(package, '\0', size);

    // package building
    strncpy(package, header, strlen(header)); // configuration type
    while(curr) {
        numstr = int2String(idx);
        s_dbid = int2String(curr->dbid);
        memset(status, '\0', 2);
        status[0] = curr->status;
        entry = mkString(
                        "dbusernum_",    numstr,      ":{",
                        "dbid:\"",       s_dbid,      "\",",
                        "dblogin:\"",    curr->login, "\",",
                        "dbpass:\"",     curr->pass,  "\",",
                        "status:\"",     status,      "\"}",
                        ",", NULL);
        strncat(package, entry, strlen(entry) + 1);
        if(curr->next == NULL)
            s_config_ver = int2String(curr->version);

        // zwalniamy pamiec i przygotowujemy zmienne do kolejnej iteracji
        free(s_dbid);
        free(entry);
        free(numstr);
        idx++;
        curr = curr->next;
    }

    // dodatkowe dane: liczba vhostow, ktore zostaly odczytane
    strncat(package, "config_ver:", strlen("config_ver:"));
    strncat(package, s_config_ver, strlen(s_config_ver));
    strncat(package, "},", 2);

    // czyscimy niepotrzebne dane
    free(s_config_ver);
    free(header);
    cleanDBusersData(db);

    return package;
}
char * DBgrantsConfigPackage(grants * db, char * scope) {
    // common variables
    int size               = 0;         // package size
    int idx                = 0;		// vhost index
    char * package         = NULL;      // output package
    char * numstr          = NULL;	// vhost index as a string
    char * entry           = NULL;      // particular vhost definition
    char * s_dbid          = NULL;      // DB ID in a form of string
    char * s_config_ver    = NULL;      // configuration version

    // specific data
    grants * curr          = db;        // node traversing pointer
    char status[2];                     // status flags can be: NUDA
    
    // package header
    char * header = mkString("{scope:", scope,",", NULL);

    if(db == NULL)
        return NULL;
    
    // preparing memory
    size = DBgrantsDataSize(db, scope) + 1;
    package = (char *) malloc(size * sizeof(char));
    memset(package, '\0', size);

    // package building
    strncpy(package, header, strlen(header)); // configuration type
    while(curr) {
        numstr = int2String(idx);
        s_dbid = int2String(curr->dbid);
        memset(status, '\0', 2);
        status[0] = curr->status;
        entry = mkString(
                        "dbprivnum_",    numstr,        ":{",
                        "dbid:\"",       s_dbid,        "\",",
                        "dblogin:\"",    curr->user,    "\",",
                        "dbname:\"",     curr->dbname,  "\",",
                        "grants:\"",     curr->privs,   "\",",   
                        "status:\"",     status,        "\"}",
                        ",", NULL);
        strncat(package, entry, strlen(entry) + 1);
        if(curr->next == NULL)
            s_config_ver = int2String(curr->version);

        // zwalniamy pamiec i przygotowujemy zmienne do kolejnej iteracji
        free(s_dbid);
        free(entry);
        free(numstr);
        idx++;
        curr = curr->next;
    }

    // dodatkowe dane: liczba vhostow, ktore zostaly odczytane
    strncat(package, "config_ver:", strlen("config_ver:"));
    strncat(package, s_config_ver, strlen(s_config_ver));
    strncat(package, "},", 2);

    // czyscimy niepotrzebne dane
    free(s_config_ver);
    free(header);
    cleanDBgrantsData(db);

    return package;
}
void cleanVhostData(vhostData * vhd) {
    vhostData * curr = vhd;
    vhostData * next = NULL;
    
    free(curr->DocumentRoot);
    free(curr->ServerAlias);
    free(curr->ServerName);
    free(curr->apacheOpts);
    free(curr->htaccess);
    free(curr->htusers);
    free(curr->purgedir);
    free(curr->status);
    free(curr->user);
    free(curr->vhost_access_list);
    free(curr->vhost_access_order);
    
    next = curr->next;
    if(next != NULL)
        cleanVhostData(next);
    free(curr);
}
void clearHtpasswdData(htpasswdData * htpasswd) {
    htpasswdData * curr = htpasswd;
    htpasswdData * next = NULL;
    
    free(curr->login);
    free(curr->pass);
    
    next = curr->next;
    if(next != NULL)
        clearHtpasswdData(next);
    free(curr);
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
void cleanDBinfoData(dbinfo * db) {
    dbinfo * curr = db;
    dbinfo * next = NULL;
    
    free(curr->dbname);
    
    next = curr->next;
    if(next != NULL)
        cleanDBinfoData(next);
    free(curr);
}
void cleanDBusersData(dbuser * db) {
    dbuser * curr = db;
    dbuser * next = NULL;
    
    free(curr->login);
    free(curr->pass);
    
    next = curr->next;
    if(next != NULL)
        cleanDBusersData(next);
    free(curr);
}
void cleanDBgrantsData(grants * db) {
    grants * curr = db;
    grants * next = NULL;
    
    free(curr->dbname);
    free(curr->user);
    free(curr->privs);
    
    next = curr->next;
    if(next != NULL)
        cleanDBgrantsData(next);
    free(curr);
}