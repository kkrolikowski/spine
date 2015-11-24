/* poc.c -- functions code testing */
#include <stdio.h>
#include <stdlib.h>
#include "db.h"

int main(void) {
    char * login = "krolik";
    char * domain = "bsdaemon.pl";

    char * sql = BuildQuery("SELECT * FROM users WHERE login = '", login, "' AND domain = '", domain, "'", NULL);

    puts(sql);

    free(sql);
    return 0;
}
