/* toolkit.h -- common purpose functions library */
#ifndef TOOLKIT_H
#define TOOLKIT_H

// fileExist() tries to open file with given path in readonly mode
// return values: 0 - failure, 1 - success
int fileExist(char * path);

// int2String() converts given integer number into string.
// int2string returns pointer to allocated memory or NULL.
// _NOTICE_: Please remember to free() allocated memory after string usage.
char * int2String(int n);

#endif /* TOOLKIT_H */

