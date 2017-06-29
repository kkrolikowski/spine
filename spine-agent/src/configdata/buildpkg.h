#ifndef BUILDPKG_H
#define BUILDPKG_H
#include "commondata.h"

/* Apche scope */

// obtaining memory size
int getVhostPackageSize(vhostData * vhd, char * scope);
int htusersDataSize(htpasswdData * htpass, char * scope);
// subpackages
char * apacheConfigPackage(vhostData * www, char * scope);
char * htpasswdConfigPackage(htpasswdData * htpass, char * scope);
// clear memory
void cleanVhostData(vhostData * vhd);
void clearHtpasswdData(htpasswdData * htpasswd);

/* System users scope */

// obtaining memory size
int getSSHkeysPackageSize(sshkeys * ssh);
int getSysUsersPackageSize(sysuser * su, char * scope);
// subpackages
char * sysusersPackage(sysuser * su, char * scope);
char * sshkeysPackage(sshkeys * k);
// clear memory
void cleanSysUsersData(sysuser * su);
void cleanSSHKeyData(sshkeys * k);

/* Database scope*/

// obtaining memory size
int DBnamesDataSize(dbinfo * db, char * scope);
int DBusersDataSize(dbuser * db, char * scope);
int DBgrantsDataSize(grants * db, char * scope);
// subpackages
char * DBNamesConfigPackage(dbinfo * db, char * scope);
char * DBgrantsConfigPackage(grants * db, char * scope);
char * DBusersConfigPackage(dbuser * db, char * scope);
// clear memory
void cleanDBgrantsData(grants * db);
void cleanDBinfoData(dbinfo * db);
void cleanDBusersData(dbuser * db);

#endif /* BUILDPKG_H */