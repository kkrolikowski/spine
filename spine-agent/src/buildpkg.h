#ifndef BUILDPKG_H
#define BUILDPKG_H
#include "commondata.h"

/* Apche scope */

// obtaining memory size
int getVhostPackageSize(vhostData * vhd);
int htusersDataSize(htpasswdData * htpass);
// subpackages
char * apacheConfigPackage(vhostData * www);
char * htpasswdConfigPackage(htpasswdData * htpass);
// clear memory
void cleanVhostData(vhostData * vhd);
void clearHtpasswdData(htpasswdData * htpasswd);

/* System users scope */

// obtaining memory size
int getSSHkeysPackageSize(sshkeys * ssh);
int getSysUsersPackageSize(sysuser * su);
// subpackages
char * sysusersPackage(sysuser * su);
char * sshkeysPackage(sshkeys * k);
// clear memory
void cleanSysUsersData(sysuser * su);
void cleanSSHKeyData(sshkeys * k);

/* Database scope*/

// obtaining memory size
int DBnamesDataSize(dbinfo * db);
int DBusersDataSize(dbuser * db);
int DBgrantsDataSize(grants * db);
// subpackages
char * DBNamesConfigPackage(dbinfo * db);
char * DBgrantsConfigPackage(grants * db);
char * DBusersConfigPackage(dbuser * db);
// clear memory
void cleanDBgrantsData(grants * db);
void cleanDBinfoData(dbinfo * db);
void cleanDBusersData(dbuser * db);

#endif /* BUILDPKG_H */