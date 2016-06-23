#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>
#include "apache.h"
#include "core.h"

char * readHtpasswdData(htpasswdData * htpasswd) {
	char * str = NULL;				// string wynikowy
	htpasswdData * pos = NULL;		// aktualna pozycja na liscie

	char * prefix = "htpasswd:";	// prefix listy
	size_t len = strlen(prefix);	// rozmiar obszaru pamieci

	// obliczamy ile bajtow zajmuja wszystkie elementy z listy
	pos = htpasswd;
	while(pos != NULL) {
		len += strlen(pos->entry);
		len++;
		pos = pos->next;
	}
	len += 2;
	// przygotowujemy pamiec, ktora przechowa string wynikowy
	str = (char *) malloc(len * sizeof(char));
	memset(str, '\0', len);

	// wypelniamy pamiec danymi
	strcpy(str, prefix);
	pos = htpasswd;
	while(pos != NULL) {
		strcat(str, pos->entry);
		strcat(str, "#");
		pos = pos->next;
	}
	clearHtpasswdData(htpasswd);
	str[strlen(str) - 1] = ',';

	return str;
}
void clearHtpasswdData(htpasswdData * htpasswd) {
	htpasswdData * curr = NULL;

	while(htpasswd != NULL) {
		curr = htpasswd;
		htpasswd = htpasswd->next;
		free(curr->entry);
		free(curr);
	}
}
void clearConfigData(hostconfig * cfd) {
	int i;

	for(i = 0; i < cfd->vhost_num; i++) {
		if(cfd->vhost[i].DocumentRoot != NULL) free(cfd->vhost[i].DocumentRoot);
		if(cfd->vhost[i].ServerAlias != NULL) free(cfd->vhost[i].ServerAlias);
		if(cfd->vhost[i].ServerName != NULL) free(cfd->vhost[i].ServerName);
		if(cfd->vhost[i].apacheOpts != NULL) free(cfd->vhost[i].apacheOpts);
		if(cfd->vhost[i].htaccess != NULL) free(cfd->vhost[i].htaccess);
		if(cfd->vhost[i].user != NULL) free(cfd->vhost[i].user);
		if(cfd->vhost[i].vhost_access_list != NULL) free(cfd->vhost[i].vhost_access_list);
		if(cfd->vhost[i].vhost_access_order != NULL) free(cfd->vhost[i].vhost_access_order);
		if(cfd->vhost[i].htusers != NULL) free(cfd->vhost[i].htusers);
	}
	if(cfd->datatype != NULL) free(cfd->datatype);
}
void initConfigData(hostconfig * cfd, long vhostnum) {
	int i = 0;

	for(i = 0; i < vhostnum; i++) {
		cfd->vhost[i].DocumentRoot = NULL;
		cfd->vhost[i].ServerAlias = NULL;
		cfd->vhost[i].ServerName = NULL;
		cfd->vhost[i].apacheOpts = NULL;
		cfd->vhost[i].htaccess = NULL;
		cfd->vhost[i].htusers = NULL;
		cfd->vhost[i].user = NULL;
		cfd->vhost[i].vhost_access_list = NULL;
		cfd->vhost[i].vhost_access_order = NULL;
	}
	cfd->htpasswd = NULL;
	cfd->datatype = NULL;
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
char * apacheConfigPackage(hostconfig data) {
	int vidx;					// vhost index
	size_t len = 0;				// liczba bajtow konfiguracji
	char * numstr = NULL;		// string z liczby
	char * vhostdata = NULL; 	// zawartosc vhosta
	char * metainfo = NULL;		// dane pomocnicze
	char * configstr = NULL;	// string przechowujacy odczytana konfiguracje
	char * authbasic = NULL;	// flaga okreslajaca, czy jest wlaczone haslo na witrynie

	// bufor na dane konfiguracyjne
	char buff[PACKAGE_SIZE];
	memset(buff, '\0', PACKAGE_SIZE);

	strcpy(buff, "{datatype:apache,");		// definiujemy typ konfiguracji (apache)

	// przetwarzamy dodane vhosty apacza
	for(vidx = 0; vidx < data.vhost_num; vidx++) {

		numstr = int2String(vidx);
		authbasic = int2String(data.vhost[vidx].password_access);
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
				",authbasic:", authbasic,
				",htusers:", data.vhost[vidx].htusers,
				",user:", data.vhost[vidx].user, "},", NULL);
		strcat(buff, vhostdata);

		free(vhostdata);
		free(numstr);
		free(authbasic);
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
	free(numstr);
	free(metainfo);

	return configstr;
}
void createHtpasswdFile(char * os, htpasswdData * htpasswd) {
	char * htpasswd_path = NULL;
	FILE * htpasswd_file = NULL;
	htpasswdData * pos = htpasswd;

	if(!strcmp(os, "Ubuntu"))
		htpasswd_path = "/etc/apache2/auth/.htpasswd";
	else if(!strcmp(os, "Centos"))
		htpasswd_path = "/etc/httpd/auth/.htpasswd";

	if((htpasswd_file = fopen(htpasswd_path, "w")) != NULL) {
		while(pos != NULL) {
			fprintf(htpasswd_file, "%s\n", pos->entry);
			pos = pos->next;
		}
		fclose(htpasswd_file);
	}
	clearHtpasswdData(htpasswd);
}
htpasswdData * parseHtpasswdData(char * stream) {
	htpasswdData * head = NULL;
	htpasswdData * curr = NULL;
	htpasswdData * prev = NULL;
	char buff[256];
	size_t buff_len = 0;
	int i = 0;

	memset(buff, '\0', 256);

	while(*stream) {
		if(*stream == '#') {
			buff_len = strlen(buff) + 1;
			curr = (htpasswdData *) malloc(sizeof(htpasswdData));
			curr->entry = (char *) malloc(buff_len * sizeof(char));
			memset(curr->entry, '\0', buff_len);
			strncpy(curr->entry, buff, buff_len);
			curr->next = NULL;

			if(head == NULL)
				head = curr;
			else
				prev->next = curr;
			prev = curr;

			memset(buff, '\0', 256);
			i = 0;
			stream++;
		}
		buff[i] = *stream;
		stream++; i++;
	}
	buff_len = strlen(buff) + 1;
	curr = (htpasswdData *) malloc(sizeof(htpasswdData));
	curr->entry = (char *) malloc(buff_len * sizeof(char));
	memset(curr->entry, '\0', buff_len);
	strncpy(curr->entry, buff, buff_len);
	curr->next = NULL;

	if(head == NULL)
		head = curr;
	else
		prev->next = curr;
	prev = curr;

	return head;
}
void createHtgroupFile(char * path, wwwdata vhosts[], int n) {
	int i;
	FILE * htgroup;

	if((htgroup = fopen(path, "w")) != NULL) {
		for(i = 0; i < n; i++) {
			if(vhosts[i].password_access == 1)
				fprintf(htgroup, "%s: %s\n", vhosts[i].ServerName, vhosts[i].htusers);
		}
		fclose(htgroup);
	}
}
void createHtgroupConfig(char * os, wwwdata vhosts[], int n, FILE * lf) {
	char * logmessage = NULL;
	char * htgroupDir = NULL;
	char * htgroupFilePath = NULL;

	if(!strcmp(os, "Ubuntu"))
		htgroupDir = "/etc/apache2/auth";
	else if(!strcmp(os, "Centos"))
		htgroupDir = "/etc/httpd/auth";
	htgroupFilePath = mkString(htgroupDir, "/.htgroup", NULL);

	if(mkdir(htgroupDir, 0755) < 0) {
		if(errno != EEXIST) {
			logmessage = mkString("[ERROR] (reciver) Blad tworzenia katalogu: ", htgroupDir, "\n", NULL);
			writeLog(lf, logmessage);
		}
		else
			createHtgroupFile(htgroupFilePath, vhosts, n);
	}
	else
		createHtgroupFile(htgroupFilePath, vhosts, n);

	free(htgroupFilePath);
}
char * accessOrder(char * str) {
	char * allow = "allow,deny";
	char * deny = "deny,allow";
	char * order = NULL;
	size_t len = 0;

	if(!strcmp(str, "10")) {
		len = strlen(allow) + 1;
		order = (char *) malloc(len * sizeof(char));
		memset(order, '\0', len);
		strcpy(order, allow);
	}
	else {
		len = strlen(deny) + 1;
		order = (char *) malloc(len * sizeof(char));
		memset(order, '\0', len);
		strcpy(order, deny);
	}

	return order;
}
char * acl(char * str) {
	char buff[256];
	char chunk[64];
	char * accesslist = NULL;
	char * acl_entry = NULL;
	size_t accesslist_len = 0;
	int i = 0;

	memset(buff, '\0', 256);
	memset(chunk, '\0', 64);
	while(*str) {
		if(*str == '#') {
			chunk[i] = '\0';
			acl_entry = apache_accesslist_entry(chunk);
			strcat(buff, acl_entry);
			strcat(buff, "\n");
			free(acl_entry);
			i = 0;
			str++;
		}
		else {
			chunk[i] = *str;
			str++; i++;
		}
	}
	chunk[i] = '\0';
	acl_entry = apache_accesslist_entry(chunk);
	strcat(buff, acl_entry);

	accesslist_len = strlen(buff) + 1;
	accesslist = (char *) malloc(accesslist_len * sizeof(char));
	memset(accesslist, '\0', accesslist_len);
	strncpy(accesslist, buff, accesslist_len);
	free(acl_entry);

	return accesslist;
}
char * apache_accesslist_entry(char * str) {
	char * allow = "\t\tAllow from ";		// poczatek linijki (allow)
	char * deny = "\t\tDeny from ";			// poczatek linijki (deny)
	char * rule = NULL;						// przyjmie wartosc allow lub deny
	char * rule_entry = NULL;				// adres stringa z pojedyncza regula
	char * access_val = NULL;				// wartosc 0 lub 1 - allow lub deny
	char * str_begin = str;					// poczatek stringa przekazanego do funkcji
	char * access_from = NULL;				// string przechowujacy wartosc from
	char * access_from_begin = NULL;		// poczatek stringa przechowujacego from
	long from_len = 0L;						// ilosc pamieci potrzebnej do przechowania wartosci from
	size_t rule_len = 0;					// ilosc pamieci potrzebnej do przechowania calego entry

	access_val = strchr(str, ':') + 1;							// odszukujemy wartosc allow lub deny
	from_len = access_val - str_begin;							// okreslamy ilosc pamieci potrzebnej na przechowanie wartosci from
	access_from = (char *) malloc(from_len * sizeof(char));		// alokujemy pamiec do przechowania wartosci from
	access_from_begin = access_from;							// zapisujemy adres poczatkowy
	memset(access_from, '\0', from_len);

	// odczytujemy from ze stringa
	while(*str_begin != ':') {
		*access_from = *str_begin;
		access_from++; str_begin++;
	}
	*access_from = '\0';

	// okreslamy czy ma byc Deny from czy Allow from
	if(strcmp(access_val, "0"))
		rule = allow;
	else
		rule = deny;

	// rezerwujemy pamiec i wypelniamy ja zebranymi danymi
	rule_len = strlen(rule) + strlen(access_from_begin) + 1;
	rule_entry = (char *) malloc(rule_len * sizeof(char));
	memset(rule_entry, '\0', rule_len);
	strcpy(rule_entry, rule);
	strcat(rule_entry, access_from_begin);

	// zwalniamy niepotrzebna pamiec
	free(access_from_begin);

	return rule_entry;
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
void apacheSetup(hostconfig cfg, char * os, FILE * lf) {
	char * msg = NULL;
	if(createVhostConfig(os, cfg.vhost, cfg.vhost_num, lf)) {
		createWebsiteDir(cfg.vhost, cfg.vhost_num);
		createHtaccess(cfg.vhost, cfg.vhost_num);
		createHtgroupConfig(os, cfg.vhost, cfg.vhost_num, lf);
		if(cfg.htusers_count > 0) {
			createHtpasswdFile(os, cfg.htpasswd);
		}
		msg = mkString("[INFO] (reciver) Konfiguracja apacza gotowa.", NULL);
		writeLog(lf, msg);
		reloadApache(os);
	}
	else {
		msg = mkString("[ERROR] (reciver) Wystapil problem podczas tworzenia plikow z konfiguracja.", NULL);
		writeLog(lf, msg);
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
int createVhostConfig(char * distro, wwwdata vhosts[], int n, FILE * lf) {
	char * configDir = NULL;			// sciezka do katalogu konfiguracyjnego apacza
	char * configDir2 = NULL;			// Ubuntu only. sciezka do katalogu sites-enabled
	char * apacheAuthDir = NULL;		// sciezka do katalogu z uzytkownikami i grupami apacza
	char * logsDir = NULL;				// sciezka do logow apacza
	char * path = NULL;					// pelna sciezka do pliku konfiguracyjnego vhosta
	char * path2 = NULL;				// symlink do sites-enabled (Ubuntu only)
	char * logentry = NULL;				// komunikat do logu programu
	char * acl_entry = NULL;			// kompletna lista konfiguracji dostepu
	char * acl_order = NULL;			// kolejnosc przetwarzania listy dostepow
	FILE * vhost;						// uchwyt pliku vhosta
	int counter = 0;					// liczba stworzonych vhostow
	int vhostExist = 0;					// ustawiamy 1 jesli plik juz istnial
	int i;

	if(!strcmp(distro, "Centos6") || !strcmp(distro, "Centos7")) {
		apacheAuthDir = "/etc/httpd/auth";
		configDir = "/etc/httpd/conf.d/";
		logsDir = "/var/log/httpd";
	}
	else if(!strcmp(distro, "Ubuntu")) {
		apacheAuthDir = "/etc/apache2/auth";
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
		acl_order = accessOrder(vhosts[i].vhost_access_order);
		acl_entry = acl(vhosts[i].vhost_access_list);
		fprintf(vhost, "<VirtualHost *:80>\n");
		fprintf(vhost, "\tServerName %s\n", vhosts[i].ServerName);
		if(strcmp(vhosts[i].ServerAlias, "NaN"))
			fprintf(vhost, "\tServerAlias %s\n", vhosts[i].ServerAlias);
		fprintf(vhost, "\tDocumentRoot \"%s\"\n\n", vhosts[i].DocumentRoot);
		fprintf(vhost, "\t<Directory %s>\n", vhosts[i].DocumentRoot);
		fprintf(vhost, "\t\tOptions %s\n", vhosts[i].apacheOpts);
		fprintf(vhost, "\t\tAllowOverride All\n");
		fprintf(vhost, "\t\tOrder %s\n", acl_order);
		fprintf(vhost, "%s\n", acl_entry);
		if(!strcmp(distro, "Ubuntu"))
			fprintf(vhost, "\t\tRequire all granted\n");
		fprintf(vhost, "\t</Directory>\n\n");
		if(vhosts[i].password_access) {
			fprintf(vhost, "\t<Location />\n");
			fprintf(vhost, "\t\tAuthType Basic\n");
			fprintf(vhost, "\t\tAuthName \"Restricted Area\"\n");
			fprintf(vhost, "\t\tAuthUserFile %s/.htpasswd\n", apacheAuthDir);
			fprintf(vhost, "\t\tAuthGroupFile %s/.htgroup\n", apacheAuthDir);
			fprintf(vhost, "\t\tRequire group %s\n", vhosts[i].ServerName);
			fprintf(vhost, "\t</Location>\n\n");
		}
		fprintf(vhost, "\tErrorLog %s/%s-error.log\n", logsDir, vhosts[i].ServerName);
		fprintf(vhost, "\tCustomLog %s/%s-access.log combined\n", logsDir, vhosts[i].ServerName);
		fprintf(vhost, "</VirtualHost>\n");
		fclose(vhost);
		free(acl_order);
		free(acl_entry);
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
