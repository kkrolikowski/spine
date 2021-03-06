/* core.c -- podstawowe funkcje programu */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "core.h"
#include "network.h"
#include "sysconfigdata.h"
#include "database.h"
#include "scopes/apache.h"
#include "scopes/db.h"
#include "monitoring.h"
#include "scopes/sysusers.h"
#include "commondata.h"
#include "toolkit.h"
#include "configdata/buildpkg.h"
#include "configdata/dbdata.h"

int savePidFile(int pid) {
	FILE * pf;

	if((pf = fopen(PID_PATH, "w")) == NULL)
		return 0;
	fprintf(pf, "%d", pid);
	fclose(pf);

	return 1;
}
void writeLog(FILE *lf, char * message) {
	char * ts = timestamp();

	fprintf(lf, "%s %s\n", ts, message);
	free(ts);
	free(message);
	fflush(lf);
}
int norootUser(void) {
	if(geteuid() > 0)
		return 1;
	else
		return 0;
}
void InitConfigData(config_data * cfd) {
	cfd->mode = NULL;
	cfd->logpath = NULL;
	cfd->host = NULL;
	cfd->port = 0;
	InitDBConnData(cfd->dbinfo);
}
int ReadConfig(config_data * cfd, FILE * cf) {
	char buff[BUFSIZE];
	int status = 0;

	memset(buff, '\0', BUFSIZE);
	while(fgets(buff, BUFSIZE, cf) != NULL) {
		if(buff[0] == '#') {
			memset(buff, '\0', BUFSIZE);
			continue;
		}
		if(strstr(buff, "mode") != NULL) {
			cfd->mode = parseLine(buff);
			status = 1;
		}
		else if(strstr(buff, "log") != NULL) {
			cfd->logpath = parseLine(buff);
			status = 1;
		}
		else if(strstr(buff, "agent_host") != NULL) {
			cfd->host = parseLine(buff);
			status = 1;
		}
		else if(strstr(buff, "agent_port") != NULL) {
			cfd->port = atoi(parseLine(buff));
			status = 1;
		}
		else if(strstr(buff, "dbhost") != NULL) {
			cfd->dbinfo.dbhost = parseLine(buff);
			status = 1;
		}
		else if(strstr(buff, "dbname") != NULL) {
			cfd->dbinfo.dbname = parseLine(buff);
			status = 1;
		}
		else if(strstr(buff, "dbuser") != NULL) {
			cfd->dbinfo.dblogin = parseLine(buff);
			status = 1;
		}
		else if(strstr(buff, "dbpass") != NULL) {
			cfd->dbinfo.dbpass = parseLine(buff);
			status = 1;
		}
		memset(buff, '\0', BUFSIZE);
	}
	return status;
}
char * parseLine(char * line) {
	char * val;
	char * pval;
	char * valStart;
	size_t val_len;

	pval = strchr(line, '=');
	val_len = strlen(pval);

	val = (char *) malloc(val_len * sizeof(char));
	valStart = val;
	memset(val, '\0', val_len);

	while(*pval++) {
		if(*pval == '\n')
			*pval = '\0';
		else
			*val = *pval;
		val++;
	}
	return valStart;
}
void RetrieveData(int port, char * mode, FILE *lf) {
	char * logentry = NULL;
	int netiffd = listener(port);
	int clifd = -1;
	char * clientResponse = NULL;       // string przesylany przez klienta (json)
	char * datatype = NULL;             // typ danych przesylanych do klienta
	char * system_id = NULL;            // indentyfikator systemu (mac-adress)
	char * os = NULL;                   // nazwa dystrubucji Linuksa
	char * configstring = NULL;
	netinfo net;                        // struktura przechowujaca ip oraz socket klienta
	hostconfig config;                  // konfiguracja calego hosta
        ver * cfgver;                       // wersje poszczegolnych obszarow konfiguracji
        char * clientver_str = NULL;        // wersja konfiguracji klienta (string)
        int clientver = 0;                  // wersja konfiguracji klienta
        int packagever = 0;                 // wersja konfiguracji pochodzaca z pakietu danych
        resp * updateMSGdata = NULL;        // update message to server agent
        char * updateMSGdataString = NULL;  // updata message: string version
        int bytesSent = 0;                  // volume of data - integer form
        char * s_bytesSent = NULL;            // volume of data - string form
        
	while(1) {
		net = clientConnection(netiffd);
		if(GreetClient(net.sock) < 1) {
			logentry = mkString("[WARN] (reciver) Blad wyslania wiadomosci powitalnej", NULL);
			writeLog(lf, logentry);
			free(net.ipaddr);
			sleep(5);
			continue;
		}
		if((clientResponse = readClientData(net.sock)) == NULL) {
			logentry = mkString("[WARN] (reciver) Brak danych od klienta", NULL);
			writeLog(lf, logentry);
			free(net.ipaddr);
			close(net.sock);
			sleep(5);
			continue;
		}
		// Sprawdzamy czy klient wysyla poprawne dane
		if((datatype = getOptVal(clientResponse, "datatype")) == NULL) {
			logentry = mkString("[WARN] (reciver) Nieobslugiwany format danych", NULL);
			writeLog(lf, logentry);
			free(clientResponse);
			free(net.ipaddr);
			close(net.sock);
			sleep(5);
			continue;
		}
		if(!strcmp(mode, "client")) {
                    os = linuxDistro();
                    ParseConfigData(clientResponse, &config);
                    packagever = readPackageVersion(clientResponse);
                    if(readLocalConfigVersion() < packagever) {
                        if(!strcmp(config.datatype, "hostconfig")) {
                            if(config.httpd.vhost != NULL) {
                                updateMSGdata = updateApacheSetup(config.httpd, os, lf);
                                cleanVhostData(config.httpd.vhost);
                            }
                            if(config.httpd.htpasswd != NULL) {
                                updateMSGdata = HtpasswdSetup(config.httpd.htpasswd, os, lf, updateMSGdata);
                                clearHtpasswdData(config.httpd.htpasswd);
                            }
                            clearAuthData(os);
                            if(config.sysUsers != NULL) {
                                updateMSGdata = updateUserAccounts(config.sysUsers, os, lf, updateMSGdata);
                                cleanSysUsersData(config.sysUsers);
                            }
                            if(config.sqldb.db != NULL) {
                                updateMSGdata = DatabaseSetup(config.sqldb.db, os, lf, updateMSGdata);
                                cleanDBinfoData(config.sqldb.db);
                            }
                            if(config.sqldb.dbusers != NULL) {
                                updateMSGdata = DatabaseUsersSetup(config.sqldb.dbusers, os, lf, updateMSGdata);
                                cleanDBusersData(config.sqldb.dbusers);
                            }
                            if(config.sqldb.dbgrants != NULL) {
                                updateMSGdata = DatabaseUserGrantsSetup(config.sqldb.dbgrants, os, lf, updateMSGdata);
                                cleanDBgrantsData(config.sqldb.dbgrants);
                            }
                            if(updateMSGdata != NULL) {
                                updateMSGdataString = backMessage(updateMSGdata);
                                clifd = connector(net.ipaddr, 2016);
                                SendPackage(clifd, updateMSGdataString);
                                close(clifd);
                                free(updateMSGdataString);
                                cleanMSGdata(updateMSGdata);
                                updateMSGdata = NULL;
                            }
                            if(writeLocalConfigVersion(packagever)) {
                                logentry = mkString("[INFO] (reciver) Konfiguracja zostala zaktualizowana", NULL);
                                writeLog(lf, logentry);
                            }
                            else {
                                logentry = mkString("[WARNING] (reciver) blad aktualizacji wersji konfigruacji", NULL);
                                writeLog(lf, logentry);
                            }                           
                        }
                    }
                    free(os);
		}
                // Status changes
                if(!strcmp(datatype, "StatusChange")) {
                    if((updateMSGdata = parseClientMessage(clientResponse)) != NULL) {
                        if(applyStatusChange(updateMSGdata)) {
                            logentry = mkString("[INFO] (reciver) status in database has been changed", NULL);
                            writeLog(lf, logentry);
                        }
                        cleanMSGdata(updateMSGdata);
                    }
                }
		// jesli dane sa typu sysinfo, to znaczy, ze trzeba je zapisac w bazie danych
		if(!strcmp(datatype, "sysinfo")) {
                    system_id = getOptVal(clientResponse, "systemid");
                    clientver_str = getOptVal(clientResponse, "config_ver");
                    clientver = atoi(clientver_str);
                    
                    updateHostInfo(net.ipaddr, clientResponse, lf);
                    updateServiceState(clientResponse);
                    if((cfgver = checkVersions(system_id)) == NULL) {
                        // zwalniamy pamiec
                        free(system_id);
                        free(clientver_str);
                        free(datatype);
                        close(net.sock);
                        free(net.ipaddr);
                        free(clientResponse);
                        continue;
                    }
                    
                    if(ReadHostConfig(system_id, &config, cfgver, clientver, lf)) {
                        configstring = buildConfigPackage(&config);
                        if((clifd = connector(net.ipaddr, 2016)) < 0) {
                            logentry = mkString("[ERROR] could not connect to ", net.ipaddr, NULL);
                            writeLog(lf, logentry);
                            free(net.ipaddr);
                            free(configstring);
                            free(system_id);
                            free(clientver_str);
                            free(datatype);
                            continue;
                        }
                        bytesSent = SendPackage(clifd, configstring);
                        s_bytesSent = int2String(bytesSent);
                        logentry = mkString("[INFO] (reciver) Konfiguracja zostala wyslana do ",  net.ipaddr, " (", s_bytesSent, " bytes)", NULL);
                        writeLog(lf, logentry);

                        close(clifd);
                        free(s_bytesSent);
                        free(configstring);
                    }             
                    free(system_id);
                    free(clientver_str);
		}
                
		close(net.sock);
		free(net.ipaddr);
		free(datatype);
		free(clientResponse);
	}
	close(netiffd);		// konczymy  nasluch
}
char * BuildPackage(systeminfo * info, monitoring * s_state, netifstats * n_stats) {
	char * s_uptime = long2String(info->uptime);
	char * s_hdd_total = ulong2String(info->hdd_total);
	char * s_hdd_free = ulong2String(info->hdd_free);
	char * s_ram_free = ulong2String(info->ram_free);
	char * s_ram_total = ulong2String(info->ram_total);
	char * s_curr_time = ulong2String(info->curr_time);
	char * s_config_ver = int2String(info->config_version);
        char * s_bytes_out = int2String(n_stats->bytes_out);
        char * s_bytes_in = int2String(n_stats->bytes_in);
	char * state_ok = "OK";
	char * state_err = "ERR";
	char * httpd_state = NULL;
	char * sshd_state = NULL;
        char * mysql_state = NULL;

	if(s_state->apache_status == 1)
		httpd_state = state_ok;
	else
		httpd_state = state_err;

	if(s_state->sshd_status == 1)
		sshd_state = state_ok;
	else
		sshd_state = state_err;
        if(s_state->mysql_status == 1)
		mysql_state = state_ok;
	else
		mysql_state = state_err;

	char * package = mkString(
			"[{datatype:\"sysinfo\",package:{",
			"monitoring:{",
			"httpd:", httpd_state,
			",sshd:", sshd_state,
                        ",mysql:", mysql_state,
			"},"
			"uptime:\"", s_uptime, "\",",
			"hostname:\"", info->hostname, "\",",
			"distro_name:\"", info->os, "\",",
			"hdd_total:\"", s_hdd_total, "\",",
			"hdd_free:\"", s_hdd_free, "\",",
			"ram_total:\"", s_ram_total, "\",",
			"ram_free:\"", s_ram_free, "\",",
			"ext_ip:\"", info->extip, "\",",
                        "eth_out:\"", s_bytes_out, "\",",
                        "eth_in:\"", s_bytes_in, "\",",
                        "cpu:\"", info->cpu, "\",",
			"config_ver:\"", s_config_ver, "\",",
			"curr_time:\"", s_curr_time, "\",",
			"systemid:\"", info->net_hwaddr, "\"}}]",
	NULL);

	size_t package_len = strlen(package) + 1;
	char * json = (char *) malloc(package_len * sizeof(char));
	memset(json, '\0', package_len);
	strncpy(json, package, package_len);

	// czyscimy pozostalosci
	free(s_uptime);
	free(s_hdd_total);
	free(s_hdd_free);
	free(s_ram_total);
	free(s_curr_time);
	free(s_ram_free);
	free(s_config_ver);
        free(s_bytes_in);
        free(s_bytes_out);
	free(package);

	return json;
}
void SendData(char * mode, char * server, int port, FILE * lf) {
	int confd;
	char * logentry = NULL;
	systeminfo osinfo;				// informacje na temat serwera
	monitoring s_state;				// status dzialania uslug na serwerze
        netifstats netstats;                            // statystyki interfejsu sieciowego
	char buff[NET_BUFFER];			// bufor, ktory sluzy do przeslania danych przez siec
	memset(buff, '\0', NET_BUFFER);
	char * json = NULL;				// dane do przeslania

	// wskazniki do funkcji pobierajacych dane liczbowe z systemu
	unsigned long (*SysInfo[6])(void) = { getuptime, DiskSizeTotal, DiskSizeFree, ramFree, ramTotal, getCurrentTime };

	// wskazniki do funkcji weryfikujacych dzialanie uslug w systemie.
	int (*check[])(void) = { apacheAlive, sshdAlive, mysqlAlive };

	while(1) {
		if((confd = connector(server, port)) < 0) {
			switch(confd) {
				case -1 : logentry = mkString("[WARNING] (sender) Polaczenie nie moze zostac zainicjowane", NULL); 	break;
				case -2 : logentry = mkString("[WARNING] (sender) Nieznany host: ", server, NULL); 					break;
				case -3 : logentry = mkString("[WARNING] (sender) Blad polaczenia z hostem: ", server, NULL); 		break;
			}
			writeLog(lf, logentry);
			sleep(HEARTBEAT);
			continue;
		}
		/*
		 * Wszelkie informacje na temat systemu operacyjnego sa wysylane jedynie z hosta, ktory
		 * jest klientem
		 */
		if(!strcmp(mode, "client")) {
			// pobieramy informacje na temat systemu
			InitSystemInformation(&osinfo);
			if(!getSystemInformation(&osinfo, SysInfo, 6, lf)) {
				logentry = mkString("[WARNING] (sender) Blad pobierania informacji z systemu", NULL);
				writeLog(lf, logentry);
			}
			// sprawdzamy stan dzialania uslug
			getServiceStatus(&s_state, check);
                        // statystyki interfejsu sieciowego
                        if(!getNetifStats(&netstats)) {
                            netstats.bytes_in = 0;
                            netstats.bytes_out = 0;
                        }
			json = BuildPackage(&osinfo, &s_state, &netstats);		// budujemy jsona z danymi
			// sprawdzamy, czy serwer jest gotowy
			if(!waitForHEllo(confd)) {
				logentry = mkString("[WARNING] (sender) Serwer nie jest gotowy, ponawiam probe...", NULL);
				writeLog(lf, logentry);
				close(confd);
				ClearSystemInformation(&osinfo);
				sleep(HEARTBEAT);
				continue;
			}
			// wysylamy dane
			if(!SendPackage(confd, json)) {
				logentry = mkString("[WARNING] (sender) Blad przeslania informacji z systemu", NULL);
				writeLog(lf, logentry);
			}
			free(json);
			ClearSystemInformation(&osinfo);
		}
		close(confd);
		sleep(HEARTBEAT);
	}
}
int clientNeedUpdate(char * clientData) {
	// parsujemy wersje konfiguracji otrzymana od klienta
	char * verStr = getOptVal(clientData, "config_ver");
	int clientConfVer = atoi(verStr);

	// pobieramy z bazy wersje konfiguracji na podstawie ID klienta
	char * hostID = getOptVal(clientData, "systemid");
	int dbConfVer = checkDBConfigVer(hostID);

	free(hostID);
	free(verStr);

	if(clientConfVer < dbConfVer)
		return 1;
	else
		return 0;
}
char * buildConfigPackage(hostconfig * data) {
    char * package              = NULL;               // output package string
    char * vhost_package        = NULL;               // vhosts definitions
    char * htusers_package      = NULL;               // htpasswd accounts data
    char * sysusers_package     = NULL;               // system accounts data
    char * dbnames_package      = NULL;               // database names data
    char * dbusers_package      = NULL;               // database users data
    char * dbgrants_package     = NULL;               // database privileges data
    size_t package_size = 0;                          // memory size needed to hold whole data
    
    // defined scopes
    vhostData * vh          = data->httpd.vhost;      // vhosts configuration
    htpasswdData * htpass   = data->httpd.htpasswd;   // htpasswd accounts
    sysuser * su            = data->sysUsers;         // system users accounts
    dbinfo * dbi            = data->sqldb.db;         // database names
    dbuser * dbu            = data->sqldb.dbusers;    // database users data
    grants * dbg            = data->sqldb.dbgrants;   // database privilege informations
    
    // global package keynames
    char * package_header = "[datatype:\"hostconfig\",";
    
    // obtaining size of defined scopes
    if(vh != NULL)
        package_size += getVhostPackageSize(vh, "apache");
    if(htpass != NULL)
        package_size += htusersDataSize(htpass, "htusers");
    if(su != NULL)
        package_size += getSysUsersPackageSize(su, "sysusers");
    if(dbi != NULL)
        package_size += DBnamesDataSize(dbi, "db_name");
    if(dbu != NULL)
        package_size += DBusersDataSize(dbu, "db_user");
    if(dbg != NULL)
        package_size += DBgrantsDataSize(dbg, "db_privs");
    package_size += strlen(package_header) + 2;
    
    // preparing memory
    package = (char *) malloc(package_size * sizeof(char));
    memset(package, '\0', package_size);
    
    strncpy(package, package_header, strlen(package_header));
    if(vh != NULL) {
        vhost_package = apacheConfigPackage(vh, "apache");
        strncat(package, vhost_package, strlen(vhost_package));
        free(vhost_package);
    }
    if(htpass != NULL) {
        htusers_package = htpasswdConfigPackage(htpass, "htusers");
        strncat(package, htusers_package, strlen(htusers_package));
        free(htusers_package);
    }
    if(su != NULL) {
        sysusers_package = sysusersPackage(su, "sysusers");
        strncat(package, sysusers_package, strlen(sysusers_package));
        free(sysusers_package);
    }
    if(dbi != NULL) {
        dbnames_package = DBNamesConfigPackage(dbi, "db_name");
        strncat(package, dbnames_package, strlen(dbnames_package));
        free(dbnames_package);
    }
    if(dbu != NULL) {
        dbusers_package = DBusersConfigPackage(dbu, "db_user");
        strncat(package, dbusers_package, strlen(dbusers_package));
        free(dbusers_package);
    }
    if(dbg != NULL) {
        dbgrants_package = DBgrantsConfigPackage(dbg, "db_privs");
        strncat(package, dbgrants_package, strlen(dbgrants_package));
        free(dbgrants_package);
    }
    *(package + strlen(package)) = ']';
    
    return package;
}
int ReadHostConfig(char * hostid, hostconfig * conf, ver * cfgver, int clientver, FILE * lf) {
    int status = 0;         // status funkcji: 1 - sukces, 0 - error
    ver * curr = cfgver;
    
    // inicjujemy struktury
    conf->httpd.htpasswd = NULL;
    conf->httpd.vhost = NULL;
    conf->sysUsers = NULL;
    conf->sqldb.db = NULL;
    conf->sqldb.dbusers = NULL;
    conf->sqldb.dbgrants = NULL;
    
    while(curr) {
        if(!strcmp(curr->scope, "apache") && curr->version > clientver) {
            if((conf->httpd.vhost = getVhostData(hostid)))
                status = 1;
            else
                status = 0;
        }
        if(!strcmp(curr->scope, "htusers") && curr->version > clientver) {
            if((conf->httpd.htpasswd = getHtpasswdData(hostid)))
                status = 1;
            else
                status = 0;
        }
        if(!strcmp(curr->scope, "sysusers") && curr->version > clientver) {
            if((conf->sysUsers = getSystemAccounts(hostid)))
                status = 1;
            else
                status = 0;
        }
        if(!strcmp(curr->scope, "db_name") && curr->version > clientver) {
            if((conf->sqldb.db = getDatabaseNames(hostid)))
                status = 1;
            else
                status = 0;
        }
        if(!strcmp(curr->scope, "db_user") && curr->version > clientver) {
            if((conf->sqldb.dbusers = getDatabaseUsers(hostid)))
                status = 1;
            else
                status = 0;
        }
        if(!strcmp(curr->scope, "db_privs") && curr->version > clientver) {
            if((conf->sqldb.dbgrants = getDatabasePrivileges(hostid)))
                status = 1;
            else
                status = 0;
        }
        curr = curr->next;
    }
    
    return status;
}
ver * checkVersions(char * systemid) {
    extern MYSQL * dbh;
    MYSQL_RES * res;
    MYSQL_ROW row;
    
    char * query = mkString("SELECT scope, version FROM configver WHERE systemid = ",
                            "(SELECT id FROM sysinfo where system_id = '", systemid,"')", NULL);
    
    ver * head = NULL;
    ver * curr = NULL;
    ver * prev = NULL;
    
    if(mysql_query(dbh, query)) {
        free(query);
        return NULL;
    }
    if((res = mysql_store_result(dbh)) == NULL) {
        free(query);
        return NULL;
    }
    if(mysql_num_rows(res) == 0)
        return NULL;
    
    while((row = mysql_fetch_row(res))) {
        curr = malloc(sizeof(ver));
        curr->scope     = readData(row[0]);
        curr->version   = atoi(row[1]);
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
int readPackageVersion(char * str) {
    char * pos = str;       // aktualna pozycja w stringu
    char sver[11];          // string zawierajacy numer wersji
    int i = 0;              // index bufora
    int max = 0;            // liczba obszarow konfiguracyjnych
    int ver = 0;            // numer wersji w formie liczbowej
    int maxv = 0;           // wybieramy najwiekszy numer wersji w pakiecie
    
    // weryfikujemy ile jest wpisow z wersja konfiguracji
    while((pos = strstr(pos, "config_ver:")) != NULL) {
        max++;
        pos++;
    }
    if(max == 0)
        return -1;
    
    int vers[max];          // tablica przechowujaca odczytane wersje konfiguracji
    int n = 0;              // index tablicy
    
    pos = str;              // wracamy na poczatek stringa
    memset(sver, '\0', 11);
    while((pos = strstr(pos, "config_ver:")) != NULL) {
        pos += strlen("config_ver:");
        while(*pos != '}' && *pos != ',')
            sver[i++] = *pos++;
        ver = atoi(sver);
        vers[n++] = ver;
        i = 0;
        memset(sver, '\0', 11);
    }
    maxv = maxver(vers, n);
    return maxv;
}
int maxver(int vers[], int n) {
    int max = vers[0];      // zakladamy, ze pierwszy jest najwiekszy
    int i;
    
    for(i = 0; i < n; i++)
        if(vers[i] > max)
            max = vers[i];
    
    return max;
}
char * backMessage(resp * rsp) {
    char * message      = NULL;      // result string
    char * entry        = NULL;      // piece of configuration
    char * dbid         = NULL;      // string form
    char status[2];                  // status in string form
    resp * pos          = rsp;       // begin of response data
    size_t memsize      = 0;         // memory size for whole message

    // key string constants
    char * header = "datatype:\"StatusChange\",";

    while(pos) {
        dbid = int2String(pos->dbid);
        status[0] = pos->status;
        status[1] = '\0';
        entry = mkString("{scope:", pos->scope, ",", dbid, ":", status, "},", NULL);
        memsize += strlen(entry);
        free(entry);
        free(dbid);
        pos = pos->next;
    }

    memsize += strlen(header) + 1;
    
    message = (char *) malloc(memsize * sizeof(char));
    memset(message, '\0', memsize);
    
    strncpy(message, header, strlen(header));
    pos = rsp;
    while(pos) {
        dbid = int2String(pos->dbid);
        status[0] = pos->status;
        status[1] = '\0';
        entry = mkString("{scope:", pos->scope, ",", dbid, ":", status, "},", NULL);
        strncat(message, entry, strlen(entry));
        free(entry);
        free(dbid);
        pos = pos->next;
    }
    
    return message;
}
void cleanMSGdata(resp * rsp) {
    resp * curr = rsp;
    
    free(curr->scope);
    if(curr->next != NULL)
      cleanMSGdata(curr->next);
    free(curr);
}
resp * parseClientMessage(char * str) {
    char * pos = str;
    const int Size = 128;
    int i = 0;
    char buff[Size];
    resp * rhead = NULL;
    resp * rcurr = NULL;
    resp * rprev = NULL;
    
    while((pos = strstr(pos, "scope:")) != NULL) {
        pos += strlen("scope:");
        rcurr = (resp *) malloc(sizeof(resp));
        memset(buff, '\0', Size);
        i = 0;
        
        while(*pos != '}') {
            if(*pos == ',') {
                rcurr->scope = (char *) malloc((strlen(buff) + 1) * sizeof(char));
                memset(rcurr->scope, '\0', strlen(buff) + 1);
                strncpy(rcurr->scope, buff, strlen(buff));
                memset(buff, '\0', Size);
                i = 0;
                pos++;
            }
            else if(*pos == ':') {
                rcurr->dbid = atoi(buff);
                rcurr->status = *(pos + 1);
                rcurr->next = NULL;
                memset(buff, '\0', Size);
                i = 0;
                pos++;
            }
            else
                buff[i++] = *pos++;
        }
        if(rhead == NULL)
            rhead = rcurr;
        else
            rprev->next = rcurr;
        rprev = rcurr;
    }
    return rhead;
}
resp *  respStatus(char * scope, char status, int dbid) {
    resp * node = NULL;
    size_t len = 0;
    
    node = (resp *) malloc(sizeof(resp));
    node->status = status;
    node->dbid = dbid;
    len = strlen(scope) + 1;
    node->scope = (char *) malloc(len * sizeof(char));
    memset(node->scope, '\0', len);
    strncpy(node->scope, scope, len);
    node->next = NULL;
    
    return node;
}
char * getOptVal(char * json, char * key) {
    char * result = NULL;                               // option value
    char * pos = strstr(json, key) + strlen(key) + 1;   // let's move to the option value
    char * begin = pos;                                 // save original position
    int dq = 0;                                         // double quotes count
    int memsize = 0;                                    // memory size needed to hold option value
    int memindex = 0;                                   // current position in output buffer
    
    // calculate memory size
    while(*pos && dq < 2) {
        if(*pos == '"')
            dq++;
        else
            memsize++;
        pos++;
    }
    memsize += 1;
    
    // allocate memory for option value
    result = (char *) malloc(memsize * sizeof(char));
    memset(result, '\0', memsize);
    
    // reset initial values, double quote count and string position
    // and copy value into output buffer
    pos = begin;
    dq = 0;
    while(*pos && dq < 2) {
        if(*pos == '"')
            dq++;
        else
            result[memindex++] = *pos;
        pos++;
    }
    return result;
}