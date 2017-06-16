/* toolkit.c -- common purpose functions library */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "toolkit.h"

int fileExist(char * path) {
    FILE * fp = NULL;

    if((fp = fopen(path, "r")) == NULL)
        return 0;
    else {
        fclose(fp);
        return 1;
    }
}
char * int2String(int n) {
    char tmp[11];
    memset(tmp, '\0', 11);
    char * str = NULL;
    size_t len = 0;

    sprintf(tmp, "%d", n);
    len = strlen(tmp) + 1;
    str = (char *) malloc(len * sizeof(char));
    if(str != NULL) {
        memset(str, '\0', len);
        strcpy(str, tmp);
    }

    return str;
}
char * long2String(long n) {
    char tmp[12];
    memset(tmp, '\0', 12);
    char * str = NULL;
    size_t len = 0;

    sprintf(tmp, "%ld", n);
    len = strlen(tmp) + 1;
    str = (char *) malloc(len * sizeof(char));
    if(str != NULL) {
        memset(str, '\0', len);
        strcpy(str, tmp);
    }
    
    return str;
}
char * ulong2String(unsigned long n) {
    char tmp[12];
    memset(tmp, '\0', 12);
    char * str = NULL;
    size_t len = 0;

    sprintf(tmp, "%lu", n);
    len = strlen(tmp) + 1;
    str = (char *) malloc(len * sizeof(char));
    if(str != NULL) {
        memset(str, '\0', len);
        strcpy(str, tmp);
    }

    return str;
}
char * timestamp(void) {
    time_t epoch;
    char * timestamp = NULL;
    struct tm * now;

    time(&epoch);
    timestamp = (char *) malloc(20 * sizeof(char));
    if(timestamp != NULL) {
        memset(timestamp, '\0', 20);
        now = localtime(&epoch);
        strftime(timestamp, 20, "%F %T", now);
    }
    return timestamp;
}