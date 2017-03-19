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

	uptime_s = jsonVal(stream, "uptime");
	hostinfo.uptime = atol(uptime_s);
	hostinfo.net_hwaddr = jsonVal(stream, "systemid");
	hostinfo.hostname = jsonVal(stream, "hostname");
	hostinfo.os = jsonVal(stream, "distro_name");
	hdd_total_s = jsonVal(stream, "hdd_total");
	hostinfo.hdd_total = atol(hdd_total_s);
	hdd_free_s = jsonVal(stream, "hdd_free");
	hostinfo.hdd_free = atol(hdd_free_s);
	ram_total_s = jsonVal(stream, "ram_total");
	hostinfo.ram_total = atol(ram_total_s);
	ram_free_s = jsonVal(stream, "ram_free");
	hostinfo.ram_free = atol(ram_free_s);
	curr_time_s = jsonVal(stream, "curr_time");
	hostinfo.curr_time = atol(curr_time_s);
	hostinfo.extip = jsonVal(stream, "ext_ip");
        hostinfo.cpu = jsonVal(stream, "cpu");
	hostinfo.ip = clientip;
        
        // wypelniamy dane dot. statystyk interfejsu sieciowego
        net_bytes_in_s = jsonVal(stream, "eth_in");
        //netinfo.bytes_in = atoi(net_bytes_in_s);
        net_bytes_out_s = jsonVal(stream, "eth_out");
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
                            "cfg.systemid AND cfg.scope = 'htusers' WHERE ht.system_id = ",
                            "(SELECT id FROM sysinfo WHERE system_id = '",hostid,"')", NULL);
    
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
                            "GROUP_CONCAT(DISTINCT CONCAT(www_access.fromhost, ':', www_access.access_permission) SEPARATOR ',') AS accesslist, ",
                            "www.access_order, www.htpasswd AS password_access, case www.htpasswd WHEN 1 THEN GROUP_CONCAT(DISTINCT www_users.login ",
                            "SEPARATOR ' ') ELSE 'NaN' END AS htusers, www.status, www.purgedir, www.id FROM www JOIN sysusers ON sysusers.id = www.user_id JOIN ",
                            "sysinfo ON sysinfo.id = www.system_id JOIN www_opts_selected ON www_opts_selected.vhost_id = www.id JOIN ",
                            "www_opts ON www_opts.id = www_opts_selected.opt_id LEFT JOIN www_access ON www_access.vhost_id = www.id LEFT JOIN ",
                            "www_users_access ON (www_users_access.vhost_id = www.id AND www.htpasswd > 0) LEFT JOIN www_users ON ",
                            "(www_users.id = www_users_access.user_id AND www.htpasswd > 0) LEFT JOIN configver ON (configver.systemid = sysinfo.id AND ",
                            "configver.scope = 'apache') WHERE www.system_id = (SELECT id FROM sysinfo WHERE system_id = '", hostid ,"') GROUP BY www.id", NULL);
    
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
	char * systemid = jsonVal(cliresp, "systemid");		// mac-adres hosta
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
    extern MYSQL * dbh;
    char * query = NULL;
    resp * curr = data;
    char stat[2];
    char * tmp = NULL;
    
    while(curr) {
        stat[0] = curr->status;
        stat[1] = '\0';
        tmp = int2String(curr->dbid);
        
        if(!strcmp(curr->scope, "sysusers")) {
            if(curr->status == 'D')
                query = mkString("DELETE FROM sysusers WHERE id = ", tmp, NULL);
            else
                query = mkString("UPDATE sysusers SET status = '", stat, "' WHERE id = ", tmp, NULL);
        }
        if(!strcmp(curr->scope, "apache")) {
            if(curr->status == 'D')
                query = mkString("DELETE FROM www WHERE id = ", tmp, NULL);
            else
                query = mkString("UPDATE www SET status = '", stat, "' WHERE id = ", tmp, NULL);
        }
        mysql_query(dbh, query);
        
        free(tmp);
        free(query);
        curr = curr->next;
    }
    return 1;
}