#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>
#include "apache.h"
#include "core.h"
#include "toolkit.h"

resp * HtpasswdSetup(htpasswdData * htp, char * os, FILE * lf, resp * rdata) {
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
            data = createHtpasswdFile(htp, htpasswdPath, lf, rdata);
        else
            return NULL;           // error creating directory
    }
    else
        data = createHtpasswdFile(htp, htpasswdPath, lf, rdata);
    
    free(htpasswdPath);
    
    return data;
}
resp * createHtpasswdFile(htpasswdData * htp, char * path, FILE * lf, resp * rdata) {
    htpasswdData * curr = htp;      // current position in memory
    char * scope = "htusers";       // response scope
    char * lmsg = NULL;             // log message

    // response data
    resp * rhead = NULL;
    resp * rcurr = NULL;
    resp * rprev = NULL;
    resp * rpos  = rdata;
    
    while(rhead != NULL)
        rhead = rhead->next;
    
    while(curr) {
        rcurr = (resp *) malloc(sizeof(resp));
        if(curr->status == 'N') {
            if(createHtpasswdEntry(curr, path)) {
                lmsg = mkString("[INFO] (reciver) Added user ", curr->login, " to .htpasswd file", NULL);
                rcurr->status = 'A';
            }
            else
                lmsg = mkString("[WARNING] (reciver) Error adding user ", curr->login, " to .htpasswd file", NULL);
            writeLog(lf, lmsg);
        }
        if(curr->status == 'U') {
            if(updateHtpasswdEntry(curr, path)) {
                lmsg = mkString("[INFO] (reciver) Updating user ", curr->login, " in .htpasswd file", NULL);
                rcurr->status = 'A';
            }
            else
                lmsg = mkString("[WARNING] (reciver) Error updating user ", curr->login, " in .htpasswd file", NULL);
            writeLog(lf, lmsg);
        }
        if(curr->status == 'D') {
            if(deleteHtpasswdEntry(curr, path)) {
                lmsg = mkString("[INFO] (reciver) User ", curr->login, " deleted from .htpasswd file", NULL);
                rcurr->status = 'D';
            }
            else
                lmsg = mkString("[WARNING] (reciver) Error deleting user ", curr->login, " from .htpasswd file", NULL);
            writeLog(lf, lmsg);
        }
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
    while(rdata) {
        if(rdata->next == NULL) {
            rdata->next = rhead;
            break;
        }
        rdata = rdata->next;
    }
    if(rpos != NULL)
        rdata = rpos;
    else
        rdata = rhead;
    
    return rdata;
}
int updateHtgroupFile(char * authDir, vhostData * vhd) {
    FILE * htgroup = NULL;
    FILE * tmp = NULL;
    char * htgroupFilePath = mkString(authDir, "/.htgroup", NULL);
    char * entry = NULL;
    int entryExist = 0;
    const int BufSize = 1024;
    char buff[BufSize];

    // ensure that auth directory exists
    if(mkdir(authDir, 0755) < 0) {
        if(errno != EEXIST) {
            free(htgroupFilePath);
            return 0;
        }
    }
    // when there's no inital .htgroup file
    if((htgroup = fopen(htgroupFilePath, "r")) == NULL) {
        if((htgroup = fopen(htgroupFilePath, "w")) == NULL)
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
            entryExist = 1;
        }
        else
            fputs(buff, tmp);
        memset(buff, '\0', BufSize);
    }
    fclose(htgroup);
    
    if(!entryExist) {
        entry = mkString(vhd->ServerName, ": ", vhd->htusers, "\n", NULL);
        fputs(entry, tmp);
        free(entry);
    }
    rewind(tmp);
    
    if((htgroup = fopen(htgroupFilePath, "w")) == NULL) {
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
        if(!updateHtgroupFile(authDir, vhd)) {
            lmsg = mkString("[ERROR] (reciver) Error preparing htgroup file");
            writeLog(lf, lmsg);
        }
    }
    else {
        if(!removeFromHtGroupFile(htgroupFilePath, vhd->ServerName)) {
            lmsg = mkString("[WARNING] (reciver) Error removing (or not exist) entry", vhd->ServerName, " from .htgroup file", NULL);
            writeLog(lf, lmsg);
        }
    }
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
void createWebsiteDir(vhostData * vh, FILE * lf) {
    char * dirpath = NULL;
    size_t len = 0;
    
    len = strlen(vh->DocumentRoot) + 2;
    dirpath = (char *) malloc(len * sizeof(char));
    memset(dirpath, '\0', len);
    
    strncpy(dirpath, vh->DocumentRoot, strlen(vh->DocumentRoot));
    strncat(dirpath, "/", 2);
    if(access(dirpath, F_OK) < 0) {
        if(errno == ENOENT) {
            if(vh->uid == 0)
                mkdirtree(vh->DocumentRoot, 0755, vh->uid, vh->uid, lf);
            else
                mkdirtree(vh->DocumentRoot, 0700, vh->uid, vh->uid, lf);
        }
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
    
    if((vhost = fopen(path, "w")) == NULL) {
        logentry = mkString("[ERROR] (reciver) Blad tworzenia pliku: ", path, NULL);
        writeLog(lf, logentry);
        free(path);
        free(path2);
        return 0;
    }
    acl_order = accessOrder(vhd->vhost_access_order);
    acl_entry = vhostACL(vhd->vhost_access_list);
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
    if(strcmp(vhd->user, "root")) {
        fprintf(vhost, "\t<IfModule mpm_itk_module>\n");
        fprintf(vhost, "\t\tAssignUserId %s %s\n", vhd->user, vhd->user);
        fprintf(vhost, "\t</IfModule>\n");
    }
    fprintf(vhost, "\tErrorLog %s/%s-error.log\n", logsDir, vhd->ServerName);
    fprintf(vhost, "\tCustomLog %s/%s-access.log combined\n", logsDir, vhd->ServerName);
    fprintf(vhost, "</VirtualHost>\n");
    fclose(vhost);
    free(acl_order);
    free(acl_entry);
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
    char * htpasswd_path = NULL;
    char * authDir = NULL;

    if(!strcmp(os, "Ubuntu")) {
        htgroup_path = "/etc/apache2/auth/.htgroup";
        htpasswd_path = "/etc/apache2/auth/.htpasswd";
        authDir = "/etc/apache2/auth";
    }
    else if(strstr(os, "Centos") != NULL) {
        htgroup_path = "/etc/httpd/auth/.htgroup";
        htpasswd_path = "/etc/httpd/auth/.htpasswd";
        authDir = "/etc/httpd/auth";
    }
    if(!fileExist(htgroup_path) && !fileExist(htpasswd_path))
        rmdir(authDir);
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
                    createWebsiteDir(vh, lf);
                if(!strcmp(vh->status, "U"))
                    updateDirPermissions(vh->DocumentRoot, vh->uid, vh->uid, lf);
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
            lmsg = mkString("[INFO] (reciver) Konfiguracja witryny ", vh->ServerName, " zostala usunięta", NULL);
            writeLog(lf, lmsg);
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
    lmsg = mkString("[INFO] (reciver) Konfiguracja apacza gotowa.", NULL);
    writeLog(lf, lmsg);
    reloadApache(os);
    
    return rhead;
}
int removeFromHtGroupFile(char * path, char * entry) {
    FILE * htgroup = NULL;
    FILE * tmp = NULL;
    int items = 0;
    const int Size = 1024;
    char buff[Size];
    
    if((htgroup = fopen(path, "r")) == NULL)
        return -1;
    if((tmp = tmpfile()) == NULL) {
        fclose(htgroup);
        return 0;
    }
    
    memset(buff, '\0', Size);
    while(fgets(buff, Size, htgroup) != NULL) {
        if(strstr(buff, entry) != NULL)
            continue;
        else {
            fputs(buff, tmp);
            items++;
        }
        memset(buff, '\0', Size);
    }
    fclose(htgroup);
    
    if(items > 0) {
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
    }
    else {
        fclose(tmp);
        unlink(path);
    }
    return 1;
}
char * vhostACL(char * str) {
    char * out      = NULL;                 // output string
    char * curr     = str;                  // current character in string
    char * allow    = "\t\tAllow from ";    // keywords #1
    char * deny     = "\t\tDeny from ";     // keywords #2
    int items       = 0;                    // items in string to process
    int acl_seq     = 0;                    // number of acl sequences
    size_t memsize  = 0;                    // memory size which handle output string
    char buff[16];                          // temporary buffer, should be enough
                                            // to store ip address
    int index       = 0;                    // buffer index
    
    // let's check how many entries we have to process
    while(*curr) {
        if(*curr == '#')
            items++;
        curr++;
    }
    items += 1;                 // there's allways one item more than #
    acl_seq = 2 * items;        // acl sequeces are 2 times more than items
    
    // size of input string + string length of longest direcive - number
    // of streering acl sequences + two extra bytes, one for newline, second
    // for string zero sign.
    memsize = strlen(str) + (strlen(allow) * items) - acl_seq + 2;
    out = (char *) malloc(memsize * sizeof(char));
    memset(out, '\0', memsize);
    
    // let's move to the begining and prepare temporary buffer
    curr = str;
    memset(buff, '\0', 16);
    while(*curr) {
        if(*curr != ':')
            buff[index++] = *curr++;
        else {
            if(*(curr + 1) == '1')
                strncat(out, allow, strlen(allow));
            else
                strncat(out, deny, strlen(deny));
            strncat(out, buff, strlen(buff));
            *(out + strlen(out)) = '\n';
            memset(buff, '\0', 16);
            if(*(curr + 2) == '#')
                curr = curr + 3;
            else
                break;
            index = 0;
        }
    }
    return out;
}
int createHtpasswdEntry(htpasswdData * data, char * path) {
    FILE * htpasswd = NULL;     // .htpasswd file
    
    // if .htpasswd doesn't exist we need to create it
    if((htpasswd = fopen(path, "r")) == NULL) {
        if((htpasswd = fopen(path, "w")) == NULL)
            return 0;
        else {
            fprintf(htpasswd, "%s:%s\n", data->login, data->pass);
            fclose(htpasswd);
            return 1;
        }
    }
    fclose(htpasswd);
    
    if((htpasswd = fopen(path, "a")) == NULL)
        return 0;
    fprintf(htpasswd, "%s:%s\n", data->login, data->pass);
    fclose(htpasswd);
    
    return 1;
}
int updateHtpasswdEntry(htpasswdData * data, char * path) {
    FILE * htpasswd     = NULL;     // .htpasswd file
    FILE * tmp          = NULL;     // temp file
    char * entry        = NULL;
    const int Size      = 1024;     // buffer size
    char buff[Size];                // temporary buffer
    
    if((htpasswd = fopen(path, "r")) == NULL)
        return 0;
    if((tmp = tmpfile()) == NULL) {
        fclose(htpasswd);
        return 0;
    }
    
    memset(buff, '\0', Size);
    while(fgets(buff, Size, htpasswd) != NULL) {
        if(strstr(buff, data->login) != NULL) {
            entry = mkString(data->login, ":", data->pass, "\n", NULL);
            fputs(entry, tmp);
            free(entry);
        }
        else
            fputs(buff, tmp);
        memset(buff, '\0', Size);
    }
    fclose(htpasswd);
    
    rewind(tmp);
    if((htpasswd = fopen(path, "w")) == NULL) {
        fclose(tmp);
        return 0;
    }
    while(fgets(buff, Size, tmp) != NULL) {
        fputs(buff, htpasswd);
        memset(buff, '\0', Size);
    }
    fclose(htpasswd);
    fclose(tmp);
    
    return 1;
}
int deleteHtpasswdEntry(htpasswdData * data, char * path) {
    FILE * htpasswd = NULL;
    FILE * tmp      = NULL;
    int items       = 0;
    const int Size  = 1024;
    char buff[Size];
    
    if((htpasswd = fopen(path, "r")) == NULL)
        return 0;
    if((tmp = tmpfile()) == NULL) {
        fclose(htpasswd);
        return 0;
    }
    
    memset(buff, '\0', Size);
    while(fgets(buff, Size, htpasswd) != NULL) {
        if(strstr(buff, data->login) != NULL) {
            memset(buff, '\0', Size);
            continue;
        }
        fputs(buff, tmp);
        memset(buff, '\0', Size);
        items++;
    }
    fclose(htpasswd);
    
    if(items > 0) {
        rewind(tmp);
        if((htpasswd = fopen(path, "w")) == NULL) {
            fclose(tmp);
            return 0;
        }
        while(fgets(buff, Size, tmp) != NULL) {
            fputs(buff, htpasswd);
            memset(buff, '\0', Size);
        }
        fclose(tmp);
        fclose(htpasswd);
    }
    else {
        fclose(tmp);
        unlink(path);
    }
    return 1;
}