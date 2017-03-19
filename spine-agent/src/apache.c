#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>
#include "apache.h"
#include "core.h"

int htusersDataSize(htpasswdData * htpass) {
    int sum = 0;
    int userCount = 0;
    char * tmp = NULL;
    htpasswdData * curr = htpass;
    char * header = "{scope:htusers,}";
    char * keys[] = { "login:,", "password:,", "dbid:,", 
                      "status:,", "config_version:,", NULL 
                    };
    char ** key = keys;
    int keySize = 0; 
    
    while(curr) {
        sum += strlen(curr->login);
        sum += strlen(curr->pass);
        
        tmp = int2String(curr->dbid);
        sum += strlen(tmp);
        free(tmp);
        sum += 1;
        if(curr->next == NULL) {
            tmp = int2String(curr->version);
            sum += strlen(tmp);
            free(tmp);
        }
        userCount++;
        curr = curr->next;
    }
    while(*key++)
        keySize += strlen(*key);
    keySize *= userCount;
    
    sum += keySize + strlen(header);
    
    return sum;
}
char * htpasswdConfigPackage(htpasswdData htpass) {
    int vidx = 0;			// vhost index
    size_t packageSize = 0;             // package size
    char * package          = NULL;     // output package
    char * numstr           = NULL;	// htuser index as a string
    char * entry            = NULL;     // particular htuser definition
    char * s_dbid           = NULL;     // DB ID in a form of string
    char status[2];                     // status flags can be: NUDA
    htpasswdData * curr     = htpass;   // node traversing pointer

    // naglowek pakietu danych
    char * package_header = "{scope:htusers,";

    // wersja konfiguracji kont htpasswd
    char * k_config_ver = "config_ver:";
    char * s_config_ver = NULL;

    // zaalokowania pamieci dla calego pakietu
    packageSize = htusersDataSize(htpass) + 1;
    package = (char *) malloc(packageSize * sizeof(char));
    memset(package, '\0', packageSize);
  
    // budujemy pakiet
    strncpy(package, package_header, strlen(package_header));
    while(curr) {
        numstr = int2String(vidx);
        s_dbid = int2String(curr->dbid);
        memset(status, '\0', 2);
        status[0] = curr->status;
        
        entry = mkString(
                        "user_",            numstr,        ":{",
                        "dbid:",            s_dbid,         ",",
                        "login:",           curr->login,    ",",
                        "password:",        curr->pass,     ",",
                        "status:",          status,         ",",
                        ",", NULL);
        
        strncat(package, entry, strlen(entry) + 1);
        if(curr->next == NULL)
            s_config_ver = int2String(curr->version);

        // zwalniamy pamiec i przygotowujemy zmienne do kolejnej iteracji
        free(s_dbid);
        free(entry);
        free(numstr);
        curr = curr->next;
    }

    // dodatkowe dane: liczba vhostow, ktore zostaly odczytane
    strncat(package, k_config_ver, strlen(k_config_ver));
    strncat(package, s_config_ver, strlen(s_config_ver));
    strncat(package, "},", 2);

    // czyscimy niepotrzebne dane
    free(s_config_ver);
    clearHtpasswdData(htpass);

    return package;
}
void clearHtpasswdData(htpasswdData * htpasswd) {
    htpasswdData * curr = htpasswd;
    htpasswdData * next = NULL;
    
    free(curr->login);
    free(curr->pass);
    
    next = curr->next;
    if(next != NULL)
        clearHtpasswdData(next);
    free(curr);
}
char * apacheConfigPackage(vhostData www) {
    int vidx = 0;			// vhost index
    size_t packageSize = 0;             // package size
    char * package          = NULL;     // output package
    char * numstr           = NULL;	// vhost index as a string
    char * entry            = NULL;     // particular vhost definition
    char * authbasic        = NULL;	// authbasic flag
    char * s_dbid           = NULL;     // DB ID in a form of string
    vhostData * vhpos       = www;      // node traversing pointer

    // naglowek pakietu danych
    char * package_header = "{scope:apache,";

    // wersja konfiguracji apacza
    char * k_config_ver = "config_ver:";
    char * s_config_ver = NULL;

    // zaalokowania pamieci dla calego pakietu
    packageSize = getVhostPackageSize(www) + 1;
    package = (char *) malloc(packageSize * sizeof(char));
    memset(package, '\0', packageSize);

    // budujemy pakiet
    strncpy(package, package_header, strlen(package_header)); // definiujemy typ konfiguracji (apache)
    while(vhpos) {
        numstr = int2String(vidx);
        authbasic = int2String(vhpos->password_access);
        s_dbid = int2String(vhpos->dbid);
        entry = mkString(
                        "vhost_", numstr, ":{",
                        "dbid:",             s_dbid,                    ",",
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
        if(vhpos->next == NULL)
            s_config_ver = int2String(vhpos->version);

        // zwalniamy pamiec i przygotowujemy zmienne do kolejnej iteracji
        free(s_dbid);
        free(entry);
        free(numstr);
        free(authbasic);
        if(strcmp(vhpos->ServerName, "NaN"))
            vidx++;
        vhpos = vhpos->next;
    }

    // dodatkowe dane: liczba vhostow, ktore zostaly odczytane
    strncat(package, k_config_ver, strlen(k_config_ver));
    strncat(package, s_config_ver, strlen(s_config_ver));
    strncat(package, "},", 2);

    // czyscimy niepotrzebne dane
    free(s_config_ver);
    cleanVhostData(www);

    return package;
}
void createHtpasswdFile(char * htpasswdFilePath, htpasswdData * htpasswd) {
	FILE * htpasswd_file = NULL;
	htpasswdData * pos = htpasswd;

	if((htpasswd_file = fopen(htpasswdFilePath, "w")) != NULL) {
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
    FILE * htgroup;

    if((htgroup = fopen(path, "w")) != NULL) {
        fprintf(htgroup, "%s: %s\n", vhd->ServerName, vhd->htusers);
        fclose(htgroup);
    }
}
void apacheAuthConfig(char * os, vhostData * vhd, htpasswdData * authData, FILE * lf) {
    char * lmsg = NULL;
    char * authDir = NULL;
    char * htgroupFilePath = NULL;
    char * htpasswdFilePath = NULL;

    if(!strcmp(os, "Ubuntu"))
        authDir = "/etc/apache2/auth";
    else if(strstr(os, "Centos") != NULL)
        authDir = "/etc/httpd/auth";
    
    htgroupFilePath = mkString(authDir, "/.htgroup", NULL);
    htpasswdFilePath = mkString(authDir, "/.htpasswd", NULL);
    
    if(mkdir(authDir, 0755) < 0) {
        if(errno == EEXIST) {
            createHtgroupFile(htgroupFilePath, vhd);
            createHtpasswdFile(htpasswdFilePath, authData);
        }
        else {
            lmsg = mkString("[ERROR] (reciver) Blad tworzenia katalogu: ", authDir, "\n", NULL);
            writeLog(lf, lmsg);
        }
    }
    else {
        createHtgroupFile(htgroupFilePath, vhd);
        createHtpasswdFile(htpasswdFilePath, authData);
    }
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
void createHtaccess(char * htaccessPath, char * hta_content) {
    FILE * htaccess = NULL;
    
    if((htaccess = fopen(htaccessPath, "w")) != NULL) {
        fprintf(htaccess, "%s\n", hta_content);
        fclose(htaccess);
    }
}
void createWebsiteDir(char * websiteDir) {
    char * dirpath = NULL;
    size_t len = 0;
    
    len = strlen(websiteDir) + 2;
    dirpath = (char *) malloc(len * sizeof(char));
    memset(dirpath, '\0', len);
    
    strncpy(dirpath, websiteDir, strlen(websiteDir));
    strncat(dirpath, "/", 2);
    if(access(dirpath, F_OK) < 0) {
        if(errno == ENOENT)
            mkdirtree(websiteDir);
    }
    free(dirpath);
}
int createVhostConfig(char * distro, vhostData * vhd, FILE * lf) {
    char * configDir = NULL;            // sciezka do katalogu konfiguracyjnego apacza
    char * configDir2 = NULL;           // Ubuntu only. sciezka do katalogu sites-enabled
    char * apacheAuthDir = NULL;        // sciezka do katalogu z uzytkownikami i grupami apacza
    char * logsDir = NULL;              // sciezka do logow apacza
    char * path = NULL;                 // pelna sciezka do pliku konfiguracyjnego vhosta
    char * path2 = NULL;                // symlink do sites-enabled (Ubuntu only)
    char * logentry = NULL;             // komunikat do logu programu
    char * acl_entry = NULL;            // kompletna lista konfiguracji dostepu
    char * acl_order = NULL;            // kolejnosc przetwarzania listy dostepow
    FILE * vhost = NULL;                // uchwyt pliku vhosta
    int vhostExist = 0;                 // ustawiamy 1 jesli plik juz istnial

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

    path = mkString(configDir, vhd->ServerName, ".conf", NULL);
    path2 = mkString(configDir2, vhd->ServerName, ".conf", NULL);
    if(!access(path, F_OK))
        vhostExist = 1;
    if((vhost = fopen(path, "w")) == NULL) {
        logentry = mkString("[ERROR] (reciver) Blad tworzenia pliku: ", path, NULL);
        writeLog(lf, logentry);
        free(path);
        free(path2);
        return 0;
    }
    acl_order = accessOrder(vhd->vhost_access_order);
    acl_entry = acl(vhd->vhost_access_list);
    fprintf(vhost, "<VirtualHost *:80>\n");
    fprintf(vhost, "\tServerName %s\n", vhd->ServerName);
    if(strcmp(vhd->ServerAlias, "NaN"))
        fprintf(vhost, "\tServerAlias %s\n", vhd->ServerAlias);
    fprintf(vhost, "\tDocumentRoot \"%s\"\n\n", vhd->DocumentRoot);
    fprintf(vhost, "\t<Directory %s>\n", vhd->DocumentRoot);
    fprintf(vhost, "\t\tOptions %s\n", vhd->apacheOpts);
    fprintf(vhost, "\t\tAllowOverride All\n");
    fprintf(vhost, "\t\tOrder %s\n", acl_order);
    fprintf(vhost, "%s\n", acl_entry);
    if(!strcmp(distro, "Ubuntu"))
        fprintf(vhost, "\t\tRequire all granted\n");
    fprintf(vhost, "\t</Directory>\n\n");
    if(vhd->password_access) {
        fprintf(vhost, "\t<Location />\n");
        fprintf(vhost, "\t\tAuthType Basic\n");
        fprintf(vhost, "\t\tAuthName \"Restricted Area\"\n");
        fprintf(vhost, "\t\tAuthUserFile %s/.htpasswd\n", apacheAuthDir);
        fprintf(vhost, "\t\tAuthGroupFile %s/.htgroup\n", apacheAuthDir);
        fprintf(vhost, "\t\tRequire group %s\n", vhd->ServerName);
        fprintf(vhost, "\t</Location>\n\n");
    }
    fprintf(vhost, "\tErrorLog %s/%s-error.log\n", logsDir, vhd->ServerName);
    fprintf(vhost, "\tCustomLog %s/%s-access.log combined\n", logsDir, vhd->ServerName);
    fprintf(vhost, "</VirtualHost>\n");
    fclose(vhost);
    free(acl_order);
    free(acl_entry);
    if(vhostExist)
        logentry = mkString("[INFO] (reciver) Konfiguracja dla ", vhd->ServerName, " zostala zaktualizowana", NULL);
    else
        logentry = mkString("[INFO] (reciver) Stworzona zostala konfiguracja dla ", vhd->ServerName, NULL);
    writeLog(lf, logentry);
    if(!strcmp(distro, "Ubuntu")) {
        if(access(path2, F_OK) < 0) {
            if(errno == ENOENT) {
                if(symlink(path, path2)) {
                    if(errno == EEXIST)
                        logentry = mkString("[WARNING] (reciver) Vhost: ", vhd->ServerName, " byl juz aktywny", NULL);
                    else
                        logentry = mkString("[ERROR] (reciver) Nie udalo sie aktywowac konfiguracji dla ", vhd->ServerName, NULL);
                    writeLog(lf, logentry);
                }
            }
        }
    }
    free(path);
    free(path2);
    
    return 1;
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
void removeVhost(char * os, vhostData * vhd) {
    char * vhostConfig = NULL;
    char * vhostConfigSymlink = NULL;
    char * vhostDir = NULL;

    vhostDir = mkString("/var/www/", vhd->ServerName, "/", NULL);
    if(!strcmp(os, "Ubuntu")) {
        vhostConfig = mkString("/etc/apache2/sites-available/", vhd->ServerName, ".conf", NULL);
        vhostConfigSymlink = mkString("/etc/apache2/sites-enabled/", vhd->ServerName, ".conf", NULL);
    }
    else if(strstr(os, "Centos") != NULL) {
        vhostConfig = mkString("/etc/httpd/conf.d/", vhd->ServerName, ".conf", NULL);
    }
    unlink(vhostConfig);
    if(vhostConfigSymlink != NULL)
        unlink(vhostConfigSymlink);
    
    if(!strcmp(vhd->purgedir, "Y"))
        purgeDir(vhostDir);

    if(vhostConfig != NULL) free(vhostConfig);
    if(vhostConfigSymlink != NULL) free(vhostConfigSymlink);
    if(vhostDir != NULL) free(vhostDir);
}
int getVhostPackageSize(vhostData * vhd) {
    int size = 0;           // licznik bajtow
    int keysize = 0;        // rozmiar kluczy w pakiecie
    vhostData * pos = vhd;  // aktualna pozycja w pamieci
    char * tmp = NULL;      // tymczasowa zmienna do przechowania
                            // wartosci numerycznych w formie stringu
    int vhostCount = 0;     // zliczanie liczby vhostow

    char * header = "{scope:apache,},";
    // nazwy kluczy w pakiecie;
    const char * keys[] = { "DocumentRoot:,", "ServerAlias:,", "ServerName:,", "ApacheOpts:,",
                            "htaccess:,", "htusers:,", "purgedir:,", "vhoststatus:,", "user:,",
                            "VhostAccessOrder:,", "VhostAccessList:,", "config_ver:",
                            "vhost_:", "dbid:,", "{},", NULL};
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
        
        tmp = int2String(pos->dbid);
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
    size += strlen(header);
    size += keysize * vhostCount;
    
    return size;
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
resp * updateApacheSetup(httpdata www, char * os, FILE * lf) {
    vhostData * vh = www.vhost;
    htpasswdData * authData = www.htpasswd;
    char * htaccessPath = NULL;
    char * lmsg = NULL;
    
    // response to server
    resp * rhead = NULL;
    resp * rcurr = NULL;
    resp * rprev = NULL;
    
    while(vh) {
        htaccessPath = mkString(vh->DocumentRoot, "/.htaccess", NULL);
        if(!strcmp(vh->status, "N") || !strcmp(vh->status, "U")) {
            if(createVhostConfig(os, vh, lf)) {
                if(vh->password_access)
                    apacheAuthConfig(os, vh, authData, lf);
                if(!strcmp(vh->status, "N"))
                    createWebsiteDir(vh->DocumentRoot);
                if(strcmp(vh->htaccess, "NaN"))
                    createHtaccess(htaccessPath, vh->htaccess);
                else
                    unlink(htaccessPath);
                
                rcurr = respStatus("apache", 'A', vh->dbid);
                if(rhead == NULL)
                    rhead = rcurr;
                else
                    rprev->next = rcurr;
                rprev = rcurr;
                
                if(!strcmp(vh->status, "N"))
                    lmsg = mkString("[INFO] (reciver) Konfiguracja witryny ", vh->ServerName, " zostala utworzona", NULL);
                else if(!strcmp(vh->status, "U"))
                    lmsg = mkString("[INFO] (reciver) Konfiguracja witryny ", vh->ServerName, " zostala zaktualizowana", NULL);
            }
            else
                lmsg = mkString("[ERROR] (reciver) Blad tworzenia konfiguracji dla witryny ", vh->ServerName, NULL);
            writeLog(lf, lmsg);
        }
        if(!strcmp(vh->status, "D")) {
            removeVhost(os, vh);
            
            rcurr = respStatus("apache", 'D', vh->dbid);
            if(rhead == NULL)
                rhead = rcurr;
            else
                rprev->next = rcurr;
            rprev = rcurr;
        }
        free(htaccessPath);
        vh = vh->next;
    }
    if(authData != NULL) {
        createHtpasswdFile(os, www.htpasswd);
    }
    else {
        clearAuthData(os);
    }
    
    lmsg = mkString("[INFO] (reciver) Konfiguracja apacza gotowa.", NULL);
    writeLog(lf, lmsg);
    reloadApache(os);
    
    return rhead;
}