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
#include <wait.h>
#include <errno.h>
#include "core.h"
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
	if((sys->config_version = readLocalConfigVersion()) >= 0)
		status = 1;
	if((sys->os = linuxDistro()) != NULL)
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
	sys->config_version = 0;
}
void ClearSystemInformation(systeminfo * sys) {
	sys->uptime = 0L;
	sys->hdd_free = 0L;
	sys->hdd_total = 0L;
	sys->ram_free = 0L;
	sys->ram_total = 0L;
	sys->config_version = 0;
	free(sys->net_hwaddr);
	free(sys->hostname);
	free(sys->os);
}
int readLocalConfigVersion(void) {
	int ver = 0;
	FILE * vf;

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
	conf.datatype = jsonVal(json, "datatype");
	conf.confVer = atoi(jsonVal(json, "config_ver"));
	conf.vhost_num = atoi(jsonVal(json, "vhost_num"));

	for(i = 0; i < conf.vhost_num; i++) {
		index = int2String(i);
		vheader = mkString("vhost_", index, NULL);
		config_pos = strstr(json, vheader);
		conf.vhost[i].ServerName = jsonVal(config_pos, "ServerName");
		conf.vhost[i].ServerAlias = jsonVal(config_pos, "ServerAlias");
		conf.vhost[i].DocumentRoot = jsonVal(config_pos, "DocumentRoot");
		conf.vhost[i].htaccess = jsonVal(config_pos, "htaccess");
		conf.vhost[i].user = jsonVal(config_pos, "user");
	}

	//free(config_pos);
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
int createVhostConfig(char * distro, wwwdata vhosts[], int n, FILE * lf) {
	char * configDir = NULL;			// sciezka do katalogu konfiguracyjnego apacza
	char * configDir2 = NULL;			// Ubuntu only. sciezka do katalogu sites-enabled
	char * logsDir = NULL;				// sciezka do logow apacza
	char * path = NULL;					// pelna sciezka do pliku konfiguracyjnego vhosta
	char * path2 = NULL;				// symlink do sites-enabled (Ubuntu only)
	char * logentry = NULL;				// komunikat do logu programu
	FILE * vhost;						// uchwyt pliku vhosta
	int counter = 0;					// liczba stworzonych vhostow
	int vhostExist = 0;					// ustawiamy 1 jesli plik juz istnial
	int i;

	if(!strcmp(distro, "Centos6") || !strcmp(distro, "Centos7")) {
		configDir = "/etc/httpd/conf.d/";
		logsDir = "/var/log/httpd";
	}
	else if(!strcmp(distro, "Ubuntu")) {
		configDir = "/etc/apache2/sites-available/";
		configDir2 = "/etc/apache2/sites-enabled/";
		logsDir = "/var/log/apache2";
	}

	for(i = 0; i < n; i++) {
		path = mkString(configDir, vhosts[i].ServerName, ".conf", NULL);
		path2 = mkString(configDir2, vhosts[i].ServerName, ".conf", NULL);
		if(!access(path, F_OK))
			vhostExist = 1;
		if((vhost = fopen(path, "w")) == NULL) {
			logentry = mkString("[ERROR] (reciver) Blad tworzenia pliku: ", path, NULL);
			writeLog(lf, logentry);
			continue;
		}
		counter++;
		fprintf(vhost, "<VirtualHost *:80>\n");
		fprintf(vhost, "\tServerName %s\n", vhosts[i].ServerName);
		if(strcmp(vhosts[i].ServerAlias, "NaN"))
			fprintf(vhost, "\tServerAlias %s\n", vhosts[i].ServerAlias);
		fprintf(vhost, "\tDocumentRoot \"%s\"\n\n", vhosts[i].DocumentRoot);
		fprintf(vhost, "\t<Directory %s>\n", vhosts[i].DocumentRoot);
		fprintf(vhost, "\t\tOptions Indexes FollowSymLinks MultiViews\n");
		fprintf(vhost, "\t\tAllowOverride All\n");
		fprintf(vhost, "\t\tOrder deny,allow\n");
		fprintf(vhost, "\t\tAllow from all\n");
		if(!strcmp(distro, "Ubuntu"))
			fprintf(vhost, "\t\tRequire all granted\n");
		fprintf(vhost, "\t</Directory>\n\n");
		fprintf(vhost, "\tErrorLog %s/%s-error.log\n", logsDir, vhosts[i].ServerName);
		fprintf(vhost, "\tCustomLog %s/%s-access.log combined\n", logsDir, vhosts[i].ServerName);
		fprintf(vhost, "</VirtualHost>\n");
		fclose(vhost);
		if(vhostExist)
			logentry = mkString("[INFO] (reciver) Konfiguracja dla ", vhosts[i].ServerName, " zostala zaktualizowana", NULL);
		else
			logentry = mkString("[INFO] (reciver) Stworzona zostala konfiguracja dla ", vhosts[i].ServerName, NULL);
		writeLog(lf, logentry);
		if(!strcmp(distro, "Ubuntu"))
			if(access(path2, F_OK) < 0) {
				if(errno == ENOENT) {
					if(symlink(path, path2)) {
						if(errno == EEXIST)
							logentry = mkString("[WARNING] (reciver) Vhost: ", vhosts[i].ServerName, " byl juz aktywny", NULL);
						else
							logentry = mkString("[ERROR] (reciver) Nie udalo sie aktywowac konfiguracji dla ", vhosts[i].ServerName, NULL);
						writeLog(lf, logentry);
					}
				}
			}

		vhostExist = 0;
		free(path);
		free(path2);
	}

	/* weryfikujemy rezultat zakladania plikow
	 *  jesli nie udalo sie stworzyc zadnego: -1
	 *  jesli udalo sie stworzyc wszystkie: 1
	 *  jesli nie udalo sie stworzyc niektorych: 0
	 */
	if(counter == 0)
		return -1;
	else if(counter == n)
		return 1;
	else
		return 0;
}
void createWebsiteDir(wwwdata vhosts[], int n) {
	int i;
	char * dirpath = NULL;
	size_t len = 0;

	for(i = 0; i < n; i++) {
		len = strlen(vhosts[i].DocumentRoot) + 2;
		dirpath = (char *) malloc(len * sizeof(char));
		memset(dirpath, '\0', len);
		strcpy(dirpath, vhosts[i].DocumentRoot);
		strcat(dirpath, "/");
		if(access(dirpath, F_OK) < 0) {
			if(errno == ENOENT)
				mkdirtree(vhosts[i].DocumentRoot);
		}
		free(dirpath);
	}
}
void createHtaccess(wwwdata vhosts[], int n) {
	FILE * htaccess;
	char * htaccessPath = NULL;
	int i;

	for(i = 0; i < n; i++) {
		htaccessPath = mkString(vhosts[i].DocumentRoot, "/.htaccess", NULL);
		if(strcmp(vhosts[i].htaccess, "NaN")) {
			if((htaccess = fopen(htaccessPath, "w")) != NULL)
				fprintf(htaccess, "%s\n", vhosts[i].htaccess);

			fclose(htaccess);
			free(htaccessPath);
		}
	}
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
void apacheSetup(hostconfig cfg, char * os, FILE * lf) {
	char * msg = NULL;
	if(createVhostConfig(os, cfg.vhost, cfg.vhost_num, lf)) {
		createWebsiteDir(cfg.vhost, cfg.vhost_num);
		createHtaccess(cfg.vhost, cfg.vhost_num);
		msg = mkString("[INFO] (reciver) Konfiguracja apacza gotowa.", NULL);
		writeLog(lf, msg);
		reloadApache(os);
	}
	else {
		msg = mkString("[ERROR] (reciver) Wystapil problem podczas tworzenia plikow z konfiguracja.", NULL);
		writeLog(lf, msg);
	}
}
void reloadApache(char * os) {
	pid_t pid;

	pid = fork();
	if(pid == 0) {
		if(!strcmp(os, "Ubuntu"))
			execl("/usr/sbin/apache2ctl", "apache2ctl", "graceful", NULL);
		else if(!strcmp(os, "Centos6"))
			execl("/usr/sbin/apachectl", "apachectl", "graceful", NULL);
		else if(!strcmp(os, "Centos7"))
			execlp("/bin/sh", "/bin/sh", "/usr/sbin/service", "httpd", "reload", NULL);
	}
	else if(pid > 0)
		wait(NULL);
}
