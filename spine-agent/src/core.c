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

	sprintf(tmp, "%ld", n);
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
	char * clientResponse = NULL;	// string przesylany przez klienta (json)
	char * datatype = NULL; 		// typ danych przesylanych do klienta
	char * system_id = NULL;		// indentyfikator systemu (mac-adress)
	char * os = NULL;				// nazwa dystrubucji Linuksa
	char * configstring = NULL;
	netinfo net;					// struktura przechowujaca ip oraz socket klienta
	hostconfig configdata;

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
			free(net.ipaddr);
			close(net.sock);
			sleep(5);
			continue;
		}
		if(!strcmp(mode, "client")) {
			os = linuxDistro();
			configdata = ParseConfigData(clientResponse);
			if(readLocalConfigVersion() < configdata.confVer) {
				if(!strcmp(configdata.datatype, "apache")) {
					if(configdata.vhost_num > 0) {
						apacheSetup(configdata, os, lf);
						if(writeLocalConfigVersion(configdata.confVer))
							logentry = mkString("[INFO] (reciver) Konfiguracja zostala zaktualizowana", NULL);
						else
							logentry = mkString("[WARNING] (reciver) blad aktualizacji wersji konfigruacji", NULL);
						writeLog(lf, logentry);

						clearVhostData(configdata.vhost, configdata.vhost_num);
					}
				}
			}
			free(os);
		}
		// jesli dane sa typu sysinfo, to znaczy, ze trzeba je zapisac w bazie danych
		if(!strcmp(datatype, "sysinfo")) {
			updateHostInfo(net.ipaddr, clientResponse, lf);
			if(clientNeedUpdate(clientResponse)) {
				system_id = jsonVal(clientResponse, "systemid");
				configdata = ReadWWWConfiguration(system_id);
				configstring = BuildConfigurationPackage(configdata);
				clifd = connector(net.ipaddr, 2016);
				SendPackage(clifd, configstring);
				close(clifd);
				free(configstring);
				free(system_id);

				//sendDataToClient(net.ipaddr);
				// lub sendDataToClient(net.sock);
				logentry = mkString("[INFO] (reciver) Jest gotowa nowa konfiguracja", NULL);
				writeLog(lf, logentry);
			}
		}
		close(net.sock);
		free(net.ipaddr);
		free(datatype);
		free(clientResponse);
	}
	close(netiffd);		// konczymy  nasluch
}
char * BuildPackage(systeminfo * info) {
	char * s_uptime = long2String(info->uptime);
	char * s_hdd_total = ulong2String(info->hdd_total);
	char * s_hdd_free = ulong2String(info->hdd_free);
	char * s_ram_free = ulong2String(info->ram_free);
	char * s_ram_total = ulong2String(info->ram_total);
	char * s_config_ver = int2String(info->config_version);

	char * package = mkString(
			"[{datatype:sysinfo,package:{uptime:", s_uptime, ",",
			"hostname:", info->hostname, ",",
			"distro_name:", info->os, ",",
			"hdd_total:", s_hdd_total, ",",
			"hdd_free:", s_hdd_free, ",",
			"ram_total:", s_ram_total, ",",
			"ram_free:", s_ram_free, ",",
			"ext_ip:", info->extip, ",",
			"config_ver:", s_config_ver, ",",
			"systemid:", info->net_hwaddr, "}}]",
	NULL);

	size_t package_len = strlen(package) + 1;
	char * json = (char *) malloc(package_len * sizeof(char));
	strncpy(json, package, package_len);

	// czyscimy pozostalosci
	free(s_uptime);
	free(s_hdd_total);
	free(s_hdd_free);
	free(s_ram_total);
	free(s_ram_free);
	free(s_config_ver);
	free(package);

	return json;
}
char * jsonVal(const char * json, const char * pattern) {
	char * val = NULL;

	size_t pattern_len = strlen(pattern);
	const char * val_pos = strstr(json, pattern) + pattern_len + 1;

	if(val_pos == NULL)
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
	strcpy(val, tmp);

	return val;
}
void SendData(char * mode, char * server, int port, FILE * lf) {
	int confd;
	char * logentry = NULL;
	systeminfo osinfo;				// informacje na temat serwera
	char buff[NET_BUFFER];			// bufor, ktory sluzy do przeslania danych przez siec
	memset(buff, '\0', NET_BUFFER);
	char * json = NULL;				// dane do przeslania

	// wskazniki do funkcji pobierajacych dane liczbowe z systemu
	unsigned long (*SysInfo[5])(void) = { getuptime, DiskSizeTotal, DiskSizeFree, ramFree, ramTotal };

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
			if(!getSystemInformation(&osinfo, SysInfo, 5)) {
				logentry = mkString("[WARNING] (sender) Blad pobierania informacji z systemu", NULL);
				writeLog(lf, logentry);
			}
			json = BuildPackage(&osinfo);		// budujemy jsona z danymi
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
char * BuildConfigurationPackage(hostconfig data) {
	size_t len = 0;
	char * tmp = NULL;

	// bufor na dane konfiguracyjne
	char buff[PACKAGE_SIZE];
	memset(buff, '\0', PACKAGE_SIZE);

	// string zawierajacy numer wersji konfiguracji
	char * s_config_ver = int2String(data.confVer);

	strcpy(buff, "[");
	tmp = apacheConfigPackage(data);
	strcat(buff, tmp);
	free(tmp);
	strcat(buff, "config_ver:");
	strcat(buff, s_config_ver);
	strcat(buff, "}]");

	len = strlen(buff) + 1;
	tmp = (char *) malloc(len + 1);
	memset(tmp, '\0', len);
	strncpy(tmp, buff, len);

	free(s_config_ver);

	return tmp;
}
char * apacheConfigPackage(hostconfig data) {
	int vidx;					// vhost index
	size_t len = 0;				// liczba bajtow konfiguracji
	char * numstr = NULL;		// string z liczby
	char * vhostdata = NULL; 	// zawartosc vhosta
	char * metainfo = NULL;		// dane pomocnicze
	char * configstr = NULL;	// string przechowujacy odczytana konfiguracje

	// bufor na dane konfiguracyjne
	char buff[PACKAGE_SIZE];
	memset(buff, '\0', PACKAGE_SIZE);

	strcpy(buff, "{datatype:apache,");		// definiujemy typ konfiguracji (apache)

	// przetwarzamy dodane vhosty apacza
	for(vidx = 0; vidx < data.vhost_num; vidx++) {

		numstr = int2String(vidx);
		metainfo = mkString("vhost_", numstr, NULL);
		strcat(buff, metainfo);

		vhostdata = mkString(
				":{ServerName:", data.vhost[vidx].ServerName,
				",ServerAlias:", data.vhost[vidx].ServerAlias,
				",DocumentRoot:", data.vhost[vidx].DocumentRoot,
				",ApacheOpts:", data.vhost[vidx].apacheOpts,
				",VhostAccessOrder:", data.vhost[vidx].vhost_access_order,
				",VhostAccessList:", data.vhost[vidx].vhost_access_list,
				",htaccess:", data.vhost[vidx].htaccess,
				",user:", data.vhost[vidx].user, "},", NULL);
		strcat(buff, vhostdata);

		free(vhostdata);
		free(numstr);
		free(metainfo);
	}

	// dodatkowe dane: liczba vhostow, ktore zostaly odczytane
	numstr = int2String(data.vhost_num);
	metainfo = mkString("vhost_num:", numstr, ",", NULL);
	strcat(buff, metainfo);

	// kopiujemy zawartosc bufora do pamieci
	len = strlen(buff) + 1;
	configstr = (char *) malloc(len * sizeof(char));
	memset(configstr, '\0', len);
	strncpy(configstr, buff, len);

	// czyscimy niepotrzebne dane
	clearVhostData(data.vhost, data.vhost_num);
	free(numstr);
	free(metainfo);

	return configstr;
}
void clearVhostData(struct wwwdata vhost[], int n) {
	int i;

	for(i = 0; i < n; i++) {
		free(vhost[i].DocumentRoot);
		free(vhost[i].ServerAlias);
		free(vhost[i].ServerName);
		free(vhost[i].htaccess);
		free(vhost[i].user);
		free(vhost[i].apacheOpts);
		free(vhost[i].vhost_access_list);
		free(vhost[i].vhost_access_order);
		free(vhost[i].htusers);
	}
}
