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

#endif /* TOOLKIT_H */

