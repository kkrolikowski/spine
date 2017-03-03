/* core.c -- podstawowe funkcje programu */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "core.h"
#include "network.h"
#include "sysconfigdata.h"
#include "database.h"
#include "apache.h"
#include "monitoring.h"
#include "sysusers.h"
#include "commondata.h"

int savePidFile(int pid) {
	FILE * pf;

	if((pf = fopen(PID_PATH, "w")) == NULL)
		return 0;
	fprintf(pf, "%d", pid);
	fclose(pf);

	return 1;
}
int existPidFile(char * path) {
	FILE * pf;

	if((pf = fopen(path, "r")) == NULL)
		return 0;
	else {
		fclose(pf);
		return 1;
	}
}
char * mkString(char * qstr, ...) {
    va_list String;                     // czesc stringa
    size_t stringLenght = 0;            // liczba znakow w calym stringu
    char * str = NULL;                  // string wynikowy

    // adres pierwszej i kolejnej czesci lancucha
    // na razie maja ten sam adres.
    char * Sfirst = qstr;
    char * Snext = qstr;

    // obliczamy dlugosc calego stringa
    va_start(String, qstr);
    while(Snext != NULL) {
        stringLenght += strlen(Snext);
        Snext = va_arg(String, char *);
    }
    va_end(String);

/*
 * Przechodzimy do poczatku i laczymy wszystkie czesci ze soba
*/
    str = (char *) malloc((stringLenght + 1) * sizeof(char));   // alokujemy pamiec
    if(str != NULL) {
        memset(str, '\0', stringLenght + 1);
        Snext = Sfirst;                       // wracamy na poczatek
        va_start(String, qstr);
        while(Snext != NULL) {
            if(str[0] == '\0')                // Sprawdzamy czy obszar pamieci jest pusty
                strcpy(str, Snext);           // jesli tak, to kopiujemy pierwsza czesc
            else							  // w przeciwnym wypadku doklejamy nastepna
                strcat(str, Snext);
            Snext = va_arg(String, char *);
        }
        va_end(String);
    }
    return str;
}
char * timestamp(void) {
	time_t epoch;
	char * timestamp = NULL;
	struct tm * now;

	time(&epoch);
	timestamp = (char *) malloc(20 * sizeof(char));
	memset(timestamp, '\0', 20);
	now = localtime(&epoch);
	strftime(timestamp, 20, "%F %T", now);

	return timestamp;
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
char * ulong2String(unsigned long n) {
	char tmp[12];
	memset(tmp, '\0', 12);
	char * str = NULL;
	size_t len = 0;

	sprintf(tmp, "%lu", n);
	len = strlen(tmp) + 1;
	str = (char *) malloc(len * sizeof(char));
	memset(str, '\0', len);
	strcpy(str, tmp);

	return str;
}
char * int2String(int n) {
	char tmp[11];
	memset(tmp, '\0', 11);
	char * str = NULL;
	size_t len = 0;

	sprintf(tmp, "%d", n);
	len = strlen(tmp) + 1;
	str = (char *) malloc(len * sizeof(char));
	memset(str, '\0', len);
	strcpy(str, tmp);

	return str;
}
char * long2String(long n) {
	char tmp[12];
	memset(tmp, '\0', 12);
	char * str = NULL;
	size_t len = 0;

	sprintf(tmp, "%ld", n);
	len = strlen(tmp) + 1;
	str = (char *) malloc(len * sizeof(char));
	memset(str, '\0', len);
	strcpy(str, tmp);

	return str;
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
		if((datatype = jsonVal(clientResponse, "datatype")) == NULL) {
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
                            if(config.httpd.vhost != NULL)
                                apacheSetup(config.httpd, os, lf);
                            if(config.sysUsers != NULL) {
                                if((updateMSGdata = updateUserAccounts(config.sysUsers, os, lf)) != NULL) {
                                    updateMSGdataString = backMessage(updateMSGdata);
                                    clifd = connector(net.ipaddr, 2016);
                                    SendPackage(clifd, updateMSGdataString);
                                    close(clifd);
                                    free(updateMSGdataString);
                                    cleanMSGdata(updateMSGdata);
                                }
                                cleanSysUsersData(config.sysUsers);
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
                    system_id = jsonVal(clientResponse, "systemid");
                    clientver_str = jsonVal(clientResponse, "config_ver");
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
                        configstring = BuildConfigurationPackage(&config);
                        clifd = connector(net.ipaddr, 2016);
                        SendPackage(clifd, configstring);

                        logentry = mkString("[INFO] (reciver) Konfiguracja zostala wyslana do ",  net.ipaddr, NULL);
                        writeLog(lf, logentry);

                        close(clifd);
                        free(configstring);
                    }
                    cleanWWWConfiguration(system_id);                
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

	if(s_state->apache_status == 1)
		httpd_state = state_ok;
	else
		httpd_state = state_err;

	if(s_state->sshd_status == 1)
		sshd_state = state_ok;
	else
		sshd_state = state_err;

	char * package = mkString(
			"[{datatype:sysinfo,package:{",
			"monitoring:{",
			"httpd:", httpd_state,
			",sshd:", sshd_state,
			"},"
			"uptime:", s_uptime, ",",
			"hostname:", info->hostname, ",",
			"distro_name:", info->os, ",",
			"hdd_total:", s_hdd_total, ",",
			"hdd_free:", s_hdd_free, ",",
			"ram_total:", s_ram_total, ",",
			"ram_free:", s_ram_free, ",",
			"ext_ip:", info->extip, ",",
                        "eth_out:", s_bytes_out, ",",
                        "eth_in:", s_bytes_in, ",",
                        "cpu:", info->cpu, ",",
			"config_ver:", s_config_ver, ",",
			"curr_time:", s_curr_time, ",",
			"systemid:", info->net_hwaddr, "}}]",
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
char * jsonVal(const char * json, const char * pattern) {
	char * val = NULL;
        char * val_pos = NULL;

	size_t pattern_len = strlen(pattern);
        if((val_pos = strstr(json, pattern)) != NULL)
            val_pos += (pattern_len + 1);
        else
            return NULL;

	int i = 0;
	size_t len = 0;
	char tmp[PACKAGE_SIZE];
	memset(tmp, '\0', PACKAGE_SIZE);

	while(*val_pos != ',' &&
			(
			(*val_pos != '}' || *(val_pos+1) != ',') &&
			(*val_pos != '}' || *(val_pos+1) != ']') &&
			(*val_pos != '}' || *(val_pos+1) != '}')
			)
		)
	{
		tmp[i] = *val_pos;
		val_pos++;
		i++;
	}

	len = strlen(tmp) + 1;
	val = (char *) malloc(len * sizeof(char));
	memset(val, '\0', len);
	strcpy(val, tmp);

	return val;
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
	int (*check[])(void) = { apacheAlive, sshdAlive };

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
	char * verStr = jsonVal(clientData, "config_ver");
	int clientConfVer = atoi(verStr);

	// pobieramy z bazy wersje konfiguracji na podstawie ID klienta
	char * hostID = jsonVal(clientData, "systemid");
	int dbConfVer = checkDBConfigVer(hostID);

	free(hostID);
	free(verStr);

	if(clientConfVer < dbConfVer)
		return 1;
	else
		return 0;
}
char * BuildConfigurationPackage(hostconfig * data) {
        char * package = NULL;          // pelen pakiet danych
        char * sysusers = NULL;         // konta uzytkownikow
        char * vhosts = NULL;           // konfiguracja vhostow
        char * htusers = NULL;          // konfiguracja htpasswd
        char * s_vhost_count = NULL;    // liczba vhostow (string)
        char * s_htusers_count = NULL;  // liczba kont htpasswd (string)
        int size = 0;                   // ilosc pamieci do zaalokowania
        const int DataTypes = 2;        // liczba obszarow konfiguracji:
                                        // - apache, sysusers
        int vhost_count = 0;            // liczba odczytanych vhostow
        int htusers_count = 0;          // liczba odczytanych kont htpasswd
        char * k_htpasswd_count = "htpasswd_count:";
        char * dataType_braces = "{},";

        // odczytujemy liczbe vhostow i zamieniamy ja na string
        if(data->httpd.vhost != NULL) {
            vhost_count = getVhostsCount(data->httpd.vhost);
            s_vhost_count = int2String(vhost_count);
        }
        // odczytujemy liczbe kont htpasswd i zamieniamy ja na string
        if(data->httpd.htpasswd != NULL) {
            htusers_count = getHTusersCount(data->httpd.htpasswd);
            s_htusers_count = int2String(htusers_count); 
        }   
        // obliczamy ile bedziemy potrzebowali pamieci na zbudowanie pakietu
        if(data->sysUsers != NULL)
            size += getSysUsersPackageSize(data->sysUsers);
        if(data->httpd.vhost != NULL || data->httpd.htpasswd != NULL)
            size += getApachedataSize(data->httpd);
        if(data->httpd.htpasswd != NULL)
            size += strlen(s_htusers_count) + strlen(k_htpasswd_count);
        size += (strlen(dataType_braces) * DataTypes) + strlen("[datatype:hostconfig]") + 1; 

        // alokujemy cala potrzebna pamiec
        package = (char *) malloc(size * sizeof(char));
        memset(package, '\0', size);
        
        // odczytujemy poszczegolne sekcje konfiguracji
        if(data->sysUsers != NULL)
            sysusers = sysusersPackage(data->sysUsers);  
        if(data->httpd.vhost != NULL)
            vhosts = apacheConfigPackage(data->httpd);   
        if(data->httpd.htpasswd != NULL)
            htusers = readHtpasswdData(data->httpd.htpasswd);
    
        // skladamy pakiet w calosc
	strncpy(package, "[datatype:hostconfig,", strlen("[datatype:hostconfig,") + 1);
        if(data->sysUsers != NULL)
            strncat(package, sysusers, strlen(sysusers) + 1);
        if(data->httpd.vhost != NULL)
            strncat(package, vhosts, strlen(vhosts) + 1);
        if(data->httpd.htpasswd != NULL)
            strncat(package, htusers, strlen(htusers) + 1);
	if(data->httpd.htpasswd != NULL) {
            strncat(package, k_htpasswd_count, strlen(k_htpasswd_count) + 1);
            strncat(package, s_htusers_count, strlen(s_htusers_count) + 1);
        }
	strncat(package, "}]", 3);

        // zwalniamy pamiec
	if(s_htusers_count != NULL)   free(s_htusers_count);
        if(s_vhost_count != NULL)     free(s_vhost_count);
        if(htusers != NULL)           free(htusers);
        if(vhosts != NULL)            free(vhosts);
        if(sysusers != NULL)          free(sysusers);
        
	return package;
}
int fileExist(char * path) {
	FILE * fp = NULL;

	if((fp = fopen(path, "r")) == NULL)
		return 0;
	else {
		fclose(fp);
		return 1;
	}
}
int ReadHostConfig(char * hostid, hostconfig * conf, ver * cfgver, int clientver, FILE * lf) {
    int status = 0;         // status funkcji: 1 - sukces, 0 - error
    ver * curr = cfgver;
    
    // inicjujemy struktury
    conf->httpd.htpasswd = NULL;
    conf->httpd.vhost = NULL;
    conf->sysUsers = NULL;
    
    while(curr) {
        if(!strcmp(curr->scope, "apache") && curr->version > clientver) {
            conf->httpd = ReadWWWConfiguration(hostid, lf);
            status = 1;
        }
        if(!strcmp(curr->scope, "sysusers") && curr->version > clientver) {
            conf->sysUsers = getSystemAccounts(conf, hostid);
            status = 1;
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
    resp * pos = rsp;
    int itemcnt = 0;
    int commacnt = 0;
    int coloncnt = 0;
    size_t messageSize = 0;
    char * tmp = NULL;
    char * message = NULL;
    char * header = "datatype:StatusChange,scope:";
    
    // obtain amount of memory to allocate
    while(pos) {
        tmp = int2String(pos->dbid);
        messageSize += strlen(tmp);
        free(tmp);
        itemcnt++;
        pos = pos->next;
    }
    commacnt = itemcnt - 1;
    coloncnt = itemcnt;
    messageSize += strlen(header) + itemcnt + coloncnt + commacnt + 1;
    messageSize += strlen(rsp->scope) + 1;
    
    // prepare memory
    message = (char *) malloc(messageSize * sizeof(char));
    memset(message, '\0', messageSize);
    
    // create package
    strncpy(message, header, strlen(header));
    strncat(message, rsp->scope, strlen(rsp->scope));
    *(message+strlen(message)) = ',';
    pos = rsp;
    while(pos) {
        tmp = int2String(pos->dbid);
        strncat(message, tmp, strlen(tmp));
        free(tmp);
        *(message+strlen(message)) = ':';
        *(message+strlen(message)) = pos->status;
        if(pos->next != NULL)
            *(message+strlen(message)) = ',';
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
    char * pos = strstr(str, "scope") + strlen("scope:");    // begin of scope;
    char * scope = NULL;
    size_t len = 0;
    
    // local buffer for ID string
    const int BufSize = 128;
    char buff[BufSize];
    int i = 0;
    memset(buff, '\0', BufSize);
    
    // get the scope
    while(*pos != ',')
        buff[i++] = *pos++;
    len = strlen(buff) + 1;
    scope = (char *) malloc(len * sizeof(char));
    memset(scope, '\0', len);
    strncpy(scope, buff, len);
    
    // preparing node of data
    resp * head = NULL;
    resp * curr = NULL;
    resp * prev = NULL;
    
    // preparing buffer and moving to first value to read
    memset(buff, '\0', BufSize);
    i = 0;
    pos++;
    while(*pos) {
        if(*pos != ':')
            buff[i++] = *pos++;
        else {
            // since we have whole ID read, we can create a node of data
            curr = (resp *) malloc(sizeof(resp));
            curr->dbid = atoi(buff);            
            curr->status = *++pos;          // status is following the colon
            curr->scope = (char *) malloc(len * sizeof(char));
            memset(curr->scope, '\0', len);
            strncpy(curr->scope, scope, len);
            curr->next = NULL;
            
            // now we can clear the buffer and reset buffer index
            memset(buff, '\0', BufSize);
            i = 0;
            
            // if the following character is not the \0 character we can go
            // to the next numeric value skipping comma sign
            if(*(pos+1))
                pos += 2;
            
            // binding nodes together
            if(head == NULL)
                head = curr;
            else
                prev->next = curr;
            prev = curr;
        }
    }
    free(scope);
    
    return head;
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