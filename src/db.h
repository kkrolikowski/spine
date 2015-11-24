#ifndef DB_H_INCLUDED
#define DB_H_INCLUDED

/*
* Function builds complete sql query string.
* agruments are strings. Last arg should be NULL
* if success function returns string, otherwise NULL.
*/
char * BuildQuery(char *, ...);

#endif // DB_H_INCLUDED
