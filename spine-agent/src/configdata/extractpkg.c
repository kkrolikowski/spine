#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "commondata.h"
#include "toolkit.h"
#include "extractpkg.h"

sysuser * ParseConfigDataSYSUSERS(char * json) {
    char * offset   = NULL;             // relative position in input string
    char * iheader  = NULL;             // this is header of data portion
    char * idx      = NULL;             // actual position of user data
    char * tmp      = NULL;             // helper variable for converting string into numeric vals
    int i           = 0;                // actual number of processed data items
    int cfgver      = 0;                // config version
    
    // when to end reading input data
    char * end      = strstr(json, "config_ver:");
    
    // system accounts data nodes
    sysuser * head = NULL;
    sysuser * curr = NULL;
    sysuser * prev = NULL;
    
    // processing config version
    tmp = getOptVal(json, "config_ver");
    cfgver = atoi(tmp);
    free(tmp);
    
    idx = int2String(i);
    iheader = mkString("user_", idx, NULL);
    while((offset = strstr(json, iheader)) != NULL && offset < end) {
        curr = (sysuser *) malloc(sizeof(sysuser));
        
        curr->gecos         = getOptVal(offset, "gecos");
        curr->login         = getOptVal(offset, "username");
        curr->sha512        = getOptVal(offset, "password");
        curr->status        = getOptVal(offset, "status");
        
        tmp                 = getOptVal(offset, "dbid");
        curr->dbid          = atoi(tmp);
        free(tmp);
        tmp                 = getOptVal(offset, "active");
        curr->active        = atoi(tmp);
        free(tmp);
        tmp                 = getOptVal(offset, "uidgid");
        curr->uidgid        = atoi(tmp);
        free(tmp);
        tmp                 = getOptVal(offset, "shell");
        curr->shellaccess   = atoi(tmp);
        free(tmp);
        tmp                 = getOptVal(offset, "expire");
        curr->expiration    = atoi(tmp);
        free(tmp);
        tmp                 = getOptVal(offset, "sudo");
        curr->sudo          = atoi(tmp);
        free(tmp);
        curr->version       = cfgver;
        
        curr->sshkey        = ParseConfigDataSSHKeys(strstr(offset, "sshkey_0"));
        curr->next = NULL;
        
        if(head == NULL)
            head = curr;
        else
            prev->next = curr;
        prev = curr;      
        
        free(iheader);
        free(idx);
        
        i++;
        idx = int2String(i);
        iheader = mkString("user_", idx, NULL);
    }
    return head;  
}
htpasswdData * ParseConfigDataHTPASSWD(char * json) {
    char * offset       = NULL;        // relative position in input string
    char * iheader      = NULL;        // this is header of data portion
    char * idx          = NULL;        // actual position of user data
    char * tmp          = NULL;        // helper variable for converting string into numeric vals
    int i               = 0;           // actual number of processed data items
    int cfgver          = 0;           // config version
    
    // when to end reading input data
    char * end          = strstr(json, "config_ver:");
    
    // inicjalizacja danych do listy laczonej
    htpasswdData * head = NULL;
    htpasswdData * curr = NULL;
    htpasswdData * prev = NULL;
    
    // processing config version
    tmp = getOptVal(json, "config_ver");
    cfgver = atoi(tmp);
    free(tmp);
    
    idx = int2String(i);
    iheader = mkString("user_", idx, NULL);
    while((offset = strstr(json, iheader)) != NULL && offset < end) {
        curr = (htpasswdData *) malloc(sizeof(htpasswdData));
        
        curr->login         = getOptVal(offset, "login");
        curr->pass          = getOptVal(offset, "password");
        tmp                 = getOptVal(offset, "status");
        curr->status        = tmp[0];
        free(tmp);
        
        tmp                 = getOptVal(offset, "dbid");
        curr->dbid          = atoi(tmp);
        free(tmp);
        curr->version       = cfgver;
        curr->next = NULL;
        
        if(head == NULL)
            head = curr;
        else
            prev->next = curr;
        prev = curr;      
        
        free(iheader);
        free(idx);
        
        i++;
        idx = int2String(i);
        iheader = mkString("user_", idx, NULL);
    }
    free(idx);
    free(iheader);
    
    return head;  
}
vhostData * ParseConfigDataAPACHE(char * json) {
    char * offset       = NULL;         // relative position in input string
    char * iheader      = NULL;         // this is header of data portion
    char * idx          = NULL;         // actual position of user data
    char * tmp          = NULL;         // tmp string for converting data to numeric types
    int i               = 0;            // actual number of processed data items
    int cfgver          = 0;            // configuration version
    
    // when to end reading input data
    char * end          = strstr(json, "config_ver:");
    
    // apache vhosts data nodes
    vhostData * curr = NULL;
    vhostData * prev = NULL;
    vhostData * head = NULL;
    
    // processing config version
    tmp = getOptVal(json, "config_ver");
    cfgver = atoi(tmp);
    free(tmp);
    
    idx = int2String(i);
    iheader = mkString("vhost_", idx, NULL);
    while((offset = strstr(json, iheader)) != NULL && offset < end) {
        
        curr = (vhostData *) malloc(sizeof(vhostData));
        curr->ServerName            = getOptVal(offset, "ServerName");
        curr->ServerAlias           = getOptVal(offset, "ServerAlias");
        curr->DocumentRoot          = getOptVal(offset, "DocumentRoot");
        curr->apacheOpts            = getOptVal(offset, "ApacheOpts");
        curr->vhost_access_order    = getOptVal(offset, "VhostAccessOrder");
        curr->vhost_access_list     = getOptVal(offset, "VhostAccessList");
        curr->htaccess              = getOptVal(offset, "htaccess");
        curr->version               = cfgver;
        curr->user                  = getOptVal(offset, "sysuser");
        curr->htusers               = getOptVal(offset, "htusers");
        curr->status                = getOptVal(offset, "vhoststatus");
        curr->purgedir              = getOptVal(offset, "purgedir");
        
        tmp                         = getOptVal(offset, "authbasic");
        curr->password_access       = atoi(tmp);
        free(tmp);
        tmp                         = getOptVal(offset, "dbid");
        curr->dbid                  = atoi(tmp);
        free(tmp);
        tmp                         = getOptVal(offset, "uid");
        curr->uid                   = atoi(tmp);
        free(tmp);
        
        curr->next = NULL;
        
        if(head == NULL)
            head = curr;
        else
            prev->next = curr;
        prev = curr;
        
        free(iheader);
        free(idx);
        
        i++;
        idx = int2String(i);
        iheader = mkString("vhost_", idx, NULL);
    }
    
    return head;
}
dbinfo * ParseConfigDataDBNAMES(char * json) {
    char * offset       = NULL;        // relative position in input string
    char * iheader      = NULL;        // this is header of data portion
    char * idx          = NULL;        // actual position of user data
    char * tmp          = NULL;        // helper variable for converting string into numeric vals
    int i               = 0;           // actual number of processed data items
    int cfgver          = 0;           // config version
    
    // when to end reading input data
    char * end          = strstr(json, "config_ver:");
    
    // inicjalizacja danych do listy laczonej
    dbinfo * head = NULL;
    dbinfo * curr = NULL;
    dbinfo * prev = NULL;
    
    // processing config version
    tmp = getOptVal(json, "config_ver");
    cfgver = atoi(tmp);
    free(tmp);
    
    idx = int2String(i);
    iheader = mkString("dbnum_", idx, NULL);
    while((offset = strstr(json, iheader)) != NULL && offset < end) {
        curr = (dbinfo *) malloc(sizeof(dbinfo));
        
        curr->dbname        = getOptVal(offset, "dbname");
        tmp                 = getOptVal(offset, "status");
        curr->status        = tmp[0];
        free(tmp);
        
        tmp                 = getOptVal(offset, "dbid");
        curr->dbid          = atoi(tmp);
        free(tmp);
        curr->version       = cfgver;
        curr->next = NULL;
        
        if(head == NULL)
            head = curr;
        else
            prev->next = curr;
        prev = curr;      
        
        free(iheader);
        free(idx);
        
        i++;
        idx = int2String(i);
        iheader = mkString("dbnum_", idx, NULL);
    }
    free(idx);
    free(iheader);
    
    return head;  
}
dbuser * ParseConfigDataDBUSERS(char * json) {
    char * offset       = NULL;        // relative position in input string
    char * iheader      = NULL;        // this is header of data portion
    char * idx          = NULL;        // actual position of user data
    char * tmp          = NULL;        // helper variable for converting string into numeric vals
    int i               = 0;           // actual number of processed data items
    int cfgver          = 0;           // config version
    
    // when to end reading input data
    char * end          = strstr(json, "config_ver:");
    
    // inicjalizacja danych do listy laczonej
    dbuser * head = NULL;
    dbuser * curr = NULL;
    dbuser * prev = NULL;
    
    // processing config version
    tmp = getOptVal(json, "config_ver");
    cfgver = atoi(tmp);
    free(tmp);
    
    idx = int2String(i);
    iheader = mkString("dbusernum_", idx, NULL);
    while((offset = strstr(json, iheader)) != NULL && offset < end) {
        curr = (dbuser *) malloc(sizeof(dbuser));
        
        curr->login         = getOptVal(offset, "dblogin");
        curr->pass          = getOptVal(offset, "dbpass");
        tmp                 = getOptVal(offset, "status");
        curr->status        = tmp[0];
        free(tmp);
        
        tmp                 = getOptVal(offset, "dbid");
        curr->dbid          = atoi(tmp);
        free(tmp);
        curr->version       = cfgver;
        curr->next = NULL;
        
        if(head == NULL)
            head = curr;
        else
            prev->next = curr;
        prev = curr;      
        
        free(iheader);
        free(idx);
        
        i++;
        idx = int2String(i);
        iheader = mkString("dbusernum_", idx, NULL);
    }
    free(idx);
    free(iheader);
    
    return head;  
}
grants * ParseConfigDataDBPRIVS(char * json) {
    char * offset       = NULL;        // relative position in input string
    char * iheader      = NULL;        // this is header of data portion
    char * idx          = NULL;        // actual position of user data
    char * tmp          = NULL;        // helper variable for converting string into numeric vals
    int i               = 0;           // actual number of processed data items
    int cfgver          = 0;           // config version
    
    // when to end reading input data
    char * end          = strstr(json, "config_ver:");
    
    // inicjalizacja danych do listy laczonej
    grants * head = NULL;
    grants * curr = NULL;
    grants * prev = NULL;
    
    // processing config version
    tmp = getOptVal(json, "config_ver");
    cfgver = atoi(tmp);
    free(tmp);
    
    idx = int2String(i);
    iheader = mkString("dbprivnum_", idx, NULL);
    while((offset = strstr(json, iheader)) != NULL && offset < end) {
        curr = (grants *) malloc(sizeof(grants));
        
        curr->dbname        = getOptVal(offset, "dbname");
        curr->user          = getOptVal(offset, "dblogin");
        curr->privs         = getOptVal(offset, "grants");
        tmp                 = getOptVal(offset, "status");
        curr->status        = tmp[0];
        free(tmp);
        
        tmp                 = getOptVal(offset, "dbid");
        curr->dbid          = atoi(tmp);
        free(tmp);
        curr->version       = cfgver;
        curr->next = NULL;
        
        if(head == NULL)
            head = curr;
        else
            prev->next = curr;
        prev = curr;      
        
        free(iheader);
        free(idx);
        
        i++;
        idx = int2String(i);
        iheader = mkString("dbprivnum_", idx, NULL);
    }
    free(idx);
    free(iheader);
    
    return head;  
}
sshkeys * ParseConfigDataSSHKeys(char * str) {
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