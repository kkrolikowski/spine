/* monitoring.c -- funkcje do monitorowania uslug */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/if_link.h>
#include <ifaddrs.h>
#include "monitoring.h"
#include "network.h"
#include "core.h"

int apacheAlive(void) {
	// request do wyslania do apacza
	char * http_req = mkString("HEAD / HTTP/1.1\nHost: localhost\n\n", NULL);

	// przygotowujemy bufor
	const int SIZE = 256;
	char buff[SIZE];
	memset(buff, '\0', SIZE);

	// nawiazujemy polaczenie z apaczem
	int http_fd = connector("localhost", 80);
	if(http_fd < 0) {
		free(http_req);
		return 0;
	}

	// wysylamy request
	strncpy(buff, http_req, SIZE);
	if(write(http_fd, buff, SIZE) < 0) {
		close(http_fd);
		free(http_req);
		return 0;
	}

	// przygotowujemy bufor i pobieramy odpowiedz
	memset(buff, '\0', SIZE);
	if(read(http_fd, buff, SIZE) < 0) {
		close(http_fd);
		free(http_req);
		return 0;
	}

	// sprawdzamy kod odpowiedzi apacza
	if((strstr(buff, "HTTP/1.1 200 OK") == NULL) && (strstr(buff, "HTTP/1.1 403 Forbidden") == NULL)) {
		close(http_fd);
		free(http_req);
		return 0;
	}

	// czynnosci koncowe
	free(http_req);
	close(http_fd);
	return 1;
}
int sshdAlive(void) {
	// przygotowujemy bufor
	const int SIZE = 256;
	char buff[SIZE];
	memset(buff, '\0', SIZE);

	// nawiazujemy polaczenie z apaczem
	int sshdfd = connector("localhost", 22);
	if(sshdfd < 0)
		return 0;

	// pobieramy odpowiedz
	if(read(sshdfd, buff, SIZE) < 0) {
		close(sshdfd);
		return 0;
	}

	// sprawdzamy odpowiedz sshd
	if((strstr(buff, "OpenSSH") == NULL)) {
		close(sshdfd);
		return 0;
	}

	// czynnosci koncowe
	close(sshdfd);
	return 1;
}
char * rawMonitoringData(const char * clientResp) {

	// przesuwamy sie na poczatek danych z monitoringu
	const char * vals = strstr(clientResp, "monitoring");
	vals += strlen("monitoring") + 2;

	// przygotowujemy bufor do odczytu danych
	char buff[1024];
	memset(buff, '\0', 1024);
	int i = 0;

	// odczytujemy dane
	while(*vals != '}')
		buff[i++] = *vals++;

	// kopiujemy odczytane dane do obszaru pamieci.
	int len = strlen(buff) + 1;
	char * raw = malloc(len * sizeof(char));
	memset(raw, '\0', len);
	strncpy(raw, buff, len);

	return raw;
}
int checks(char * raw) {
	int num = 0;
	char * cur = raw;

	while(*cur++)
		if(*cur == ':')
			num++;
	return num;
}
void ReadRawMonitoringData(char * raw, kv kvdata[], int n) {
	char * cur = raw;
	int i = 0;
	int m = 0;
	size_t len = 0;

	char tmp[16];
	memset(tmp, '\0', 16);

	while(*cur) {
		if(*cur == ':') {
			tmp[i] = '\0';
			len = strlen(tmp) + 1;
			kvdata[m].key = malloc(len * sizeof(char));
			memset(kvdata[m].key, '\0', len);
			strncpy(kvdata[m].key, tmp, len);
			memset(tmp, '\0', 16);
			i = 0;
			cur++;
		}
		if(*cur == ',') {
			tmp[i] = '\0';
			len = strlen(tmp) + 1;
			kvdata[m].val = malloc(len * sizeof(char));
			memset(kvdata[m].val, '\0', len);
			strncpy(kvdata[m++].val, tmp, len);
			memset(tmp, '\0', 16);
			i = 0;
			cur++;

		}
		tmp[i++] = *cur++;
	}
	tmp[i] = '\0';
	len = strlen(tmp) + 1;
	kvdata[m].val = malloc(len * sizeof(char));
	memset(kvdata[m].val, '\0', len);
	strncpy(kvdata[m].val, tmp, len);
}
void InitCheckData(kv data[], int n) {
	int i;

	for(i = 0; i < n; i++) {
		data[i].key = NULL;
		data[i].val = NULL;
	}
}
void ClearCheckData(kv data[], int n) {
	int i;

	for(i = 0; i < n; i++) {
		free(data[i].key);
		free(data[i].val);
	}
}
void getServiceStatus(monitoring * srvdata, int (*check[])(void)) {
	srvdata->apache_status = check[0]();
	srvdata->sshd_status  = check[1]();
}
int getNetifStats(netifstats * ifstats) {
    struct ifaddrs * netif, * netif_next;
    struct rtnl_link_stats * stats;
    int family;
    char * msg = NULL;
    
    if(getifaddrs(&netif) == -1)
        return 0;
    netif_next = netif->ifa_next;            // omijamy loopback
    family = netif_next->ifa_addr->sa_family;
    if(netif_next->ifa_data != NULL && family == AF_PACKET) {
        stats = netif_next->ifa_data;        // pobieramy statystyki
        ifstats->bytes_in = stats->rx_bytes;
        ifstats->bytes_out = stats->tx_bytes;
        freeifaddrs(netif);
        return 1;
    }
    else {
        freeifaddrs(netif);
        return 0;
    }
}