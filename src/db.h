#ifndef DB_H_INCLUDED
#define DB_H_INCLUDED
#include <mysql.h>

/*                          */
/*         Data types       */
/*                          */
/* Data type used for Database connection handling */
typedef struct dbConnData {
   char * db_host;      // Database host
   char * db_user;      // Database user
   char * db_pass;      // Database password
   char * db_name;      // Database name
} dbConnData;
/*                          */
/*         Functions        */
/*                          */
/*
 * Function initializes connection data structure
*/
void dbConnDataInit(dbConnData * dbcd);
/*
* Function builds complete sql query string.
* agruments are strings. Last arg should be NULL
* if success function returns string, otherwise NULL.
*/
char * BuildQuery(char *, ...);

/*
 * dbhInit() -- allocates memmory for connection hadling
 * Return values:
 *  MYSQL handle - on success
 *  NULL pointer - on failure
*/
MYSQL * dbhInit(void);

/*
 * dbLogin() -- tries to login to databse.
 * Returns valid handler to database:
 *  MYSQL* - on success
 *  NULL - on failure
*/
MYSQL * dbLogin(dbConnData * dbcd);
#endif // DB_H_INCLUDED
