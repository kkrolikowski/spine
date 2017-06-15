/* toolkit.c -- common purpose functions library */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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