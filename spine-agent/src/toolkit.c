/* toolkit.c -- common purpose functions library */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include "toolkit.h"
#include "core.h"

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
        strncpy(str, tmp, strlen(tmp));
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
        strncpy(str, tmp, strlen(tmp));
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
        strncpy(str, tmp, strlen(tmp));
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
char * mkString(char * qstr, ...) {
    va_list String;                     // String element
    size_t stringLenght = 0;            // number of bytes in whole string
    char * str = NULL;                  // result

    // First and next list elements
    char * Sfirst = qstr;
    char * Snext = qstr;

    // string lenth calculation
    va_start(String, qstr);
    while(Snext != NULL) {
        stringLenght += strlen(Snext);
        Snext = va_arg(String, char *);
    }
    va_end(String);

/*
 * Let's go to the first list element and put all pieces together
*/
    str = (char *) malloc((stringLenght + 1) * sizeof(char));
    memset(str, '\0', ((stringLenght + 1) * sizeof(char)));
    if(str != NULL) {
        memset(str, '\0', stringLenght + 1);
        Snext = Sfirst;
        va_start(String, qstr);
        while(Snext != NULL) {
            if(str[0] == '\0')
                strncpy(str, Snext, strlen(Snext));
            else
                strncat(str, Snext, strlen(Snext));
            Snext = va_arg(String, char *);
        }
        va_end(String);
    }
    return str;
}
void mkdirtree(char * path, mode_t mode, uid_t owner, gid_t group, FILE * lf) {
  int i = 0;
  char * p = path;
  char * lmsg = NULL;
  char * tmp = NULL;
  char buff[PATH_MAX];
  memset(buff, '\0', PATH_MAX);

  while(*p) {
    buff[i] = *p;
    if(*p == '/') {
      mkdir(buff, mode);
      if(!chown(buff, owner, group)) {
          tmp = int2String(owner);
          lmsg = mkString("[WARNING] Cannot change owner of ", buff, "to: ", tmp, NULL);
          free(tmp);
          writeLog(lf, lmsg);
      }
    }
    i++; p++;
  }
  mkdir(buff, mode);
  if(!chown(buff, owner, group)) {
    tmp = int2String(owner);
    lmsg = mkString("[WARNING] Cannot change owner of ", buff, "to: ", tmp, NULL);
    free(tmp);
    writeLog(lf, lmsg);
  }
}