/* database.c -- zestaw funkcji, ktore operuja na bazie danych */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include "core.h"
#include "sysconfigdata.h"
#include "database.h"

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
int updateHostInfo(char * stream, FILE * lf) {
	systeminfo hostinfo;
	char * logmessage = NULL;

	// odczytujemy informacje z jsona
	InitSystemInformation(&hostinfo);
	hostinfo.uptime = atol(jsonVal(stream, "uptime"));
	hostinfo.net_hwaddr = jsonVal(stream, "systemid");

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

	mysql_free_result(res);
	return status;
}
int updateItem(systeminfo * info) {
	extern MYSQL * dbh;
	int status = 0;

	// konwertuje uptime na stringa
	char uptime_s[12];
	memset(uptime_s, '\0', 12);
	sprintf(uptime_s, "%ld", info->uptime);

	char * query = mkString("UPDATE sysinfo SET uptime = ", uptime_s, " WHERE system_id = '", info->net_hwaddr, "'", NULL);

	if(!mysql_query(dbh, query))
		status = 1;
	free(query);

	return status;
}
int insertItem(systeminfo * info) {
	extern MYSQL * dbh;
	int status = 0;

	// konwertuje uptime na stringa
	char uptime_s[12];
	memset(uptime_s, '\0', 12);
	sprintf(uptime_s, "%ld", info->uptime);

	char * query = mkString("INSERT INTO sysinfo(uptime, system_id) VALUES(", uptime_s, ", '", info->net_hwaddr, "')", NULL);

	if(!mysql_query(dbh, query))
		status = 1;
	free(query);

	return status;
}
