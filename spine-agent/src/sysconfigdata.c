/* sysconfigdata.c -- zestaw funkcji do odczytywania i konfiguracji systemu */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/ioctl.h>
#include <sys/statvfs.h>
#include <net/if.h>
#include <unistd.h>
#include <limits.h>
#include "sysconfigdata.h"

unsigned long getuptime(void) {
	struct sysinfo sys;

	if(!sysinfo(&sys))
		return sys.uptime;
	else
		return -1;
}
unsigned long ramTotal(void) {
	struct sysinfo sys;

	if(!sysinfo(&sys))
		return sys.totalram;
	else
		return -1;
}
unsigned long ramFree(void) {
	struct sysinfo sys;

	if(!sysinfo(&sys))
		return sys.freeram;
	else
		return -1;
}
char * getMacAddress() {
	int sock;
	struct ifreq ifr;
	char * iface = getNetInterfaceName();	// unstawiamy interfejs, ktorego mac-adres bedziemy odczytywac
	unsigned char * mac = NULL;				// tutaj znajdzie sie mac-adres
	char * macstr = (char *) malloc(18 * sizeof(char)); // tutaj bedzie string zawierajacy sformatowany mac-adres;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_ifru.ifru_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_ifrn.ifrn_name, iface, IFNAMSIZ-1);
	ioctl(sock, SIOCGIFHWADDR, &ifr);
	mac = (unsigned char *) ifr.ifr_ifru.ifru_hwaddr.sa_data;

	sprintf(macstr, "%.2x-%.2x-%.2x-%.2x-%.2x-%.2x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	free(iface);
	close(sock);
	return macstr;
}
char * getNetInterfaceName() {
	struct if_nameindex * netif = if_nameindex();
	struct if_nameindex * first_netif = netif;
	char * nifname = NULL;
	size_t nifname_len = 0;
	char tmp[16];
	memset(tmp, '\0', 16);

	netif = netif + 1;
	strcpy(tmp, netif->if_name);

	nifname_len = strlen(tmp) + 1;
	nifname = (char *) malloc(nifname_len * sizeof(char));
	strcpy(nifname, tmp);

	if_freenameindex(first_netif);
	return nifname;
}
unsigned long DiskSizeTotal() {
	struct statvfs fs;
	char * fsdir = "/";
	unsigned long totalsize = 0L;

	if(statvfs(fsdir, &fs))
		return 0;
	totalsize = fs.f_bsize * fs.f_blocks;

	return totalsize;
}
unsigned long DiskSizeFree() {
	struct statvfs fs;
	char * fsdir = "/";
	unsigned long freesize = 0L;

	if(statvfs(fsdir, &fs))
		return 0;
	freesize = fs.f_bsize * fs.f_bfree;

	return freesize;
}
char * getHostname() {
	char tmp[HOST_NAME_MAX];
	char * hostname = NULL;
	size_t hostname_len = 0;

	memset(tmp, '\0', sizeof(tmp));
	if(gethostname(tmp, sizeof(tmp)) > 0)
		return NULL;
	hostname_len = strlen(tmp) + 1;
	hostname = (char *) malloc(hostname_len * sizeof(char));
	strncpy(hostname, tmp, hostname_len);

	return hostname;
}
int getSystemInformation(systeminfo * sys, unsigned long (*SysInfo[])(void), int n) {
	int status = 0;
	int i;

	for(i = 0; i < n; i++) {
		switch(i) {
			case 0: sys->uptime = SysInfo[i](); status = 1; break;
			case 1: sys->hdd_total = SysInfo[i](); status = 1; break;
			case 2: sys->hdd_free = SysInfo[i](); status = 1; break;
			case 3: sys->ram_free = SysInfo[i](); status = 1; break;
			case 4: sys->ram_total = SysInfo[i](); status = 1; break;
		}
	}
	if((sys->net_hwaddr = getMacAddress()) != NULL)
		status = 1;
	if((sys->hostname = getHostname()) != NULL)
		status = 1;

	return status;
}
void InitSystemInformation(systeminfo * sys) {
	sys->uptime = 0L;
	sys->hdd_free = 0L;
	sys->hdd_total = 0L;
	sys->ram_free = 0L;
	sys->ram_total = 0L;
	sys->net_hwaddr = NULL;
	sys->hostname = NULL;
	sys->ip = NULL;
}
void ClearSystemInformation(systeminfo * sys) {
	sys->uptime = 0L;
	sys->hdd_free = 0L;
	sys->hdd_total = 0L;
	sys->ram_free = 0L;
	sys->ram_total = 0L;
	free(sys->net_hwaddr);
	free(sys->hostname);
	free(sys->ip);
}
