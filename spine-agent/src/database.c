/* database.c -- zestaw funkcji, ktore operuja na bazie danych */
#include <stdio.h>
#include <stdlib.h>
#include "database.h"

void InitDBConnData(dbconn * db) {
	db->dbhost = NULL;
	db->dblogin = NULL;
	db->dbname = NULL;
	db->dbpass = NULL;
}
void clearDBConnData(dbconn * db) {
	free(db->dbhost);
	free(db->dblogin);
	free(db->dbname);
	free(db->dbpass);
}
