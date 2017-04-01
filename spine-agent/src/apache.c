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
    htpasswdData * curr = htpass;       // current node
    int size = 0;                       // overrall data size
    int keySize = 0;                    // key names size
    int nodeCount = 0;                  // processed nodes count
    char * tmp = NULL;                  // temporary string

    // package header
    char * header = "{scope:htusers,},";    
    // package keys names
    char * keys[] = { "user_:" "login:,", "password:,", "dbid:,", 
                      "status:,", "config_version:,", NULL 
                    };
    char ** key = keys;
    
    while(*key)
        keySize += strlen(*key++);
    
    while(curr) {
        size += strlen(curr->login);
        size += strlen(curr->pass);
        
        tmp = int2String(curr->dbid);
        size += strlen(tmp);
        free(tmp);
        
        // string length of each user index number
        tmp = int2String(nodeCount);
        size += strlen(tmp);
        free(tmp);
        
        size += 1;          // one byte for status flag in each item
        if(curr->next == NULL) {
            tmp = int2String(curr->version);
            size += strlen(tmp);
            free(tmp);
        }
        nodeCount++;
        curr = curr->next;
    }
    size += strlen(header);
    size += keySize * nodeCount;
    
    return size;
}
char * htpasswdConfigPackage(htpasswdData * htpass) {
    // common data
    htpasswdData * curr     = htpass;   // node traversing pointer
    int size = 0;                       // package size
    int idx  = 0;			// item number
    char * package          = NULL;     // output package
    char * numstr           = NULL;	// item number as a string
    char * entry            = NULL;     // particular entry definition
    char * s_dbid           = NULL;     // DB ID in a form of string
    // specific data
    char status[2];                     // status flags can be: NUDA

    // package header
    char * header = "{scope:htusers,";

    // config version
    char * k_config_ver = "config_ver:";
    char * s_config_ver = NULL;
    
    if(htpass == NULL)
        return NULL;
    
    // preparing memory
    size = htusersDataSize(htpass) + 1;
    package = (char *) malloc(size * sizeof(char));
    memset(package, '\0', size);
  
    // building a package
    strncpy(package, header, strlen(header));
    while(curr) {
        numstr = int2String(idx);
        s_dbid = int2String(curr->dbid);
        memset(status, '\0', 2);
        status[0] = curr->status;
        
        entry = mkString(
                        "user_",            numstr,        ":{",
                        "dbid:",            s_dbid,         ",",
                        "login:",           curr->login,    ",",
                        "password:",        curr->pass,     ",",
                        "status:",          status,         ",",
                        NULL);
        
        strncat(package, entry, strlen(entry) + 1);
        if(curr->next == NULL)
            s_config_ver = int2String(curr->version);

        // zwalniamy pamiec i przygotowujemy zmienne do kolejnej iteracji
        free(s_dbid);
        free(entry);
        free(numstr);
        idx++;
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
char * apacheConfigPackage(vhostData * www) {
    // common data
    vhostData * curr       = www;       // node traversing pointer
    int size               = 0;         // package size
    int idx                = 0;		// vhost index
    char * package         = NULL;      // output package
    char * numstr          = NULL;	// vhost index as a string
    char * entry           = NULL;      // particular vhost definition
    char * s_dbid          = NULL;      // DB ID in a form of string
    // specific data
    char * authbasic       = NULL;	// authbasic flag

    // package header
    char * header = "{scope:apache,";

    // config version
    char * k_config_ver = "config_ver:";
    char * s_config_ver = NULL;

    if(www == NULL)
        return NULL;
    
    // preparing memory
    size = getVhostPackageSize(www) + 1;
    package = (char *) malloc(size * sizeof(char));
    memset(package, '\0', size);

    // package building
    strncpy(package, header, strlen(header)); // configuration type
    while(curr) {
        numstr = int2String(idx);
        authbasic = int2String(curr->password_access);
        s_dbid = int2String(curr->dbid);
        entry = mkString(
                        "vhost_", numstr, ":{",
                        "dbid:",             s_dbid,                   ",",
                        "ServerName:",       curr->ServerName,         ",",
                        "ServerAlias:",      curr->ServerAlias,        ",",
                        "DocumentRoot:",     curr->DocumentRoot,       ",",
                        "ApacheOpts:",       curr->apacheOpts,         ",",
                        "VhostAccessOrder:", curr->vhost_access_order, ",",
                        "VhostAccessList:",  curr->vhost_access_list,  ",",
                        "htaccess:",         curr->htaccess,           ",",
                        "authbasic:",        authbasic,                ",",
                        "htusers:",          curr->htusers,            ",",
                        "vhoststatus:",      curr->status,             ",",
                        "purgedir:",         curr->purgedir,           ",",
                        "user:",             curr->user,               "}",
                        ",", NULL);
        strncat(package, entry, strlen(entry) + 1);
        if(curr->next == NULL)
            s_config_ver = int2String(curr->version);

        // zwalniamy pamiec i przygotowujemy zmienne do kolejnej iteracji
        free(s_dbid);
        free(entry);
        free(numstr);
        free(authbasic);
        if(strcmp(curr->ServerName, "NaN"))
            idx++;
        curr = curr->next;
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
resp * HtpasswdSetup(htpasswdData * htp, char * os, resp * rdata) {
    char * authDir = NULL;      // os related directory "auth" path
    char * htpasswdPath = NULL; // file path to .htpasswd
    
    // response data
    resp * data = NULL;
    
    // let's create path to .htpasswd file
    if(!strcmp(os, "Ubuntu"))
        authDir = "/etc/apache2/auth";
    else
        authDir = "/etc/httpd/auth";
    htpasswdPath = mkString(authDir, "/.htpasswd", NULL);
    
    // let's create auth directory
    if(mkdir(authDir, 0755) < 0) {
        if(errno == EEXIST)
            data = createHtpasswdFile(htp, htpasswdPath, rdata);
        else
            return NULL;           // error creating directory
    }
    else
        data = createHtpasswdFile(htp, htpasswdPath, rdata);
    
    free(htpasswdPath);
    
    return data;
}
resp * createHtpasswdFile(htpasswdData * htp, char * path, resp * rdata) {
    FILE * htpasswd = NULL;         // filehandler for .htpasswd
    htpasswdData * curr = htp;      // current position in memory
    char * buff = NULL;             // buffer with logins and passwords
    char * entry = NULL;            // buffer entry;
    char * scope = "htusers";       // response scope
    size_t buffLen = 0;             // amount of memory for buffer
    
    // response data
    resp * rhead = rdata;
    resp * rcurr = NULL;
    resp * rprev = NULL;
    
    while(rhead != NULL)
        rhead = rhead->next;
    
    // obtaining memory size to allocate
    while(curr) {
        if(curr->status == 'N' || curr->status == 'U')
            buffLen += strlen(curr->login) + strlen(curr->pass) + 2;
        curr = curr->next;
    }
    
    // preparing memory for all entries which should be placed in .htpasswd file
    buffLen += 1;
    buff = (char *) malloc(buffLen * sizeof(char));
    memset(buff, '\0', buffLen);
    
    // feeding the buffer and creating response nodes for further processing
    curr = htp;
    while(curr) {
        rcurr = (resp *) malloc(sizeof(resp));
        if(curr->status == 'N' || curr->status == 'U') {
            entry = mkString(curr->login, ":", curr->pass, "\n", NULL);
            strncat(buff, entry, strlen(entry));
            free(entry);         
            rcurr->status = 'A';
        }
        else
            rcurr->status = 'D';
        
        rcurr->dbid = curr->dbid;
        rcurr->scope = (char *) malloc((strlen(scope) +1) * sizeof(char));
        memset(rcurr->scope, '\0', strlen(scope) +1);
        strncpy(rcurr->scope, scope, strlen(scope));
        rcurr->next = NULL;
        
        if(rhead == NULL)
            rhead = rcurr;
        else
            rprev->next = rcurr;
        rprev = rcurr;
        
        curr = curr->next;
    }
    if(strlen(buff)) {
        if((htpasswd = fopen(path, "w")) == NULL)
            return NULL;
        fputs(buff, htpasswd);
        fclose(htpasswd);
        free(buff);
    }
    return rhead;
}
int createHtgroupFile(char * path, vhostData * vhd) {
    FILE * htgroup = NULL;
    FILE * tmp = NULL;
    char * entry = NULL;
    const int BufSize = 1024;
    char buff[BufSize];

    // when there's no inital .htgroup file
    if((htgroup = fopen(path, "r")) == NULL) {
        if((htgroup = fopen(path, "w")) == NULL)
            return 0;
        else {
            entry = mkString(vhd->ServerName, ": ", vhd->htusers, "\n", NULL);
            fputs(entry, htgroup);
            free(entry);
            fclose(htgroup);
            return 1;
        }
    }
    
    // .htgroup exists, we can proceed 
    if((tmp = tmpfile()) == NULL) {
        fclose(htgroup);
        return 0;
    }
    memset(buff, '\0', BufSize);
    while(fgets(buff, BufSize, htgroup) != NULL) {
        if(strstr(buff, vhd->ServerName) != NULL) {
            entry = mkString(vhd->ServerName, ": ", vhd->htusers, "\n", NULL);
            fputs(entry, tmp);
            free(entry);
        }
        else
            fputs(buff, tmp);
        memset(buff, '\0', BufSize);
    }
    fclose(htgroup);
    rewind(tmp);
    
    if((htgroup = fopen(path, "w")) == NULL) {
        fclose(tmp);
        return 0;
    }
    memset(buff, '\0', BufSize);
    while(fgets(buff, BufSize, tmp) != NULL) {
        fputs(buff, htgroup);
        memset(buff, '\0', BufSize);
    }
    fclose(tmp);
    fclose(htgroup);
    
    return 1;
}
void apacheAuthConfig(char * os, vhostData * vhd, FILE * lf) {
    char * lmsg = NULL;
    char * authDir = NULL;
    char * htgroupFilePath = NULL;

    if(!strcmp(os, "Ubuntu"))
        authDir = "/etc/apache2/auth";
    else if(strstr(os, "Centos") != NULL)
        authDir = "/etc/httpd/auth";
    
    htgroupFilePath = mkString(authDir, "/.htgroup", NULL);
    
    if(vhd->password_access) {
        if(mkdir(authDir, 0755) < 0) {
            if(errno == EEXIST) {
                if(!createHtgroupFile(htgroupFilePath, vhd)) {
                    lmsg = mkString("[ERROR] (reciver) Error preparing htgroup file");
                    writeLog(lf, lmsg);
                }
            }
            else {
                lmsg = mkString("[ERROR] (reciver) Blad tworzenia katalogu: ", authDir, "\n", NULL);
                writeLog(lf, lmsg);
            }
        }
        else {
            if(!createHtgroupFile(htgroupFilePath, vhd)) {
                lmsg = mkString("[ERROR] (reciver) Error preparing htgroup file");
                writeLog(lf, lmsg);
            }
        }
    }
    else {
        if(!removeFromHtGroupFile(htgroupFilePath, vhd->ServerName)) {
            lmsg = mkString("[ERROR] (reciver) Error removing entry", vhd->ServerName, " from .htgroup file");
            writeLog(lf, lmsg);
        }
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
    char * htgroup_path = NULL;

    if(!strcmp(os, "Ubuntu"))
        htgroup_path = "/etc/apache2/auth/.htgroup";
    else if(strstr(os, "Centos") != NULL)
        htgroup_path = "/etc/httpd/auth/.htgroup";
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
    vhostData * curr = vhd;     // current node
    int size = 0;               // overrall data size
    int keySize = 0;            // key names size
    int nodeCount = 0;          // processed nodes count
    char * tmp = NULL;          // temporary string
    
    // package header 
    char * header = "{scope:apache,},";
    // package keys names
    const char * keys[] = { "DocumentRoot:,", "ServerAlias:,", "ServerName:,", "ApacheOpts:,",
                            "htaccess:,", "htusers:,", "purgedir:,", "vhoststatus:,", "user:,",
                            "VhostAccessOrder:,", "VhostAccessList:,", "config_ver:",
                            "vhost_:", "dbid:,", "{},", "authbasic:,", NULL};
    const char ** key = keys;    
    
    while(*key)
        keySize += strlen(*key++);
    
    while(curr) {
        // string data
        size += strlen(curr->DocumentRoot);
        size += strlen(curr->ServerAlias);
        size += strlen(curr->ServerName);
        size += strlen(curr->apacheOpts);
        size += strlen(curr->htaccess);
        size += strlen(curr->htusers);
        size += strlen(curr->purgedir);
        size += strlen(curr->status);
        size += strlen(curr->user);
        size += strlen(curr->vhost_access_list);
        size += strlen(curr->vhost_access_order);

        // numeric data
        tmp = int2String(curr->password_access);
        size += strlen(tmp);
        free(tmp);
        
        tmp = int2String(nodeCount);
        size += strlen(tmp);
        free(tmp);
        
        tmp = int2String(curr->dbid);
        size += strlen(tmp);
        free(tmp);
        
        if(curr->next == NULL) {
            tmp = int2String(curr->version);
            size += strlen(tmp);
            free(tmp);
        }
                
        nodeCount++;
        curr = curr->next;
    }
    size += strlen(header);
    size += keySize * nodeCount;
    
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
    char * htaccessPath = NULL;
    char * lmsg = NULL;
    int authItems = 0;
    
    // response to server
    resp * rhead = NULL;
    resp * rcurr = NULL;
    resp * rprev = NULL;
    
    while(vh) {
        htaccessPath = mkString(vh->DocumentRoot, "/.htaccess", NULL);
        if(!strcmp(vh->status, "N") || !strcmp(vh->status, "U")) {
            if(createVhostConfig(os, vh, lf)) {
                apacheAuthConfig(os, vh, lf);
                if(vh->password_access)
                    authItems++;
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
    if(!authItems)
        clearAuthData(os);
    
    lmsg = mkString("[INFO] (reciver) Konfiguracja apacza gotowa.", NULL);
    writeLog(lf, lmsg);
    reloadApache(os);
    
    return rhead;
}
int removeFromHtGroupFile(char * path, char * entry) {
    FILE * htgroup = NULL;
    FILE * tmp = NULL;
    const int Size = 1024;
    char buff[Size];
    
    if((htgroup = fopen(path, "r")) == NULL)
        return 0;
    if((tmp = tmpfile()) == NULL) {
        fclose(htgroup);
        return 0;
    }
    
    memset(buff, '\0', Size);
    while(fgets(buff, Size, htgroup) != NULL) {
        if(strstr(buff, entry) != NULL)
            continue;
        else
            fputs(buff, tmp);
        memset(buff, '\0', Size);
    }
    fclose(htgroup);
    rewind(tmp);
    
    if((htgroup = fopen(path, "w")) == NULL) {
        fclose(tmp);
        return 0;
    }
    memset(buff, '\0', Size);
    while(fgets(buff, Size, tmp) != NULL) {
        fputs(buff, htgroup);
        memset(buff, '\0', Size);
    }
    fclose(tmp);
    fclose(htgroup);
    
    return 1;
}