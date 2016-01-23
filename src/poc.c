/* poc.c -- functions code testing */
#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include "db.h"

int main(void) {
    dbConnData db;

    dbConnDataInit(&db);

    char * login = "krolik";
    char * domain = "bsdaemon.pl";
    char * dbhost = "pazuzu.lan";
    char * dbuser = "dyndnsx";
    char * dbpass = "xxx";
    char * dbname = "dyndns";

    if(!dbLogin(dbhost, dbuser, dbpass, dbname))
        printf("Can't connect to MySQL server: %s", dbhost);

    char * sql = BuildQuery("SELECT * FROM users WHERE login = '", login, "' AND domain = '", domain, "'", NULL);


    free(sql);
    return 0;
}
