#ifndef EXTRACTPKG_H
#define EXTRACTPKG_H

/* Apache scope */

vhostData * ParseConfigDataAPACHE(char * json);
htpasswdData * ParseConfigDataHTPASSWD(char * json);

/* Sysusers scope */

sysuser * ParseConfigDataSYSUSERS(char * json);
sshkeys * ParseConfigDataSSHKeys(char * str);

/* Database scope */

dbinfo * ParseConfigDataDBNAMES(char * json);
dbuser * ParseConfigDataDBUSERS(char * json);
grants * ParseConfigDataDBPRIVS(char * json);

#endif /* EXTRACTPKG_H */

