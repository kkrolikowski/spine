/* database.c -- zestaw funkcji, ktore operuja na bazie danych */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include "core.h"
#include "sysconfigdata.h"
#include "database.h"
#include "monitoring.h"
#include "sysusers.h"
#include "commondata.h"

void InitDBConnData(dbconn db) {
	db.dbhost = NULL;
	db.dblogin = NULL;
	db.dbname = NULL;
	db.dbpass = NULL;
}
void clearDBConnData(dbconn db) {
	free(db.dbhost);
	free(db.dblogin);
	free(db.dbname);
	free(db.dbpass);
}
MYSQL * dbConnect(dbconn cfg) {
	MYSQL * dbh = mysql_init(NULL);

	if(mysql_real_connect(dbh, cfg.dbhost, cfg.dblogin, cfg.dbpass, cfg.dbname, 3306, NULL, 0) == NULL)
		return NULL;

	return dbh;
}
int updateHostInfo(char * clientip, char * stream, FILE * lf) {
	systeminfo hostinfo;
        //netifstats netinfo;
	char * logmessage = NULL;
	char * uptime_s = NULL;
	char * hdd_total_s = NULL;
	char * hdd_free_s = NULL;
	char * ram_total_s = NULL;
	char * ram_free_s = NULL;
	char * curr_time_s = NULL;
        char * net_bytes_in_s = NULL;
        char * net_bytes_out_s = NULL;

	// odczytujemy informacje z jsona
	InitSystemInformation(&hostinfo);

	uptime_s = getOptVal(stream, "uptime");
	hostinfo.uptime = atol(uptime_s);
	hostinfo.net_hwaddr = getOptVal(stream, "systemid");
	hostinfo.hostname = getOptVal(stream, "hostname");
	hostinfo.os = getOptVal(stream, "distro_name");
	hdd_total_s = getOptVal(stream, "hdd_total");
	hostinfo.hdd_total = atol(hdd_total_s);
	hdd_free_s = getOptVal(stream, "hdd_free");
	hostinfo.hdd_free = atol(hdd_free_s);
	ram_total_s = getOptVal(stream, "ram_total");
	hostinfo.ram_total = atol(ram_total_s);
	ram_free_s = getOptVal(stream, "ram_free");
	hostinfo.ram_free = atol(ram_free_s);
	curr_time_s = getOptVal(stream, "curr_time");
	hostinfo.curr_time = atol(curr_time_s);
	hostinfo.extip = getOptVal(stream, "ext_ip");
        hostinfo.cpu = getOptVal(stream, "cpu");
	hostinfo.ip = clientip;
        
        // wypelniamy dane dot. statystyk interfejsu sieciowego
        net_bytes_in_s = getOptVal(stream, "eth_in");
        //netinfo.bytes_in = atoi(net_bytes_in_s);
        net_bytes_out_s = getOptVal(stream, "eth_out");
        //netinfo.bytes_out = atoi(net_bytes_out_s);

	// sprawdzam czy istnieje w bazie rekord z okreslonym systemid.
	// Aktualizuje rekord jest tak. Jesli nie, dodaje nowy
	if(hostExist(hostinfo.net_hwaddr)) {
		if(!updateItem(&hostinfo)) {
			logmessage = mkString("[WARNING] (reciver) Blad aktualizacji bazy", NULL);
			writeLog(lf, logmessage);
		}
	}
	else {
		if(!insertItem(&hostinfo)) {
			logmessage = mkString("[WARNING] (reciver) Blad aktualizacji bazy", NULL);
			writeLog(lf, logmessage);
		}
	}
        
        // uzupelniamy info na temat statystyk interfejsu sieciowego
        insertNetworkData(net_bytes_in_s, net_bytes_out_s, curr_time_s, hostinfo.net_hwaddr);
	
        free(uptime_s);
	free(hdd_total_s);
	free(hdd_free_s);
	free(ram_total_s);
	free(ram_free_s);
	free(curr_time_s);
        free(net_bytes_in_s);
        free(net_bytes_out_s);

	ClearSystemInformation(&hostinfo);

	return 1;
}
int hostExist(char * hostid) {
	extern MYSQL * dbh;
	MYSQL_RES * res;
	char * query = mkString("SELECT id FROM sysinfo WHERE system_id = '", hostid, "'", NULL);
	int status = 0;

	if(!mysql_query(dbh, query))
		status = 0;
	if((res = mysql_store_result(dbh))) {
		if(!mysql_num_rows(res))
			status = 0;
		else
			status = 1;
	}
	else
		status = 0;

	free(query);
	mysql_free_result(res);
	return status;
}
int checkDBConfigVer(char * systemid) {
    extern MYSQL * dbh;
    MYSQL_RES * res;
    MYSQL_ROW r;
    char * query = mkString("SELECT scope, version FROM configver WHERE systemid = '",
                            "(SELECT id FROM sysinfo where system_id = '", systemid,"')", NULL);
    int ver = 0;

    if(mysql_query(dbh, query)) {
        free(query);
        return 0;
    }
    if((res = mysql_store_result(dbh))) {
        r = mysql_fetch_row(res);
        ver = atoi(r[0]);
    }
    mysql_free_result(res);
    free(query);

    return ver;
}
int updateItem(systeminfo * info) {
	extern MYSQL * dbh;
	int status = 0;

	char * uptime_s = long2String(info->uptime);
	char * hdd_total_s = ulong2String(info->hdd_total);
	char * hdd_free_s = ulong2String(info->hdd_free);
	char * ram_total_s = ulong2String(info->ram_total);
	char * ram_free_s = ulong2String(info->ram_free);
	char * curr_time_s = ulong2String(info->curr_time);

	char * query = mkString(
			"UPDATE sysinfo SET uptime = ", uptime_s,
			", ip = '", info->ip,
			"', ext_ip = '", info->extip,
			"', hostname = '", info->hostname,
			"', distro = '", info->os,
			"', hdd_total = ", hdd_total_s,
			", hdd_free = ", hdd_free_s,
			", ram_total = ", ram_total_s,
			", ram_free = ", ram_free_s,
			", seen = ", curr_time_s,
                        ", cpu_usage = ", info->cpu,
			" WHERE system_id = '", info->net_hwaddr, "'", NULL);

	if(!mysql_query(dbh, query))
		status = 1;

	free(query);
	free(uptime_s);
	free(hdd_total_s);
	free(hdd_free_s);
	free(ram_total_s);
	free(ram_free_s);
	free(curr_time_s);

	return status;
}
int insertItem(systeminfo * info) {
	extern MYSQL * dbh;
	int status = 0;
	int sysid = 0;			// ID rekordu w bazie
	char * sysid_s = NULL;	// ID w formie stringu

	char * uptime_s = long2String(info->uptime);
	char * hdd_total_s = ulong2String(info->hdd_total);
	char * hdd_free_s = ulong2String(info->hdd_free);
	char * ram_total_s = ulong2String(info->ram_total);
	char * ram_free_s = ulong2String(info->ram_free);
	char * curr_time_s = ulong2String(info->curr_time);


	char * query = mkString("INSERT INTO sysinfo",
                                "(ip, ext_ip, hostname, distro, uptime, hdd_total, hdd_free, ram_total, ",
                                "ram_free, system_id, seen, host_status, cpu_usage) VALUES('",
                                info->ip, "', '", info->extip, "', '", info->hostname, "', '", info->os, 
                                "', ", uptime_s, ", ", hdd_total_s, ", ", hdd_free_s, ", ", ram_total_s, 
                                ", ", ram_free_s,", '", info->net_hwaddr, "', ", curr_time_s, ", 'A', ",
                                info->cpu,")", NULL);

	if(!mysql_query(dbh, query))
		status = 1;

	free(query);
	free(uptime_s);
	free(hdd_total_s);
	free(hdd_free_s);
	free(ram_total_s);
	free(ram_free_s);
	free(curr_time_s);

	// sprawdzamy ID z jakim dodal sie do bazy serwer
	sysid = getDBHostID(info->net_hwaddr);
	sysid_s = int2String(sysid);

	// dodajemy do bazy domyslnego usera (root).
	query = mkString("INSERT INTO sysusers(login,fullname,email,gecos,status,system_id) ",
			"VALUES('root', 'Charlie Root', 'root@angrybits.pl', 'Charlie Root', 'A', ", sysid_s, ")", NULL);
	if(!mysql_query(dbh, query))
		status = 1;

	free(query);
	free(sysid_s);

	return status;
}
htpasswdData * ReadHtpasswdData(char * hostid) {
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
vhostData * ReadVhostData(char * hostid) {
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
char * readData(char * input) {
    size_t len = strlen(input) + 1;
    char * tmp = (char *) malloc(len * sizeof(char));
    memset(tmp, '\0', len);
    strncpy(tmp, input, len);

    return tmp;
}
int getDBHostID(char * hwaddr) {
	extern MYSQL * dbh;
	MYSQL_RES * res;
	MYSQL_ROW row;
	int id = 0;
	char * query = mkString("SELECT id FROM sysinfo WHERE system_id = '", hwaddr, "'", NULL);

	if(!mysql_query(dbh, query)) {
		if((res = mysql_store_result(dbh)) != NULL) {
			row = mysql_fetch_row(res);
			id = atoi(row[0]);
			mysql_free_result(res);
		}
	}
	free(query);
	return id;
}
void updateServiceState(char * cliresp) {
	char * systemid = getOptVal(cliresp, "systemid");		// mac-adres hosta
	char * m_data = rawMonitoringData(cliresp);  		// surowe dane dot. monitoringu
	int items = checks(m_data);					 		// liczba zarejestrowanych checkow
	extern MYSQL * dbh;
	int i;

	// przygotowujemy tablice, ktora zostanie wypelniona danymi z checkow
	kv chk_states[items];

	InitCheckData(chk_states, items); 				// inicjumemy zmienne
	// wypelniamy zadeklarowana tablice danymi
	ReadRawMonitoringData(m_data, chk_states, items);

	for(i = 0; i < items; i++) {
		if(s_exist(chk_states[i].key, systemid) > 0)
			s_update(chk_states[i].key, chk_states[i].val, systemid);
		else if (s_exist(chk_states[i].key, systemid) == 0)
			s_insert(chk_states[i].key, chk_states[i].val, systemid);
	}

	// czyscimy pamiec
	free(systemid);
	free(m_data);
	ClearCheckData(chk_states, items);
}
int s_exist(char * s, char * systemid) {
	extern MYSQL * dbh;
	MYSQL_RES * res;
	char * query = mkString("SELECT id FROM service_checks WHERE service = '", s, "' AND ",
			"host_id = (SELECT id FROM sysinfo WHERE system_id = '", systemid, "')", NULL);

	if(mysql_query(dbh, query)) {
		free(query);
		return -1;
	}
	if((res = mysql_store_result(dbh)) == NULL) {
		free(query);
		return -1;
	}
	if(mysql_num_rows(res) > 0) {
		mysql_free_result(res);
		free(query);
		return 1;
	}
	else {
		mysql_free_result(res);
		free(query);
		return 0;
	}

}
int s_insert(char * srv, char * state, char * hostid) {
	extern MYSQL * dbh;
	char * insert = mkString("INSERT INTO service_checks(service, status, host_id)",
			" VALUES('",srv, "', '",state,"', (SELECT id FROM sysinfo WHERE system_id = '",hostid,"'))", NULL);

	if (mysql_query(dbh, insert)) {
		free(insert);
		return 0;
	}
	else {
		free(insert);
		return 1;
	}
}
int s_update(char * srv, char * state, char * hostid) {
	extern MYSQL * dbh;
	char * update = mkString("UPDATE service_checks SET status = '", state, "' WHERE ",
			" service = '",srv,"' AND host_id = (SELECT id FROM sysinfo WHERE system_id = '",hostid,"')", NULL);

	if (mysql_query(dbh, update)) {
		free(update);
		return 0;
	}
	else {
		free(update);
		return 1;
	}
}
void insertNetworkData(char * bytes_in, char * bytes_out, char * time_stmp, char * hostid) {
    extern MYSQL * dbh;
    char * insert = mkString("INSERT INTO netif_stats(`in`, `out`, time_stamp, system_id) ",
                             "VALUES(",bytes_in,", ",bytes_out,", ",time_stmp,", ",
                             "(SELECT id FROM sysinfo WHERE system_id = '",hostid,"'))", NULL);
    
    mysql_query(dbh, insert);
    free(insert);
}
sysuser * getSystemAccounts(hostconfig * hc, char * systemid) {
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
                curr->sshkey = readSSHkeys(row[8]);
                
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
sshkeys * readSSHkeys(char * str) {
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
int applyStatusChange(resp * data) {
    resp * curr = data;
    
    while(curr) {
        if(curr->status == 'A')
            activate(curr->scope, curr->dbid);
        if(curr->status == 'D')
            delete(curr->scope, curr->dbid);
        
        curr = curr->next;
    }
    return 1;
}
void activate(char * scope, int id) {
    extern MYSQL * dbh;
    
    char * query = NULL;
    char * sid = int2String(id);
    
    if(!strcmp(scope, "apache"))
       query = mkString("UPDATE www SET status = 'A' WHERE id = ", sid, NULL); 
    if(!strcmp(scope, "htusers"))
       query = mkString("UPDATE www_users SET status = 'A' WHERE id = ", sid, NULL);
    if(!strcmp(scope, "sysusers"))
       query = mkString("UPDATE sysusers SET status = 'A' WHERE id = ", sid, NULL);
    if(!strcmp(scope, "db_name"))
       query = mkString("UPDATE db_name SET status = 'A' WHERE id = ", sid, NULL);
    if(!strcmp(scope, "db_user"))
       query = mkString("UPDATE db_user SET status = 'A' WHERE id = ", sid, NULL);
    if(!strcmp(scope, "db_privs"))
       query = mkString("UPDATE db_privs SET status = 'A' WHERE id = ", sid, NULL);
    
    mysql_query(dbh, query);
    free(query);
    free(sid);
}
void delete(char * scope, int id) {
    extern MYSQL * dbh;
    
    char * query = NULL;
    char * query_privs = NULL;
    char * sid = int2String(id);
    
    if(!strcmp(scope, "apache"))
       query = mkString("DELETE FROM www WHERE id = ", sid, NULL); 
    if(!strcmp(scope, "htusers"))
       query = mkString("DELETE FROM www_users WHERE id = ", sid, NULL);
    if(!strcmp(scope, "sysusers"))
       query = mkString("DELETE FROM sysusers WHERE id = ", sid, NULL);
    if(!strcmp(scope, "db_name"))
       query = mkString("DELETE FROM db_name WHERE id = ", sid, NULL);
    if(!strcmp(scope, "db_user")) {
        query_privs = mkString("DELETE FROM db_privs WHERE user_id = ", sid, NULL);
        query = mkString("DELETE FROM db_user WHERE id = ", sid, NULL);
        mysql_query(dbh, query_privs);
        free(query_privs);
    }
    if(!strcmp(scope, "db_privs"))
        query = mkString("DELETE FROM db_privs WHERE id = ", sid, NULL);
    
    mysql_query(dbh, query);
    free(query);
    free(sid);
}
dbinfo * getDatabaseNames(char * systemid) {
    extern MYSQL * dbh;
    MYSQL_RES * res;
    MYSQL_ROW row;
    char * query = mkString("SELECT dn.id, dn.name AS dbname, dn.status, cv.version ",
                            "FROM db_name dn JOIN sysinfo s ON dn.host_id = s.id JOIN ",
                            "configver cv ON dn.host_id = cv.systemid AND cv.scope = ",
                            "'db_name' WHERE system_id = '",systemid,"'", NULL);
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
                            "JOIN sysinfo s ON du.host_id = s.id WHERE s.system_id = ",
                            "'",systemid,"'", NULL);
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
                            "WHERE s.system_id = '",systemid,"'", NULL);
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
int DBnamesDataSize(dbinfo * db) {
    dbinfo * curr = db;                 // current node
    int size = 0;                       // overrall data size
    int keySize = 0;                    // key names size
    int nodeCount = 0;                  // processed nodes count
    int dqCount   = 2;                  // double quotes count in each position
    char * tmp = NULL;                  // temporary string

    // package header
    char * header = "{scope:db_name,},";    
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
    
    return size;
}
int DBusersDataSize(dbuser * db) {
    dbuser * curr = db;                 // current node
    int size = 0;                       // overrall data size
    int keySize = 0;                    // key names size
    int nodeCount = 0;                  // processed nodes count
    int dqCount   = 2;                  // double quotes count in each position
    char * tmp = NULL;                  // temporary string

    // package header
    char * header = "{scope:db_user,},";    
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
    
    return size;
}
int DBgrantsDataSize(grants * db) {
    grants * curr = db;                 // current node
    int size = 0;                       // overrall data size
    int keySize = 0;                    // key names size
    int nodeCount = 0;                  // processed nodes count
    int dqCount   = 2;                  // double quotes count in each position
    char * tmp = NULL;                  // temporary string

    // package header
    char * header = "{scope:db_privs,},";    
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
    
    return size;
}
char * DBNamesConfigPackage(dbinfo * db) {
    // common data
    dbinfo * curr          = db;        // node traversing pointer
    int size               = 0;         // package size
    int idx                = 0;		// node index
    char * package         = NULL;      // output package
    char * numstr          = NULL;	// node index as a string
    char * entry           = NULL;      // particular entry definition
    char * s_dbid          = NULL;      // DB ID in a form of string
    
    // specific data
    char status[2];                     // status flags can be: NUDA
    
    // package header
    char * header = "{scope:db_name,";

    // config version
    char * k_config_ver = "config_ver:";
    char * s_config_ver = NULL;

    if(db == NULL)
        return NULL;
    
    // preparing memory
    size = DBnamesDataSize(db) + 1;
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
    strncat(package, k_config_ver, strlen(k_config_ver));
    strncat(package, s_config_ver, strlen(s_config_ver));
    strncat(package, "},", 2);

    // czyscimy niepotrzebne dane
    free(s_config_ver);
    cleanDBinfoData(db);

    return package;
}
char * DBusersConfigPackage(dbuser * db) {
    // common data
    dbuser * curr          = db;        // node traversing pointer
    int size               = 0;         // package size
    int idx                = 0;		// node index
    char * package         = NULL;      // output package
    char * numstr          = NULL;	// node index as a string
    char * entry           = NULL;      // particular entry definition
    char * s_dbid          = NULL;      // DB ID in a form of string
    
    // specific data
    char status[2];                     // status flags can be: NUDA
    
    // package header
    char * header = "{scope:db_user,";

    // config version
    char * k_config_ver = "config_ver:";
    char * s_config_ver = NULL;

    if(db == NULL)
        return NULL;
    
    // preparing memory
    size = DBusersDataSize(db) + 1;
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
    strncat(package, k_config_ver, strlen(k_config_ver));
    strncat(package, s_config_ver, strlen(s_config_ver));
    strncat(package, "},", 2);

    // czyscimy niepotrzebne dane
    free(s_config_ver);
    cleanDBusersData(db);

    return package;
}
char * DBgrantsConfigPackage(grants * db) {
    // common data
    grants * curr          = db;        // node traversing pointer
    int size               = 0;         // package size
    int idx                = 0;		// node index
    char * package         = NULL;      // output package
    char * numstr          = NULL;	// node index as a string
    char * entry           = NULL;      // particular entry definition
    char * s_dbid          = NULL;      // DB ID in a form of string
    
    // specific data
    char status[2];                     // status flags can be: NUDA
    
    // package header
    char * header = "{scope:db_privs,";

    // config version
    char * k_config_ver = "config_ver:";
    char * s_config_ver = NULL;

    if(db == NULL)
        return NULL;
    
    // preparing memory
    size = DBgrantsDataSize(db) + 1;
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
    strncat(package, k_config_ver, strlen(k_config_ver));
    strncat(package, s_config_ver, strlen(s_config_ver));
    strncat(package, "},", 2);

    // czyscimy niepotrzebne dane
    free(s_config_ver);
    cleanDBgrantsData(db);

    return package;
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
resp * DatabaseSetup(dbinfo * db, char * os, FILE * lf, resp * respdata) {
    dbinfo * curr = db;
    char * msg = NULL;
    
    // response to server
    resp * rhead = respdata;
    resp * rcurr = NULL;
    resp * rprev = NULL;
    
    // moving to the end of the list
    while(rhead != NULL)
        rhead = rhead->next;
    
    while(curr) {
        if(curr->status == 'N') {
            if(dbmgr(curr->dbname, curr->status, os))
                msg = mkString("[INFO] (reciver) Database: ", curr->dbname, " created", NULL);
            else
                msg = mkString("[ERROR] (reciver) Creation database: ", curr->dbname, " failed", NULL);
            writeLog(lf, msg);
            
            rcurr = respStatus("db_name", 'A', curr->dbid);
            if(rhead == NULL)
                rhead = rcurr;
            else
                rprev->next = rcurr;
            rprev = rcurr;
        }
        if(curr->status == 'D') {
            if(dbmgr(curr->dbname, curr->status, os))
                msg = mkString("[INFO] (reciver) Database: ", curr->dbname, " removed", NULL);
            else
                msg = mkString("[ERROR] (reciver) Removing database: ", curr->dbname, " failed", NULL);
            writeLog(lf, msg);
            
            rcurr = respStatus("db_name", 'D', curr->dbid);
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
resp * DatabaseUsersSetup(dbuser * db, char * os, FILE * lf, resp * respdata) {
    dbuser * curr = db;
    char * msg = NULL;
    
    // response to server
    resp * rhead = respdata;
    resp * rcurr = NULL;
    resp * rprev = NULL;
    
    // moving to the end of the list
    while(rhead != NULL)
        rhead = rhead->next;
    
    while(curr) {
        if(curr->status == 'N') {
            if(dbusermgr(curr, curr->status, os))
                msg = mkString("[INFO] (reciver) Database user: ", curr->login, " created", NULL);
            else
                msg = mkString("[ERROR] (reciver) Creation database user: ", curr->login, " failed", NULL);
            writeLog(lf, msg);
            
            rcurr = respStatus("db_user", 'A', curr->dbid);
            if(rhead == NULL)
                rhead = rcurr;
            else
                rprev->next = rcurr;
            rprev = rcurr;
        }
        if(curr->status == 'D') {
            if(dbusermgr(curr, curr->status, os))
                msg = mkString("[INFO] (reciver) Database user: ", curr->login, " removed", NULL);
            else
                msg = mkString("[ERROR] (reciver) Removing database user: ", curr->login, " failed", NULL);
            writeLog(lf, msg);
            
            rcurr = respStatus("db_user", 'D', curr->dbid);
            if(rhead == NULL)
                rhead = rcurr;
            else
                rprev->next = rcurr;
            rprev = rcurr;
        }
        if(curr->status == 'U') {
            if(dbusermgr(curr, curr->status, os))
                msg = mkString("[INFO] (reciver) Database user: ", curr->login, " updated", NULL);
            else
                msg = mkString("[ERROR] (reciver) Updating database user: ", curr->login, " failed", NULL);
            writeLog(lf, msg);
            
            rcurr = respStatus("db_user", 'A', curr->dbid);
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
resp * DatabaseUserGrantsSetup(grants * db, char * os, FILE * lf, resp * respdata) {
    grants * curr = db;
    char * msg = NULL;
    
    // response to server
    resp * rhead = respdata;
    resp * rcurr = NULL;
    resp * rprev = NULL;
    
    // moving to the end of the list
    while(rhead != NULL)
        rhead = rhead->next;
    
    while(curr) {
        if(curr->status == 'N') {
            if(dbgrantsmgr(curr, curr->status, os))
                msg = mkString("[INFO] (reciver) Adding permissions to ", curr->dbname, "for ", curr->user, NULL);
            else
                msg = mkString("[ERRPR] (reciver) Adding permissions to ", curr->dbname, "for ", curr->user, " failed", NULL);
            writeLog(lf, msg);
            
            rcurr = respStatus("db_privs", 'A', curr->dbid);
            if(rhead == NULL)
                rhead = rcurr;
            else
                rprev->next = rcurr;
            rprev = rcurr;
        }
        if(curr->status == 'D') {
            if(dbgrantsmgr(curr, curr->status, os))
                msg = mkString("[INFO] (reciver) Revoke permissions to ", curr->dbname, "for ", curr->user, NULL);
            else
                msg = mkString("[ERROR] (reciver) Revoke permissions to ", curr->dbname, "for ", curr->user, " failed", NULL);
            writeLog(lf, msg);
            
            rcurr = respStatus("db_user", 'D', curr->dbid);
            if(rhead == NULL)
                rhead = rcurr;
            else
                rprev->next = rcurr;
            rprev = rcurr;
        }
        if(curr->status == 'U') {
            if(dbgrantsmgr(curr, curr->status, os))
                msg = mkString("[INFO] (reciver) Updating permissions to ", curr->dbname, "for ", curr->user, NULL);
            else
                msg = mkString("[INFO] (reciver) Updating permissions to ", curr->dbname, "for ", curr->user, " failed", NULL);
            writeLog(lf, msg);
            
            rcurr = respStatus("db_privs", 'A', curr->dbid);
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
int dbmgr(char * dbname, char action, char * os) {
    MYSQL * mysqlh = mysqlconn(os);
    char * query = NULL;
    int status = 0;
    
    if(action == 'N')
        query = mkString("CREATE DATABASE ", dbname, NULL);
    else if(action == 'D')
        query = mkString("DROP DATABASE ", dbname, NULL);
    
    if(!mysqlh)
        return 0;
    if(!mysql_query(mysqlh, query))
        status = 1;
    
    mysql_close(mysqlh);
    free(query);
    
    return status;
}
int dbusermgr(dbuser * db, char action, char * os) {
    MYSQL * mysqlh = mysqlconn(os);
    char * query = NULL;
    int status = 0;
    
    if(action == 'N')
        query = mkString("CREATE USER '", db->login, "'@'localhost' IDENTIFIED ",
                         "BY PASSWORD '",db->pass, "'" , NULL);
    else if(action == 'D')
        query = mkString("DROP USER '", db->login, "'@'localhost'", NULL);
    else if(action == 'U')
        query = mkString("UPDATE mysql.user SET authentication_string = '",db->pass,
                         "' WHERE User = '",db->login,"' AND host = 'localhost'",  NULL);
    
    if(!mysqlh)
        return 0;
    if(!mysql_query(mysqlh, query)) {
        mysql_query(mysqlh, "flush privileges");
        status = 1;
    }
    
    mysql_close(mysqlh);
    free(query);
    
    return status;
}
int dbgrantsmgr(grants * db, char action, char * os) {
    MYSQL * mysqlh = mysqlconn(os);
    
    char * grant = mkString("GRANT ", db->privs, " ON ", db->dbname, ".* TO '",
                        db->user, "'@'localhost'", NULL);
    char * revoke = mkString("REVOKE ALL ON ", db->dbname, " FROM '", db->user, 
                        "'@'localhost'", NULL);
    char * query = NULL;
    int status = 0;
    
    if(action == 'N')
        query = grant;
    else if(action == 'D') {
        if(mysqlUserExist(mysqlh, db->user))
            query = revoke;
    }
    else if(action == 'U') {
        mysql_query(mysqlh, revoke);
        query = grant;
    }
    
    if(!mysqlh)
        return 0;
    if(!mysql_query(mysqlh, query)) {
        mysql_query(mysqlh, "flush privileges");
        status = 1;
    }
    
    mysql_close(mysqlh);
    free(grant);
    free(revoke);
    
    return status;
}
int mysqlUserExist(MYSQL * dbh, char * login) {
    char * query = mkString("SELECT User FROM mysql.user WHERE User = '", login, "'", NULL);
    int status = 0;
    
    if(!mysql_query(dbh, query)) {
        if(mysql_field_count(dbh))
            status = 1;
        else
            status = 0;
    }
    else
        status = 0;
    
    free(query);
    
    return status;
}