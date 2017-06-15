#ifndef DBDATA_H
#define DBDATA_H

/* Apache scope */

vhostData * getVhostData(char * hostid);
htpasswdData * getHtpasswdData(char * hostid);

/* System users scope */

sysuser * getSystemAccounts(char * systemid);
sshkeys * getSSHkeys(char * str);

/* Database scope */

dbinfo * getDatabaseNames(char * systemid);
dbuser * getDatabaseUsers(char * systemid);
grants * getDatabasePrivileges(char * systemid);

#endif /* DBDATA_H */

