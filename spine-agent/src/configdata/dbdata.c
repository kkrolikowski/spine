#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include "core.h"
#include "commondata.h"
#include "dbdata.h"

vhostData * getVhostData(char * hostid) {
    // Zmienne umozliwiajace wyciaganie danych z bazy
    extern MYSQL * dbh;
    MYSQL_RES * res;
    MYSQL_ROW row;
    
    // zapytanie wyciagajace konfiguracje vhostow z bazy
    char * query = mkString("SELECT www.ServerName, www.ServerAlias, www.DocumentRoot, www.htaccess, sysusers.login AS user, ",
                            "configver.version AS config_ver, GROUP_CONCAT(DISTINCT www_opts.vhostopt SEPARATOR ' ') AS opts, ",
                            "GROUP_CONCAT(DISTINCT CONCAT(www_access.fromhost, ':', www_access.access_permission) SEPARATOR '#') AS accesslist, ",
                            "www.access_order, www.htpasswd AS password_access, case www.htpasswd WHEN 1 THEN GROUP_CONCAT(DISTINCT www_users.login ",
                            "SEPARATOR ' ') ELSE 'NaN' END AS htusers, www.status, www.purgedir, www.id, sysusers.uid FROM www JOIN sysusers ON sysusers.id = www.user_id JOIN ",
                            "sysinfo ON sysinfo.id = www.system_id JOIN www_opts_selected ON www_opts_selected.vhost_id = www.id JOIN ",
                            "www_opts ON www_opts.id = www_opts_selected.opt_id LEFT JOIN www_access ON www_access.vhost_id = www.id LEFT JOIN ",
                            "www_users_access ON (www_users_access.vhost_id = www.id AND www.htpasswd > 0) LEFT JOIN www_users ON ",
                            "(www_users.id = www_users_access.user_id AND www.htpasswd > 0) LEFT JOIN configver ON (configver.systemid = sysinfo.id AND ",
                            "configver.scope = 'apache') WHERE www.status NOT LIKE 'A' AND www.system_id = (SELECT id FROM sysinfo WHERE system_id = '", hostid ,
                            "') GROUP BY www.id", NULL);
    
    // obsluga listy odczytanych vhostow
    vhostData * head = NULL;
    vhostData * curr = NULL;
    vhostData * prev = NULL;
    
    // pobieranie danych z bazy i utworzenie listy laczonej
    if(!mysql_query(dbh, query)) {
        if((res = mysql_store_result(dbh)) != NULL) {
            if(mysql_num_rows(res) > 0) {
                while((row = mysql_fetch_row(res))) {
                    curr = (vhostData *) malloc(sizeof(vhostData));
    
                    curr->ServerName            = readData(row[0]);
                    curr->ServerAlias           = readData(row[1]);
                    curr->DocumentRoot          = readData(row[2]);
                    curr->htaccess              = readData(row[3]);
                    curr->user                  = readData(row[4]);
                    curr->version               = atoi(row[5]);
                    curr->apacheOpts            = readData(row[6]);
                    curr->vhost_access_list     = readData(row[7]);
                    curr->vhost_access_order    = readData(row[8]);
                    curr->password_access       = atoi(row[9]);
                    curr->htusers               = readData(row[10]);
                    curr->status                = readData(row[11]);
                    curr->purgedir              = readData(row[12]);
                    curr->dbid                  = atoi(row[13]);
                    curr->uid                   = atoi(row[14]);
                    
                    curr->next = NULL;

                    if(head == NULL)
                        head = curr;
                    else
                        prev->next = curr;
                    prev = curr;
                }
            }
        }
        mysql_free_result(res);
    }
    
    // czyszczenie pamieci
    free(query);
    
    return head;
}
htpasswdData * getHtpasswdData(char * hostid) {
    // Zmienne umozliwiajace wyciaganie danych z bazy
    extern MYSQL * dbh;
    MYSQL_RES * res;
    MYSQL_ROW row;
    
    // zapytanie pobierajace liste kont htpasswd
    char * query = mkString("SELECT ht.id, ht.login, ht.password, ht.status, cfg.version ",
                            "FROM www_users ht LEFT JOIN configver cfg ON ht.system_id = ",
                            "cfg.systemid AND cfg.scope = 'htusers' WHERE ht.status NOT LIKE 'A' ",
                            "AND ht.system_id = (SELECT id FROM sysinfo WHERE system_id = '",hostid,
                            "')", NULL);
    
    // obsluga listy kont htpasswd
    htpasswdData * head = NULL;
    htpasswdData * curr = NULL;
    htpasswdData * prev = NULL;
    
    size_t len = 0;     // zmienna pomocnicza do mierzenia wielkosci pamieci
    
    // pobieranie danych z bazy i utworzenie listy laczonej
    if(!mysql_query(dbh, query)) {
        if((res = mysql_store_result(dbh)) != NULL) {
            if(mysql_num_rows(res) > 0) {
                while((row = mysql_fetch_row(res))) {  
                    curr = (htpasswdData *) malloc(sizeof(htpasswdData));                   
                    // get ID
                    curr->dbid = atoi(row[0]);                    
                    // get login
                    len = strlen(row[1]) + 1;
                    curr->login = (char *) malloc(len * sizeof(char));
                    memset(curr->login, '\0', len);
                    strncpy(curr->login, row[1], len);                  
                    // get password
                    len = strlen(row[2]) + 1;
                    curr->pass = (char *) malloc(len * sizeof(char));
                    memset(curr->pass, '\0', len);
                    strncpy(curr->pass, row[2], len);
                    // get status
                    curr->status = row[3][0];
                    //version
                    curr->version = atoi(row[4]);
                    
                    // closing node
                    curr->next = NULL;

                    if(head == NULL)
                        head = curr;
                    else
                        prev->next = curr;
                    prev = curr;
                }
            }
        }
        mysql_free_result(res);
    }
    
    // czyszczenie pamieci
    free(query);
    
    return head;
}
sysuser * getSystemAccounts(char * systemid) {
    extern MYSQL * dbh;
    MYSQL_RES * res;
    MYSQL_ROW row;
    
    char * accountsInfo = mkString("SELECT u.id, u.login, u.pass, u.gecos, u.uid, u.active, u.expiration, ",
                                   "u.shell, CASE u.sshkeys WHEN 1 THEN GROUP_CONCAT(s.sshkey ",
                                   "SEPARATOR ',') ELSE 'NaN' END AS ssh_keys, cv.version AS config_ver, u.status, u.sudo FROM sysusers u LEFT JOIN ",
                                   "sysusers_sshkeys s ON (u.id = s.user_id AND u.sshkeys = 1) LEFT JOIN sysinfo si ",
                                   "ON u.system_id = si.id LEFT JOIN configver cv ON (cv.systemid = si.id AND cv.scope = 'sysusers') ",
                                   "WHERE u.login != 'root' AND u.system_id = (SELECT id FROM sysinfo WHERE system_id = '", systemid,"') GROUP BY u.id", NULL);
    
    // inicjujemy liste laczona w ktorej znajda sie dane odczytane z bazy
    sysuser * head = NULL;
    sysuser * curr = NULL;
    sysuser * prev = NULL;
    
    if(!mysql_query(dbh, accountsInfo)) {
        if((res = mysql_store_result(dbh)) != NULL) {
            while((row = mysql_fetch_row(res))) {               
                // inicjujemy wezel
                curr = (sysuser *) malloc(sizeof(sysuser));
                
                curr->dbid      = atoi(row[0]);         // id rekordu
                curr->login     = readData(row[1]);     // login
                curr->sha512    = readData(row[2]);     // haslo
                curr->gecos     = readData(row[3]);     // GECOS
                curr->uidgid    = atoi(row[4]);         // UID/GID usera
                curr->active    = atoi(row[5]);         // status konta
                
                // zapisujemy w pamieci informacje na temat expiracji konta
                if(!strcmp(row[5], "Never"))
                    curr->expiration = 0;
                else
                    curr->expiration = atoi(row[6]);
                
                // zapisujemy informacje, czy konto bedzie mialo dostep do shella
                curr->shellaccess = atoi(row[7]);
                
                // dolaczamy wezel pamieci z kluczami ssh
                curr->sshkey = getSSHkeys(row[8]);
                
               curr->version = atoi(row[9]);
               curr->status  = readData(row[10]);
               curr->sudo    = atoi(row[11]);         // dostep do roota
                
                // tworzymy kolejny wezel
                curr->next = NULL;
                if(head == NULL)
                    head = curr;
                else
                    prev->next = curr;
                prev = curr;
                
            }
            mysql_free_result(res);
        }
    }
    free(accountsInfo);
    
    return head;
}
sshkeys * getSSHkeys(char * str) {
    char buff[1024];
    memset(buff, '\0', 1024);
    int i = 0;
    size_t len = 0;

    sshkeys * head = NULL;
    sshkeys * curr = NULL;
    sshkeys * prev = NULL;

    while(*str) {
        if(*str != ',') {
            buff[i] = *str;
        }
        else {
            buff[i] = '\0';
            len = strlen(buff) + 1;
            curr = (sshkeys * ) malloc(sizeof(sshkeys));
            curr->key = (char *) malloc(len);
            memset(curr->key, '\0', len);
            strncpy(curr->key, buff, len);
            curr->next = NULL;
            if(head == NULL)
                head = curr;
            else
                prev->next = curr;
            prev = curr;
            i = 0;
            memset(buff, '\0', 1024);
            str++;
            continue;
        }
        i++; str++;
    }
    buff[i] = '\0';
    len = strlen(buff) + 1;
    curr = (sshkeys * ) malloc(sizeof(sshkeys));
    curr->key = (char *) malloc(len);
    memset(curr->key, '\0', len);
    strncpy(curr->key, buff, len);
    i = 0;
    memset(buff, '\0', 1024);
    curr->next = NULL;

    if(head == NULL)
        head = curr;
    else
        prev->next = curr;
    prev = curr;

    return head;
}
dbinfo * getDatabaseNames(char * systemid) {
    extern MYSQL * dbh;
    MYSQL_RES * res;
    MYSQL_ROW row;
    char * query = mkString("SELECT dn.id, dn.name AS dbname, dn.status, cv.version ",
                            "FROM db_name dn JOIN sysinfo s ON dn.host_id = s.id JOIN ",
                            "configver cv ON dn.host_id = cv.systemid AND cv.scope = ",
                            "'db_name' WHERE dn.status NOT LIKE 'A' AND system_id = '",
                            systemid,"'", NULL);
    dbinfo * head = NULL;
    dbinfo * curr = NULL;
    dbinfo * prev = NULL;
    
    if(mysql_query(dbh, query)) {
        free(query);
        return NULL;
    }
    if((res = mysql_store_result(dbh)) == NULL) {
        free(query);
        return NULL;
    }
    while((row = mysql_fetch_row(res))) {  
        curr = (dbinfo *) malloc(sizeof(dbinfo));
        
        curr->dbid      = atoi(row[0]);
        curr->dbname    = readData(row[1]);
        curr->status    = row[2][0];
        curr->version   = atoi(row[3]);
        curr->next      = NULL;
        
        if(head == NULL)
            head = curr;
        else
            prev->next = curr;
        prev = curr;
    }
    mysql_free_result(res);
    free(query);
    
    return head;
}
dbuser * getDatabaseUsers(char * systemid) {
    extern MYSQL * dbh;
    MYSQL_RES * res;
    MYSQL_ROW row;
    char * query = mkString("SELECT du.id, du.login AS dblogin, du.pass AS dbpass, ",
                            "du.status, cv.version FROM db_user du JOIN configver cv ",
                            "ON du.host_id = cv.systemid AND cv.scope = 'db_user' ",
                            "JOIN sysinfo s ON du.host_id = s.id WHERE du.status ",
                            "NOT LIKE 'A' AND s.system_id = '",systemid,"'", NULL);
    dbuser * head = NULL;
    dbuser * curr = NULL;
    dbuser * prev = NULL;
    
    if(mysql_query(dbh, query)) {
        free(query);
        return NULL;
    }
    if((res = mysql_store_result(dbh)) == NULL) {
        free(query);
        return NULL;
    }
    while((row = mysql_fetch_row(res))) {  
        curr = (dbuser *) malloc(sizeof(dbuser));
        
        curr->dbid      = atoi(row[0]);
        curr->login     = readData(row[1]);
        curr->pass      = readData(row[2]);
        curr->status    = row[3][0];
        curr->version   = atoi(row[4]);
        curr->next      = NULL;
        
        if(head == NULL)
            head = curr;
        else
            prev->next = curr;
        prev = curr;
    }
    mysql_free_result(res);
    free(query);
    
    return head;
}
grants * getDatabasePrivileges(char * systemid) {
    extern MYSQL * dbh;
    MYSQL_RES * res;
    MYSQL_ROW row;
    
    char * query = mkString("SELECT dp.id, dn.name AS dbname, du.login AS dblogin, ",
                            "dp.grants, dp.status, cv.version FROM db_privs dp JOIN ",
                            "db_user du ON dp.user_id = du.id JOIN db_name dn ON dp.db_id ",
                            " = dn.id JOIN sysinfo s ON du.host_id = s.id JOIN configver ",
                            "cv ON du.host_id = cv.systemid AND cv.scope = 'db_privs' ",
                            "WHERE dp.status NOT LIKE 'A' AND s.system_id = '",systemid,"'", NULL);
    grants * head = NULL;
    grants * curr = NULL;
    grants * prev = NULL;
    
    if(mysql_query(dbh, query)) {
        free(query);
        return NULL;
    }
    if((res = mysql_store_result(dbh)) == NULL) {
        free(query);
        return NULL;
    }
    while((row = mysql_fetch_row(res))) {  
        curr = (grants *) malloc(sizeof(grants));
        
        curr->dbid      = atoi(row[0]);
        curr->dbname    = readData(row[1]);
        curr->user      = readData(row[2]);
        curr->privs     = readData(row[3]);
        curr->status    = row[4][0];
        curr->version   = atoi(row[5]);
        curr->next      = NULL;
        
        if(head == NULL)
            head = curr;
        else
            prev->next = curr;
        prev = curr;
    }
    mysql_free_result(res);
    free(query);
    
    return head;
}