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
#include <limits.h>
#include <dirent.h>
#include <time.h>
#include <glibtop/cpu.h>
#include "core.h"
#include "sysconfigdata.h"
#include "network.h"
#include "monitoring.h"
#include "commondata.h"
#include "sysusers.h"

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
    char * pos = json;          // ustawiamy sie na poczatku pakietu
    // przetwarzamy typ pakietu
    conf->datatype = jsonVal(pos, "datatype");
    
    if((pos = strstr(pos, "scope:sysusers")) != NULL)
        conf->sysUsers = ParseConfigDataSYSUSERS(pos);
    if((pos = strstr(pos, "scope:apache")) != NULL)
        ParseConfigDataAPACHE(json, &conf->httpd);
    
}
sysuser * ParseConfigDataSYSUSERS(char * json) {
    int i = 0;                                          // biezacy numer konta
    char * config_pos = NULL;                           // pozycja wzgledem user_(n)
    char * uheader = NULL;                              // tutaj znajdzie sie naglowek user_(n)
    char * index = NULL;                                // biezacy numer konta w formie stringu
    char * tmp = NULL;                                  // tymczasowe przechowanie stringu
    int userCount = JSONsearchString(json, "user_");    // calkowita liczba kont w pakiecie
    
    // inicjalizacja danych do listy laczonej
    sysuser * head = NULL;
    sysuser * curr = NULL;
    sysuser * prev = NULL;
    
    for(i = 0; i < userCount; i++) {
        index = int2String(i);
        uheader = mkString("user_", index, NULL);
        config_pos = strstr(json, uheader);
        
        curr = (sysuser *) malloc(sizeof(sysuser));
        
        curr->gecos         = jsonVal(config_pos, "gecos");
        curr->login         = jsonVal(config_pos, "username");
        curr->sha512        = jsonVal(config_pos, "password");
        
        tmp                 = jsonVal(config_pos, "active");
        curr->active        = atoi(tmp);
        free(tmp);
        tmp                 = jsonVal(config_pos, "uidgid");
        curr->uidgid        = atoi(tmp);
        free(tmp);
        tmp                 = jsonVal(config_pos, "shell");
        curr->shellaccess   = atoi(tmp);
        free(tmp);
        tmp                 = jsonVal(config_pos, "expire");
        curr->expiration    = atoi(tmp);
        free(tmp);
        curr->sshkey        = readSSHKeysFromPackage(strstr(config_pos, "sshkey_0"));
        curr->next = NULL;
        
        if(head == NULL)
            head = curr;
        else
            prev->next = curr;
        prev = curr;
        
        free(index);
        free(uheader);
    }
    return head;  
}
void ParseConfigDataAPACHE(char * json, httpdata * www) {
    int i;                      // biezacy numer vhosta
    char * config_pos = NULL;   // pozycja w stringu wzgledem vhost_(n)
    char * vheader = NULL;      // tutaj bedzie naglowek vhost_(n)
    char * index = NULL;        // biezacy numer vhosta w formie stringu
    char * authbasic = NULL;    // wartosc tekstowa skladnika password_access
    
    // przetwarzamy calkowita liczbe vhostow
    char * vhostnum_s = jsonVal(json, "vhost_num");
    int vhostCount = atoi(vhostnum_s);
    free(vhostnum_s);
    
    // przetwarzamy calkowita liczbe kont htpasswd
    char * htusers_count_s = jsonVal(json, "htpasswd_count");
    int htusersCount = atoi(htusers_count_s);
    free(htusers_count_s);
    
    char * htpasswd_s = NULL;   // string przechowujacy dane htpasswd

    // inicjujemy liste laczona
    vhostData * curr = NULL;
    vhostData * prev = NULL;
    vhostData * head = NULL;
    
    for(i = 0; i < vhostCount; i++) {
        index = int2String(i);
        vheader = mkString("vhost_", index, NULL);
        config_pos = strstr(json, vheader);
        authbasic = jsonVal(config_pos, "authbasic");
        
        curr = (vhostData *) malloc(sizeof(vhostData));
        curr->ServerName            = jsonVal(config_pos, "ServerName");
        curr->ServerAlias           = jsonVal(config_pos, "ServerAlias");
        curr->DocumentRoot          = jsonVal(config_pos, "DocumentRoot");
        curr->apacheOpts            = jsonVal(config_pos, "ApacheOpts");
        curr->vhost_access_order    = jsonVal(config_pos, "VhostAccessOrder");
        curr->vhost_access_list     = jsonVal(config_pos, "VhostAccessList");
        curr->htaccess              = jsonVal(config_pos, "htaccess");
        curr->password_access       = atoi(authbasic);
        curr->user                  = jsonVal(config_pos, "user");
        curr->htusers               = jsonVal(config_pos, "htusers");
        curr->status                = jsonVal(config_pos, "vhoststatus");
        curr->purgedir              = jsonVal(config_pos, "purgedir");
        curr->next = NULL;
        
        if(head == NULL)
            head = curr;
        else
            prev->next = curr;
        prev = curr;
        
        free(vheader);
        free(authbasic);
        free(index);
        
    }
    if(htusersCount > 0) {
        htpasswd_s = jsonVal(json, "htpasswd");
        www->htpasswd = parseHtpasswdData(htpasswd_s);
        free(htpasswd_s);
    }
    else
        www->htpasswd = NULL;
    
    www->vhost = head;
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
void mkdirtree(char * path) {
  int i = 0;
  char * p = path;
  char buff[PATH_MAX];
  memset(buff, '\0', PATH_MAX);

  while(*p) {
    buff[i] = *p;
    if(*p == '/')
      mkdir(buff, 0755);
    i++; p++;
  }
  mkdir(buff, 0755);
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
void purgeDir(char * name) {
	DIR * d;
	struct dirent * entry;
	struct stat st;
	const int pathlen = 256;
	char buff[pathlen];

	memset(buff, '\0', pathlen);
	strcpy(buff, name);

	d = opendir(name);
	stat(buff, &st);
	while((entry = readdir(d)) != NULL) {
		if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
			continue;
		strcat(buff, entry->d_name);
		stat(buff, &st);
		if(S_ISDIR(st.st_mode)) {
			strcat(buff, "/");
			purgeDir(buff);
		}
		else
			unlink(buff);

		memset(buff, '\0', pathlen);
		strcpy(buff, name);
	}
	closedir(d);
	rmdir(name);
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
int JSONsearchString(char * json, char * needle) {
    char * pos = json;
    int count = 0;
    
    while((pos = strstr(pos, needle)) != NULL) {
        count++;
        pos++;
    }
    return count;
}