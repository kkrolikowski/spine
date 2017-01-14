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
    char * str = NULL;			// string wynikowy
    htpasswdData * pos = NULL;		// aktualna pozycja na liscie
    char * prefix = "htpasswd:";	// prefix listy
    size_t len = getHtPasswdPackageSize(htpasswd);	// rozmiar obszaru pamieci

    // przygotowujemy pamiec, ktora przechowa string wynikowy
    str = (char *) malloc(len * sizeof(char));
    memset(str, '\0', len);

    // wypelniamy pamiec danymi
    strncpy(str, prefix, strlen(prefix) + 1);
    pos = htpasswd;
    while(pos != NULL) {
        strncat(str, pos->entry, strlen(pos->entry) + 1);
        strncat(str, "#", 2);
        pos = pos->next;
    }
    clearHtpasswdData(htpasswd);
    str[strlen(str) - 1] = ',';

    return str;
}
void clearHtpasswdData(htpasswdData * htpasswd) {
    htpasswdData * curr = htpasswd;
    htpasswdData * next = NULL;
    
    free(curr->entry);
    
    next = curr->next;
    if(next != NULL)
        clearHtpasswdData(next);
    free(curr);
}
char * apacheConfigPackage(httpdata www) {
	int vidx = 0;			// vhost index
	size_t packageSize = 0;         // rozmiar pakietu
        char * package = NULL;          // pakiet konfiguracyjny
	char * numstr = NULL;		// string z liczby
	char * entry = NULL;            // zawartosc vhosta
	char * metainfo = NULL;		// dane pomocnicze
	char * authbasic = NULL;	// flaga okreslajaca, czy jest wlaczone haslo na witrynie
        vhostData * vhpos = www.vhost; // wskaznik pomocniczny, ktory bedzie przemieszczal sie po wezle
        
        // naglowek pakietu danych
        char * package_header = "{scope:apache,";
        
        // wersja konfiguracji apacza
        char * k_config_ver = "config_ver:";
        char * s_config_ver = NULL;
        int config_ver = 0;

        // zaalokowania pamieci dla calego pakietu
        packageSize = getApachedataSize(www);
        package = (char *) malloc(packageSize * sizeof(char));
        memset(package, '\0', packageSize);
        
        // budujemy pakiet
        strncpy(package, package_header, strlen(package_header)); // definiujemy typ konfiguracji (apache)
        while(vhpos) {
            numstr = int2String(vidx);
            authbasic = int2String(vhpos->password_access);
            entry = mkString(
                            "vhost_", numstr, ":{",
                            "ServerName:",       vhpos->ServerName,         ",",
                            "ServerAlias:",      vhpos->ServerAlias,        ",",
                            "DocumentRoot:",     vhpos->DocumentRoot,       ",",
                            "ApacheOpts:",       vhpos->apacheOpts,         ",",
                            "VhostAccessOrder:", vhpos->vhost_access_order, ",",
                            "VhostAccessList:",  vhpos->vhost_access_list,  ",",
                            "htaccess:",         vhpos->htaccess,           ",",
                            "authbasic:",        authbasic,                 ",",
                            "htusers:",          vhpos->htusers,            ",",
                            "vhoststatus:",      vhpos->status,             ",",
                            "purgedir:",         vhpos->purgedir,           ",",
                            "user:",             vhpos->user,               "}",
                            ",", NULL);
            strncat(package, entry, strlen(entry) + 1);
            if(vhpos->next == NULL) {
                config_ver = vhpos->version;
                s_config_ver = int2String(config_ver);
            }
            
            // zwalniamy pamiec i przygotowujemy zmienne do kolejnej iteracji
            free(entry);
            free(numstr);
            free(authbasic);
            if(strcmp(vhpos->ServerName, "NaN"))
                vidx++;
            vhpos = vhpos->next;
        }

	// dodatkowe dane: liczba vhostow, ktore zostaly odczytane
	numstr = int2String(vidx);
	metainfo = mkString("vhost_num:", numstr, ",", NULL);
	strncat(package, metainfo, strlen(metainfo) + 1);
        strncat(package, k_config_ver, strlen(k_config_ver));
        strncat(package, s_config_ver, strlen(s_config_ver));
        strncat(package, ",", 1);

	// czyscimy niepotrzebne dane
	free(numstr);
	free(metainfo);
        free(s_config_ver);
        cleanVhostData(www.vhost);
        
	return package;
}
void createHtpasswdFile(char * os, htpasswdData * htpasswd) {
	char * htpasswd_path = NULL;
	FILE * htpasswd_file = NULL;
	htpasswdData * pos = htpasswd;

	if(!strcmp(os, "Ubuntu"))
		htpasswd_path = "/etc/apache2/auth/.htpasswd";
	else if(strstr(os, "Centos") != NULL)
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
void createHtgroupFile(char * path, vhostData * vhd) {
    int i;
    FILE * htgroup;
    vhostData * curr = vhd;

    if((htgroup = fopen(path, "w")) != NULL) {
        while(curr) {
            if(curr->password_access == 1)
                fprintf(htgroup, "%s: %s\n", curr->ServerName, curr->htusers);
            
            curr = curr->next;
        }
        fclose(htgroup);
    }
}
void createHtgroupConfig(char * os, vhostData * vhd, FILE * lf) {
    char * logmessage = NULL;
    char * htgroupDir = NULL;
    char * htgroupFilePath = NULL;

    if(!strcmp(os, "Ubuntu"))
        htgroupDir = "/etc/apache2/auth";
    else if(strstr(os, "Centos") != NULL)
        htgroupDir = "/etc/httpd/auth";
    htgroupFilePath = mkString(htgroupDir, "/.htgroup", NULL);

    if(mkdir(htgroupDir, 0755) < 0) {
        if(errno != EEXIST) {
            logmessage = mkString("[ERROR] (reciver) Blad tworzenia katalogu: ", htgroupDir, "\n", NULL);
            writeLog(lf, logmessage);
        }
        else
            createHtgroupFile(htgroupFilePath, vhd);
    }
    else
        createHtgroupFile(htgroupFilePath, vhd);

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
void apacheSetup(httpdata www, char * os, FILE * lf) {
    char * msg = NULL;
    int htusersCount = getHTusersCount(www.htpasswd);
    
    if(createVhostConfig(os, www.vhost, lf)) {
        createWebsiteDir(www.vhost);
        createHtaccess(www.vhost);
        createHtgroupConfig(os, www.vhost, lf);
        if(htusersCount > 0) {
            createHtpasswdFile(os, www.htpasswd);
        }
        else {
            clearAuthData(os);
        }
        msg = mkString("[INFO] (reciver) Konfiguracja apacza gotowa.", NULL);
        writeLog(lf, msg);
        reloadApache(os);
    }
    else {
        msg = mkString("[ERROR] (reciver) Wystapil problem podczas tworzenia plikow z konfiguracja.", NULL);
        writeLog(lf, msg);
    }
    if(removeVhost(os, www.vhost)) {
        msg = mkString("[INFO] (reciver) Konfiguracja apacza zostala wyczyszczona z niepotrzebnych witryn.", NULL);
        writeLog(lf, msg);
        reloadApache(os);
    }
    cleanVhostData(www.vhost);
}
void createHtaccess(vhostData * vhd) {
    FILE * htaccess;
    char * htaccessPath = NULL;
    vhostData * curr = vhd;
    
    while(curr) {
        htaccessPath = mkString(curr->DocumentRoot, "/.htaccess", NULL);
        if(strcmp(curr->htaccess, "NaN")) {
            if((htaccess = fopen(htaccessPath, "w")) != NULL)
                fprintf(htaccess, "%s\n", curr->htaccess);

            fclose(htaccess);
            free(htaccessPath);
        }
        curr = curr->next;
    }
}
void createWebsiteDir(vhostData * vhd) {
    int i;
    vhostData * curr = vhd;
    char * dirpath = NULL;
    size_t len = 0;
    
    while(curr) {
        len = strlen(curr->DocumentRoot) + 2;
        dirpath = (char *) malloc(len * sizeof(char));
        memset(dirpath, '\0', len);
        strncpy(dirpath, curr->DocumentRoot, strlen(curr->DocumentRoot));
        strncat(dirpath, "/", 2);
        if(access(dirpath, F_OK) < 0) {
            if(errno == ENOENT)
                mkdirtree(curr->DocumentRoot);
        }
        free(dirpath);
        curr = curr->next;
    }
}
int createVhostConfig(char * distro, vhostData * vhd, FILE * lf) {
    vhostData * pos = vhd;              // aktualna pozycja w pamieci
    char * configDir = NULL;            // sciezka do katalogu konfiguracyjnego apacza
    char * configDir2 = NULL;           // Ubuntu only. sciezka do katalogu sites-enabled
    char * apacheAuthDir = NULL;        // sciezka do katalogu z uzytkownikami i grupami apacza
    char * logsDir = NULL;              // sciezka do logow apacza
    char * path = NULL;                 // pelna sciezka do pliku konfiguracyjnego vhosta
    char * path2 = NULL;                // symlink do sites-enabled (Ubuntu only)
    char * logentry = NULL;             // komunikat do logu programu
    char * acl_entry = NULL;            // kompletna lista konfiguracji dostepu
    char * acl_order = NULL;            // kolejnosc przetwarzania listy dostepow
    FILE * vhost;                       // uchwyt pliku vhosta
    int counter = 0;                    // liczba stworzonych vhostow
    int total = getVhostsCount(vhd);    // calkowita liczba vhostow 
    int vhostExist = 0;                 // ustawiamy 1 jesli plik juz istnial
    int i;

    // ustawiamy odpowiednie sciezki w zaleznosci od systemu operacyjnego
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

    while(pos) {
        if(!strcmp(pos->status, "A")) {
            path = mkString(configDir, pos->ServerName, ".conf", NULL);
            path2 = mkString(configDir2, pos->ServerName, ".conf", NULL);
            if(!access(path, F_OK))
                vhostExist = 1;
            if((vhost = fopen(path, "w")) == NULL) {
                logentry = mkString("[ERROR] (reciver) Blad tworzenia pliku: ", path, NULL);
                writeLog(lf, logentry);
                pos = pos->next;
                continue;
            }
            counter++;
            acl_order = accessOrder(pos->vhost_access_order);
            acl_entry = acl(pos->vhost_access_list);
            fprintf(vhost, "<VirtualHost *:80>\n");
            fprintf(vhost, "\tServerName %s\n", pos->ServerName);
            if(strcmp(pos->ServerAlias, "NaN"))
                fprintf(vhost, "\tServerAlias %s\n", pos->ServerAlias);
            fprintf(vhost, "\tDocumentRoot \"%s\"\n\n", pos->DocumentRoot);
            fprintf(vhost, "\t<Directory %s>\n", pos->DocumentRoot);
            fprintf(vhost, "\t\tOptions %s\n", pos->apacheOpts);
            fprintf(vhost, "\t\tAllowOverride All\n");
            fprintf(vhost, "\t\tOrder %s\n", acl_order);
            fprintf(vhost, "%s\n", acl_entry);
            if(!strcmp(distro, "Ubuntu"))
                fprintf(vhost, "\t\tRequire all granted\n");
            fprintf(vhost, "\t</Directory>\n\n");
            if(pos->password_access) {
                fprintf(vhost, "\t<Location />\n");
                fprintf(vhost, "\t\tAuthType Basic\n");
                fprintf(vhost, "\t\tAuthName \"Restricted Area\"\n");
                fprintf(vhost, "\t\tAuthUserFile %s/.htpasswd\n", apacheAuthDir);
                fprintf(vhost, "\t\tAuthGroupFile %s/.htgroup\n", apacheAuthDir);
                fprintf(vhost, "\t\tRequire group %s\n", pos->ServerName);
                fprintf(vhost, "\t</Location>\n\n");
            }
            fprintf(vhost, "\tErrorLog %s/%s-error.log\n", logsDir, pos->ServerName);
            fprintf(vhost, "\tCustomLog %s/%s-access.log combined\n", logsDir, pos->ServerName);
            fprintf(vhost, "</VirtualHost>\n");
            fclose(vhost);
            free(acl_order);
            free(acl_entry);
            if(vhostExist)
                logentry = mkString("[INFO] (reciver) Konfiguracja dla ", pos->ServerName, " zostala zaktualizowana", NULL);
            else
                logentry = mkString("[INFO] (reciver) Stworzona zostala konfiguracja dla ", pos->ServerName, NULL);
            writeLog(lf, logentry);
            if(!strcmp(distro, "Ubuntu"))
                if(access(path2, F_OK) < 0) {
                    if(errno == ENOENT) {
                        if(symlink(path, path2)) {
                            if(errno == EEXIST)
                                logentry = mkString("[WARNING] (reciver) Vhost: ", pos->ServerName, " byl juz aktywny", NULL);
                            else
                                logentry = mkString("[ERROR] (reciver) Nie udalo sie aktywowac konfiguracji dla ", pos->ServerName, NULL);
                            writeLog(lf, logentry);
                        }
                    }
                }
            
            vhostExist = 0;
            free(path);
            free(path2);
        }
        else if(!strcmp(pos->status, "D"))
            counter++;
        pos = pos->next;
    }

    /* weryfikujemy rezultat zakladania plikow
     *  jesli nie udalo sie stworzyc zadnego: -1
     *  jesli udalo sie stworzyc wszystkie: 1
     *  jesli nie udalo sie stworzyc niektorych: 0
     */
    if(counter == 0)
        return -1;
    else if(counter == total)
        return 1;
    else
        return 0;
}
void clearAuthData(char * os) {
	char * htpasswd_path = NULL;
	char * htgroup_path = NULL;

	if(!strcmp(os, "Ubuntu")) {
		htpasswd_path = "/etc/apache2/auth/.htpasswd";
		htgroup_path = "/etc/apache2/auth/.htgroup";
	}
	else if(strstr(os, "Centos") != NULL) {
		htpasswd_path = "/etc/httpd/auth/.htpasswd";
		htgroup_path = "/etc/httpd/auth/.htgroup";
	}

	if(fileExist(htpasswd_path))
		unlink(htpasswd_path);
	if(fileExist(htgroup_path))
		unlink(htgroup_path);
}
int removeVhost(char * os, vhostData * vhd) {
    int count = 0;
    char * vhostConfig = NULL;
    char * vhostConfigSymlink = NULL;
    char * vhostDir = NULL;
    vhostData * curr = vhd;

    while(curr) {
        vhostDir = mkString("/var/www/", curr->ServerName, "/", NULL);
        if(!strcmp(os, "Ubuntu")) {
            vhostConfig = mkString("/etc/apache2/sites-available/", curr->ServerName, ".conf", NULL);
            vhostConfigSymlink = mkString("/etc/apache2/sites-enabled/", curr->ServerName, ".conf", NULL);
        }
        else if(strstr(os, "Centos") != NULL) {
            vhostConfig = mkString("/etc/httpd/conf.d/", curr->ServerName, ".conf", NULL);
        }
        if(!strcmp(curr->status, "D")) {
            if(vhostConfig != NULL) {
                unlink(vhostConfig);
                count++;
            }
            if(vhostConfigSymlink != NULL)
                unlink(vhostConfigSymlink);
        }
        if(!strcmp(curr->purgedir, "Y"))
            purgeDir(vhostDir);

        if(vhostConfig != NULL) free(vhostConfig);
        if(vhostConfigSymlink != NULL) free(vhostConfigSymlink);
        if(vhostDir != NULL) free(vhostDir);
        
        curr = curr->next;
    }
    return count;
}
int getApachedataSize(httpdata www) {
    int size = 0;
    int vhostPackageSize = 0;
    int htpasswdPackageSize = 0;
    
    if(www.vhost != NULL)
        vhostPackageSize = getVhostPackageSize(www.vhost);
    if(www.htpasswd != NULL)
        htpasswdPackageSize = getHtPasswdPackageSize(www.htpasswd);
        
    size = vhostPackageSize + htpasswdPackageSize;
    size += strlen("{scope:apache,}");
    size += strlen("vhost_num:,");
    size += strlen("htpasswd_count:,");
    
    return size;
}
int getVhostPackageSize(vhostData * vhd) {
    int size = 0;           // licznik bajtow
    int keysize = 0;        // rozmiar kluczy w pakiecie
    vhostData * pos = vhd;  // aktualna pozycja w pamieci
    char * tmp = NULL;      // tymczasowa zmienna do przechowania
                            // wartosci numerycznych w formie stringu
    int vhostCount = 0;     // zliczanie liczby vhostow

    // nazwy kluczy w pakiecie;
    const char * keys[] = { "DocumentRoot:,", "ServerAlias:,", "ServerName:,", "ApacheOpts:,",
                            "htaccess:,", "htusers:,", "purgedir:,", "vhoststatus:,", "user:,",
                            "VhostAccessOrder:,", "VhostAccessList:,", "config_ver:",
                            "vhost_:", "{},", NULL};
    const char ** key = keys;
    while(*key) {
        keysize += strlen(*key);
        key++;
    }
    while(pos) {
        // Dane tekstowe
        size += strlen(pos->DocumentRoot);
        size += strlen(pos->ServerAlias);
        size += strlen(pos->ServerName);
        size += strlen(pos->apacheOpts);
        size += strlen(pos->htaccess);
        size += strlen(pos->htusers);
        size += strlen(pos->purgedir);
        size += strlen(pos->status);
        size += strlen(pos->user);
        size += strlen(pos->vhost_access_list);
        size += strlen(pos->vhost_access_order);

        // Dane numeryczne;
        tmp = int2String(pos->password_access);
        size += strlen(tmp);
        free(tmp);
        
        tmp = int2String(vhostCount);
        size += strlen(tmp);
        free(tmp);
        
        if(pos->next == NULL) {
            tmp = int2String(pos->version);
            size += strlen(tmp);
            free(tmp);
        }
                
        vhostCount++;
        pos = pos->next;
    }
    size += keysize * vhostCount;
    
    return size;
}
int getHtPasswdPackageSize(htpasswdData * htp) {
    int size = 0;
    htpasswdData * pos = htp;
    int entryCount = 0;
    
    char * key = "htpasswd:,";
    while(pos) {
        size += strlen(pos->entry);
        entryCount++;
        pos = pos->next;
    }
    size += strlen(key) + entryCount;
    
    return size;
}
int getVhostsCount(vhostData *vh) {
    int sum  = 0;
    vhostData * pos = vh;
    
    while(pos) {
        sum++;
        pos = pos->next;
    }
    return sum;
}
int getHTusersCount(htpasswdData * htp) {
    int sum = 0;
    htpasswdData * pos = htp;
    
    while(pos) {
        // jesli entry nie jest NaN zwieksz licznik
        if(strcmp(pos->entry, "NaN"))
            sum++;
        pos = pos->next;
    }
    return sum;
}
void cleanVhostData(vhostData * vhd) {
    vhostData * curr = vhd;
    vhostData * next = NULL;
    
    free(curr->DocumentRoot);
    free(curr->ServerAlias);
    free(curr->ServerName);
    free(curr->apacheOpts);
    free(curr->htaccess);
    free(curr->htusers);
    free(curr->purgedir);
    free(curr->status);
    free(curr->user);
    free(curr->vhost_access_list);
    free(curr->vhost_access_order);
    
    next = curr->next;
    if(next != NULL)
        cleanVhostData(next);
    free(curr);
}