/* database.c -- zestaw funkcji, ktore operuja na bazie danych */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include "core.h"
#include "sysconfigdata.h"
#include "database.h"
#include "monitoring.h"
#include "scopes/sysusers.h"
#include "commondata.h"
#include "toolkit.h"

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