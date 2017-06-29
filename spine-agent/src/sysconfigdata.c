/* sysconfigdata.c -- zestaw funkcji do odczytywania i konfiguracji systemu */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/statvfs.h>
#include <sys/utsname.h>
#include <net/if.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <glibtop/cpu.h>
#include "core.h"
#include "sysconfigdata.h"
#include "network.h"
#include "monitoring.h"
#include "commondata.h"
#include "toolkit.h"
#include "scopes/sysusers.h"
#include "configdata/extractpkg.h"

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
int getSystemInformation(systeminfo * sys, unsigned long (*SysInfo[])(void), int n, FILE * lf) {
	int status = 0;
	int i;
	char * logentry = NULL;

	for(i = 0; i < n; i++) {
		switch(i) {
			case 0: sys->uptime = SysInfo[i](); status = 1; break;
			case 1: sys->hdd_total = SysInfo[i](); status = 1; break;
			case 2: sys->hdd_free = SysInfo[i](); status = 1; break;
			case 3: sys->ram_free = SysInfo[i](); status = 1; break;
			case 4: sys->ram_total = SysInfo[i](); status = 1; break;
			case 5: sys->curr_time = SysInfo[i](); status = 1; break;
		}
	}
	if((sys->net_hwaddr = getMacAddress()) != NULL)
		status = 1;
	if((sys->hostname = getHostname()) != NULL)
		status = 1;
	if((sys->config_version = readLocalConfigVersion()) >= 0)
		status = 1;
	if((sys->os = linuxDistro()) != NULL)
		status = 1;
	if((sys->extip = getExternalIP()) != NULL) {
		writeIPCache(sys->extip);
		status = 1;
	}
	else {
		logentry = mkString("[WARNING] Problem z odczytem danych z IP API, pobieram informacje z cache'a", NULL);
		writeLog(lf, logentry);
		sys->extip = readIPCache();
		status = 1;
	}
        sys->cpu = CPUusage();
        
	return status;
}
void InitSystemInformation(systeminfo * sys) {
	sys->uptime = 0L;
	sys->hdd_free = 0L;
	sys->hdd_total = 0L;
	sys->ram_free = 0L;
	sys->ram_total = 0L;
	sys->curr_time = 0L;
	sys->net_hwaddr = NULL;
	sys->hostname = NULL;
	sys->os = NULL;
	sys->ip = NULL;
	sys->extip = NULL;
        sys->cpu = NULL;
	sys->config_version = 0;
}
void ClearSystemInformation(systeminfo * sys) {
	sys->uptime = 0L;
	sys->hdd_free = 0L;
	sys->hdd_total = 0L;
	sys->ram_free = 0L;
	sys->ram_total = 0L;
	sys->curr_time = 0L;
	sys->config_version = 0;
	free(sys->extip);
	free(sys->net_hwaddr);
	free(sys->hostname);
	free(sys->os);
        free(sys->cpu);
}
int readLocalConfigVersion(void) {
	int ver = 0;
	FILE * vf;

	if(access(SPOOL_DIR, F_OK) < 0)
		mkdir(SPOOL_DIR, 0755);
	if((vf = fopen(VERSION_FILE, "r")) == NULL) {
		if((vf = fopen(VERSION_FILE, "w")) == NULL)
			return -1;
		writeLocalConfigVersion(ver);
		fclose(vf);
		return 0;
	}
	if(fread(&ver, sizeof(ver), 1, vf)) {
		fclose(vf);
		return ver;
	}
	else {
		fclose(vf);
		return 0;
	}
}
int writeLocalConfigVersion(int ver) {
	FILE * vf;

	if(access(SPOOL_DIR, F_OK) < 0)
		mkdir(SPOOL_DIR, 0755);
	if((vf = fopen(VERSION_FILE, "w")) == NULL)
		return 0;
	fwrite(&ver, sizeof(ver), 1, vf);
	fclose(vf);

	return 1;
}
void ParseConfigData(char * json, hostconfig * conf) {
    // inicjujemy zmienne, ktore przechowaja odebrana konfiguracje
    conf->datatype = NULL;
    conf->httpd.htpasswd = NULL;
    conf->httpd.vhost = NULL;
    conf->sysUsers = NULL;
    conf->sqldb.db = NULL;
    conf->sqldb.dbusers = NULL;
    conf->sqldb.dbgrants = NULL;

    char * pos = json;          // ustawiamy sie na poczatku pakietu
    // przetwarzamy typ pakietu
    conf->datatype = getOptVal(json, "datatype");
     
    if((pos = strstr(json, "scope:sysusers")) != NULL)
       conf->sysUsers = ParseConfigDataSYSUSERS(pos);
    if((pos = strstr(json, "scope:apache")) != NULL)
       conf->httpd.vhost = ParseConfigDataAPACHE(pos);
    if((pos = strstr(json, "scope:htusers")) != NULL)
       conf->httpd.htpasswd = ParseConfigDataHTPASSWD(pos);
    if((pos = strstr(json, "scope:db_name")) != NULL)
        conf->sqldb.db = ParseConfigDataDBNAMES(pos);
    if((pos = strstr(json, "scope:db_user")) != NULL)
        conf->sqldb.dbusers = ParseConfigDataDBUSERS(pos);
    if((pos = strstr(json, "scope:db_privs")) != NULL)
        conf->sqldb.dbgrants = ParseConfigDataDBPRIVS(pos);
}
char * linuxDistro(void) {
	char buff[128];
	char * distro = NULL;
	struct utsname info;
	char * name = NULL;
	size_t len = 0;

	uname(&info);
	memset(buff, '\0', 128);
	strcpy(buff, info.release);
	strcat(buff, info.version);

	if(strstr(buff, "Ubuntu") != NULL)
		distro = "Ubuntu";
	else if(strstr(buff, "el6") != NULL)
		distro = "Centos6";
	else if(strstr(buff, "el7") != NULL)
		distro = "Centos7";

	len = strlen(distro) + 1;
	name = (char *) malloc(len * sizeof(char));
	memset(name, '\0', len);
	strncpy(name, distro, len);

	return name;
}
char * readIPCache(void) {
	FILE * cache;
	char * extip = NULL;
	size_t len = 0;
	char buff[16];

	memset(buff, '\0', 16);
	if((cache = fopen(IPAPI_CACHE, "r")) == NULL)
		return NULL;
	if(fscanf(cache, "%s", buff) < 1)
		return NULL;
	fclose(cache);

	len = strlen(buff) + 1;
	extip = (char *) malloc(len * sizeof(char));
	memset(extip, '\0', len);
	strncpy(extip, buff, len);

	return extip;
}
int writeIPCache(char * extip) {
	FILE * cache;

	if((cache = fopen(IPAPI_CACHE, "w")) == NULL)
		return -1;
	fprintf(cache, "%s", extip);
	fclose(cache);

	return 1;
}
unsigned long getCurrentTime(void) {
	return (unsigned long) time(NULL);
}
char * CPUusage(void) {
    // inicjujemy strukture i pobieramy wartosci na temat utylizacji cpu
    glibtop_cpu cpu;
    glibtop_get_cpu(&cpu);
    
    static int cpu_total_old = 0;   // zapamietujemy poprzednia wartosc calkowitej utylizacji CPU
    static int cpu_idle_old = 0;    // zapamietujemy poprzednia wartosc bezczynnosci CPU    
    
    // obliczamy procentowa utylizacje CPU
    int cpu_diff_total = cpu.total - cpu_total_old;
    int cpu_diff_idle = cpu.idle - cpu_idle_old;
    int usage = 100 * (cpu_diff_total - cpu_diff_idle) / cpu_diff_total;
    
    // przeksztalcamy wartosc liczbowa na string
    char buff[4];
    memset(buff, '\0', 4);
    snprintf(buff, 4, "%d", usage);
    size_t len = strlen(buff) + 1;
    char * usage_s = malloc(len * sizeof(char));
    memset(usage_s, '\0', len);
    strncpy(usage_s, buff, len);

    
    cpu_total_old = cpu.total;
    cpu_idle_old = cpu.idle;

    return usage_s;
}
