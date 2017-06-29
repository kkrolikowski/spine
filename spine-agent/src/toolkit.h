/* toolkit.h -- common purpose functions library */
#ifndef TOOLKIT_H
#define TOOLKIT_H
#include "commondata.h"
// fileExist() tries to open file with given path in readonly mode
// return values: 0 - failure, 1 - success
int fileExist(char * path);

// int2String() converts given integer number into string.
// int2string returns pointer to allocated memory or NULL.
// _NOTICE_: Please remember to free() allocated memory after string usage.
char * int2String(int n);

// long2String() converts given long number into string.
// long2String returns pointer to allocated memory or NULL.
// _NOTICE_: Please remember to free() allocated memory after string usage.
char * long2String(long n);

// ulong2String() converts given unsigned long number into string.
// ulong2String returns pointer to allocated memory or NULL.
// _NOTICE_: Please remember to free() allocated memory after string usage.
char * ulong2String(unsigned long n);

// timestamp() returns preformatted string with current timestamp
// timestamp format: YYY-MM-DD hh:mm:ss. Timestamp returns NULL 
// on memory allocation problems
// _NOTICE_: Please remember to free() allocated memory after string usage.
char * timestamp(void);

// mkString() is designed to concatenate all agruments passed to it into one string.
// Sample usage: str = mkString("string literal ", string1, " string literal", NULL);
// _IMPORTANT_: string list should be terminated with NULL.
// _NOTICE_: Please remember to free() allocated memory after string usage.
char * mkString(char * qstr, ...);

// mkdirtree() creates directory tructure based on givent path.
// aditional parameters required are permission mode, (i.e. 0755), UID, GID
// and logfile handle
void mkdirtree(char * path, mode_t mode, uid_t owner, gid_t group, FILE * lf);

// updateDirPermissions() alters permissions along the given path.
// This is a recursive function that takes additional arguments which are
// UID and GID. Files and directories modes are hardcoded:
// *     root user: 0755 - directories, 0644 - files
// * non-root user: 0710 - directories, 0600 - files
void updateDirPermissions(char * path, uid_t uid, gid_t gid, FILE * lf);

// purgeDir() is a recursive function that removes given directory path (name)
// with all it's contents.
void purgeDir(char * name);

// recursiveCopy() copies contents of directory available at a given path
// into /home/user directory. Function sets basic pemission on destination files
// and directories. /home/ path is hardcoded
void recursiveCopy(sysuser * su, char * path, FILE * lf);

// copy() is a simple function, that opens "from" and writes it's contents into
// "to".
int copy(char * from, char * to);

#endif /* TOOLKIT_H */