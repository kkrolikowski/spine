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
#include "core.h"
#include "sysconfigdata.h"
#include "network.h"

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
	if((sys->config_version = readLocalConfigVersion()) >= 0)
		status = 1;
	if((sys->os = linuxDistro()) != NULL)
		status = 1;
	if((sys->extip = getExternalIP()) != NULL)
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
	sys->os = NULL;
	sys->ip = NULL;
	sys->extip = NULL;
	sys->config_version = 0;
}
void ClearSystemInformation(systeminfo * sys) {
	sys->uptime = 0L;
	sys->hdd_free = 0L;
	sys->hdd_total = 0L;
	sys->ram_free = 0L;
	sys->ram_total = 0L;
	sys->config_version = 0;
	free(sys->extip);
	free(sys->net_hwaddr);
	free(sys->hostname);
	free(sys->os);
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
hostconfig ParseConfigData(char * json) {
	int i;
	hostconfig conf;
	char * config_pos = NULL;
	char * vheader = NULL;
	char * index = NULL;
	char * authbasic = NULL;
	char * confver_s = NULL;
	char * vhostnum_s = NULL;
	char * htusers_count_s = NULL;
	char * htpasswd_s = NULL;

	vhostnum_s = jsonVal(json, "vhost_num");
	conf.vhost_num = atoi(vhostnum_s);

	htusers_count_s = jsonVal(json, "htpasswd_count");
	conf.htusers_count = atoi(htusers_count_s);

	initConfigData(&conf, conf.vhost_num);

	conf.datatype = jsonVal(json, "datatype");
	confver_s = jsonVal(json, "config_ver");
	conf.confVer = atoi(confver_s);

	for(i = 0; i < conf.vhost_num; i++) {
		index = int2String(i);
		vheader = mkString("vhost_", index, NULL);
		config_pos = strstr(json, vheader);
		conf.vhost[i].ServerName = jsonVal(config_pos, "ServerName");
		conf.vhost[i].ServerAlias = jsonVal(config_pos, "ServerAlias");
		conf.vhost[i].DocumentRoot = jsonVal(config_pos, "DocumentRoot");
		conf.vhost[i].apacheOpts = jsonVal(config_pos, "ApacheOpts");
		conf.vhost[i].vhost_access_order = jsonVal(config_pos, "VhostAccessOrder");
		conf.vhost[i].vhost_access_list = jsonVal(config_pos, "VhostAccessList");
		conf.vhost[i].htaccess = jsonVal(config_pos, "htaccess");
		authbasic = jsonVal(config_pos, "authbasic");
		conf.vhost[i].password_access = atoi(authbasic);
		conf.vhost[i].user = jsonVal(config_pos, "user");
		conf.vhost[i].htusers = jsonVal(config_pos, "htusers");
		free(authbasic);
	}
	if(conf.htusers_count > 0) {
		htpasswd_s = jsonVal(json, "htpasswd");
		conf.htpasswd = parseHtpasswdData(htpasswd_s);
		free(htpasswd_s);
	}
	free(confver_s);
	free(vhostnum_s);
	free(htusers_count_s);
	free(vheader);
	free(index);

	return conf;
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
