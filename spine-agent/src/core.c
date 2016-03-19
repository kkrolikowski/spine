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

#define PACKAGE_CHUNKS 5		// liczba kawalkow z ktorych sklada sie pakiet wysylany przez klienta

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
	char * clientResponse = NULL;	// string przesylany przez klienta (json)
	char * datatype = NULL; 		// typ danych przesylanych do klienta
	netinfo net;					// struktura przechowujaca ip oraz socket klienta

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
		// jesli dane sa typu sysinfo, to znaczy, ze trzeba je zapisac w bazie danych
		if(!strcmp(datatype, "sysinfo"))
			updateHostInfo(net.ipaddr, clientResponse, lf);

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

	// deklarujemy i inicujemy poszczegolne czesci skladowe pakietu
	char * package1 = mkString("[{datatype:sysinfo,package:{uptime:", s_uptime, ",", NULL);
	char * package2 = mkString("hostname:", info->hostname, ",", NULL);
	char * package3 = mkString("hdd_total:", s_hdd_total, ",", NULL);
	char * package4 = mkString("hdd_free:", s_hdd_free, ",", NULL);
	char * package5 = mkString("systemid:", info->net_hwaddr, "}}]", NULL);

	// obliczamy ile pamieci bedzie potrzeba na przechowanie calego pakietu
	char * packages[PACKAGE_CHUNKS] = { package1, package2, package3, package4, package5 };
	size_t package_len = 0;
	int i;
	for(i = 0; i < PACKAGE_CHUNKS; i++)
		package_len += strlen(packages[i]);
	package_len += 1;

	// sklejamy wszystkie czesi pakietu do kupy
	char * json = (char *) malloc(package_len * sizeof(char));
	strcpy(json, package1);
	strcat(json, package2);
	strcat(json, package3);
	strcat(json, package4);
	strcat(json, package5);

	// czyscimy pozostalosci
	cleanChunks(packages, PACKAGE_CHUNKS);
	free(s_uptime);
	free(s_hdd_total);
	free(s_hdd_free);

	return json;
}
void cleanChunks(char * parts[], int n) {
	int i;
	for(i = 0; i < n; i++)
		free(parts[i]);
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

	while(*val_pos != ',' && *val_pos != '}') {
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
			if(!getSystemInformation(&osinfo)) {
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
