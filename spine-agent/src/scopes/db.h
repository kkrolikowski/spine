#ifndef DB_H
#define DB_H
#include <mysql.h>
#include "commondata.h"

// Function manages databases on particular host. Creates new and removes
// old ones.
resp * DatabaseSetup(dbinfo * db, char * os, FILE * lf, resp * respdata);

// database users management
resp * DatabaseUsersSetup(dbuser * db, char * os, FILE * lf, resp * respdata);

// database privileges
resp * DatabaseUserGrantsSetup(grants * db, char * os, FILE * lf, resp * respdata);

// function performes on database one of the actions:
// add new or remove's existing based on action flag. 
// Returned values: 1 - success, 0: failure
int dbmgr(char * dbname, char action, char * os);

// function manages database users. Performes on of the actions:
// add new, upates password or removes existing user.
// Returned values: 1 - success, 0 - failure
int dbusermgr(dbuser * db, char action, char * os);

int dbgrantsmgr(grants * db, char action, char * os);
// function checks if user exists in database
int mysqlUserExist(MYSQL * dbh, char * login);

// function initiates connection to localhost database. It has hardcoded
// root@localhost account
MYSQL * mysqlconn(char * os);

#endif /* DB_H */

