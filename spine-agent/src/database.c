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
int updateHostInfo(char * clientip, char * stream, FILE * lf) {
	systeminfo hostinfo;
	char * logmessage = NULL;
	char * uptime_s = NULL;
	char * hdd_total_s = NULL;
	char * hdd_free_s = NULL;
	char * ram_total_s = NULL;
	char * ram_free_s = NULL;

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
	hostinfo.extip = jsonVal(stream, "ext_ip");
	hostinfo.ip = clientip;

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
	free(uptime_s);
	free(hdd_total_s);
	free(hdd_free_s);
	free(ram_total_s);
	free(ram_free_s);

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
	char * query = mkString("SELECT config_ver FROM sysinfo WHERE system_id = '", systemid, "'", NULL);
	int ver = 0;

	if(mysql_query(dbh, query)) {
		free(query);
		return -1;
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
			" WHERE system_id = '", info->net_hwaddr, "'", NULL);

	if(!mysql_query(dbh, query))
		status = 1;

	free(query);
	free(uptime_s);
	free(hdd_total_s);
	free(hdd_free_s);
	free(ram_total_s);
	free(ram_free_s);

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


	char * query = mkString("INSERT INTO sysinfo(ip, ext_ip, hostname, distro, uptime, hdd_total, hdd_free, ram_total, ram_free, system_id, config_ver) VALUES('",
			info->ip, "', '", info->extip, "', '", info->hostname, "', '", info->os, "', ", uptime_s, ", ", hdd_total_s, ", ", hdd_free_s, ", ", ram_total_s, ", ", ram_free_s,
			", '", info->net_hwaddr, "', 0)", NULL);

	if(!mysql_query(dbh, query))
		status = 1;

	free(query);
	free(uptime_s);
	free(hdd_total_s);
	free(hdd_free_s);
	free(ram_total_s);
	free(ram_free_s);

	// sprawdzamy ID z jakim dodal sie do bazy serwer
	sysid = getDBHostID(info->net_hwaddr);
	sysid_s = int2String(sysid);

	// dodajemy do bazy domyslnego usera (root).
	query = mkString("INSERT INTO sysusers(login,fullname,email,system_id) ",
			"VALUES('root', 'Charlie Root', 'root@angrybits.pl', ", sysid_s, ")", NULL);
	if(!mysql_query(dbh, query))
		status = 1;

	free(query);
	free(sysid_s);

	return status;
}
hostconfig ReadWWWConfiguration(char * hostid) {
	extern MYSQL * dbh;
	MYSQL_RES * res;
	MYSQL_ROW row;
	hostconfig hconfig;
	long resnum = 0L;

	size_t len = 0;
	htpasswdData * head = NULL;
	htpasswdData * curr = NULL;
	htpasswdData * prev = NULL;

	char * query = mkString("SELECT www.ServerName, www.ServerAlias, www.DocumentRoot, www.htaccess, sysusers.login AS user, ",
							"sysinfo.config_ver AS config_ver, GROUP_CONCAT(DISTINCT www_opts.vhostopt SEPARATOR ' ') AS opts, ",
							"GROUP_CONCAT(DISTINCT CONCAT(www_access.fromhost, ':', www_access.access_permission) SEPARATOR ',') AS accesslist, ",
							"www.access_order, www.htpasswd AS password_access, case www.htpasswd WHEN 1 THEN GROUP_CONCAT(DISTINCT www_users.login ",
							"SEPARATOR ' ') ELSE 'NaN' END AS htusers, www.status, www.purgedir FROM www JOIN sysusers ON sysusers.id = www.user_id JOIN ",
							"sysinfo ON sysinfo.id = www.system_id JOIN www_opts_selected ON www_opts_selected.vhost_id = www.id JOIN ",
							"www_opts ON www_opts.id = www_opts_selected.opt_id LEFT JOIN www_access ON www_access.vhost_id = www.id LEFT JOIN ",
							"www_users_access ON (www_users_access.vhost_id = www.id AND www.htpasswd > 0) LEFT JOIN www_users ON ",
							"(www_users.id = www_users_access.user_id AND www.htpasswd > 0) WHERE www.system_id = (SELECT id FROM ",
							"sysinfo WHERE system_id = '", hostid ,"') GROUP BY www.id", NULL);

	int vhi = 0;			// index tablicy przechowujacej vhosty apacza
	if(!mysql_query(dbh, query)) {
		if((res = mysql_store_result(dbh)) != NULL) {
			resnum = mysql_num_rows(res);
			initConfigData(&hconfig, resnum);
			hconfig.vhost_num = (int) resnum;
			while((row = mysql_fetch_row(res)) && vhi < VHOST_MAX) {
				hconfig.vhost[vhi].ServerName = readData(row[0]);
				hconfig.vhost[vhi].ServerAlias = readData(row[1]);
				hconfig.vhost[vhi].DocumentRoot = readData(row[2]);
				hconfig.vhost[vhi].htaccess = readData(row[3]);
				hconfig.vhost[vhi].user = readData(row[4]);
				hconfig.confVer = atoi(row[5]);
				hconfig.vhost[vhi].apacheOpts = readData(row[6]);
				hconfig.vhost[vhi].vhost_access_list = readData(row[7]);
				hconfig.vhost[vhi].vhost_access_order = readData(row[8]);
				hconfig.vhost[vhi].password_access = atoi(row[9]);
				hconfig.vhost[vhi].htusers = readData(row[10]);
				hconfig.vhost[vhi].status = readData(row[11]);
				hconfig.vhost[vhi].purgedir = readData(row[12]);
				vhi++;
			}
			mysql_free_result(res);
		}
	}
	free(query);

	query = mkString("SELECT CONCAT(login, ':', password) AS htpasswd FROM www_users WHERE system_id = (SELECT id FROM sysinfo WHERE system_id = '",
					hostid, "')", NULL);
	if(!mysql_query(dbh, query)) {
		if((res = mysql_store_result(dbh)) != NULL) {
			if((resnum = mysql_num_rows(res)) > 0) {
				while((row = mysql_fetch_row(res))) {
					len = strlen(row[0]) + 1;
					curr = (htpasswdData *) malloc(sizeof(htpasswdData));
					curr->entry = (char *) malloc(len * sizeof(char));
					memset(curr->entry, '\0', len);
					strcpy(curr->entry, row[0]);
					curr->next = NULL;

					if(head == NULL)
						head = curr;
					else
						prev->next = curr;
					prev = curr;
				}
			}
			else {
				len = strlen("NaN") + 1;
				curr = (htpasswdData *) malloc(sizeof(htpasswdData));
				curr->entry = (char *) malloc(len * sizeof(char));
				memset(curr->entry, '\0', len);
				strcpy(curr->entry, "NaN");
				curr->next = NULL;
				head = curr;
			}
			hconfig.htusers_count = (int) resnum;
		}
		hconfig.htpasswd = head;
		mysql_free_result(res);
	}
	free(query);
	return hconfig;
}
char * readData(char * input) {
	size_t len = strlen(input) + 1;
	char * tmp = (char *) malloc(len * sizeof(char));
	memset(tmp, '\0', len);
	strcpy(tmp, input);

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
void cleanWWWConfiguration(char * hostid) {
	extern MYSQL * dbh;
	MYSQL_RES * res;
	MYSQL_ROW row;
	long rows = 0L;

	char * query = mkString("SELECT id FROM www WHERE `status` = 'D' AND system_id = (SELECT id FROM sysinfo WHERE system_id = '", hostid, "')", NULL);
	char * subquery = NULL;
	if(!mysql_query(dbh, query)) {
		if((res = mysql_store_result(dbh)) != NULL) {
			if((rows = mysql_num_rows(res)) > 0) {
				while((row = mysql_fetch_row(res))) {
					subquery = mkString("DELETE FROM www_opts_selected WHERE vhost_id = ", row[0], NULL);
					mysql_query(dbh, subquery);
					free(subquery);

					subquery = mkString("DELETE FROM www_access WHERE vhost_id = ", row[0], NULL);
					mysql_query(dbh, subquery);
					free(subquery);

					subquery = mkString("DELETE FROM www_users_access WHERE vhost_id = ", row[0], NULL);
					mysql_query(dbh, subquery);
					free(subquery);

					subquery = mkString("DELETE FROM www WHERE id = ", row[0], NULL);
					mysql_query(dbh, subquery);
					free(subquery);
				}
				mysql_free_result(res);
			}
		}
	}
	free(query);
}
