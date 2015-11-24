#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "db.h"

char * BuildQuery(char * qstr, ...) {
    va_list Qpart;                      // query string part
    size_t stringLenght = 0;            // needed for memory allocation
    char * sql = NULL;                  // return value of complete query

    // first and next query part.
    // they hold the same address at the beginning
    char * Qfirst = qstr;
    char * Qnext = qstr;

    // obtaining string lengh of all parts of the query
    va_start(Qpart, qstr);
    while(Qnext != NULL) {
        stringLenght += strlen(Qnext);
        Qnext = va_arg(Qpart, char *);
    }
    va_end(Qpart);

/*
 *  Building complete SQL query string
*/
    sql = (char *) malloc((stringLenght + 1) * sizeof(char));   // prepare enough memory
    if(sql != NULL) {
        memset(sql, '\0', stringLenght + 1);
        Qnext = Qfirst;                       // let's move to the begining
        va_start(Qpart, qstr);
        while(Qnext != NULL) {
            if(sql[0] == '\0')                // if string is empty let's call strcpy
                strcpy(sql, Qnext);           // otherwise concatenate.
            else
                strcat(sql, Qnext);
            Qnext = va_arg(Qpart, char *);
        }
        va_end(Qpart);
    }
    return sql;
}
