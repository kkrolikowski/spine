#include <stdio.h>
#include <mysql.h>
#include <stdlib.h>
#include <string.h>
#include "commondata.h"
#include "toolkit.h"
#include "core.h"
#include "db.h"

resp * DatabaseSetup(dbinfo * db, char * os, FILE * lf, resp * respdata) {
    dbinfo * curr = db;
    char * msg = NULL;
    
    // response to server
    resp * rhead = NULL;
    resp * rcurr = NULL;
    resp * rprev = NULL;
    resp * rpos  = respdata;
    
    while(curr) {
        if(curr->status == 'N') {
            if(dbmgr(curr->dbname, curr->status, os))
                msg = mkString("[INFO] (reciver) Database: ", curr->dbname, " created", NULL);
            else
                msg = mkString("[ERROR] (reciver) Creation database: ", curr->dbname, " failed", NULL);
            writeLog(lf, msg);          
            rcurr = respStatus("db_name", 'A', curr->dbid);
        }
        if(curr->status == 'D') {
            if(dbmgr(curr->dbname, curr->status, os))
                msg = mkString("[INFO] (reciver) Database: ", curr->dbname, " removed", NULL);
            else
                msg = mkString("[ERROR] (reciver) Removing database: ", curr->dbname, " failed", NULL);
            writeLog(lf, msg);
            rcurr = respStatus("db_name", 'D', curr->dbid);
        }
        if(rhead == NULL)
            rhead = rcurr;
        else
            rprev->next = rcurr;
        rprev = rcurr;
        
        curr = curr->next;
    }
    while(respdata) {
        if(respdata->next == NULL) {
            respdata->next = rhead;
            break;
        }
        respdata = respdata->next;
    }
    if(rpos != NULL)
        respdata = rpos;
    else
        respdata = rhead;
    
    return respdata;
}
resp * DatabaseUsersSetup(dbuser * db, char * os, FILE * lf, resp * respdata) {
    dbuser * curr = db;
    char * msg = NULL;
    
    // response to server
    resp * rhead = NULL;
    resp * rcurr = NULL;
    resp * rprev = NULL;
    resp * rpos  = respdata;
    
    while(curr) {
        if(curr->status == 'N') {
            if(dbusermgr(curr, curr->status, os))
                msg = mkString("[INFO] (reciver) Database user: ", curr->login, " created", NULL);
            else
                msg = mkString("[ERROR] (reciver) Creation database user: ", curr->login, " failed", NULL);
            writeLog(lf, msg);           
            rcurr = respStatus("db_user", 'A', curr->dbid);
        }
        if(curr->status == 'D') {
            if(dbusermgr(curr, curr->status, os))
                msg = mkString("[INFO] (reciver) Database user: ", curr->login, " removed", NULL);
            else
                msg = mkString("[ERROR] (reciver) Removing database user: ", curr->login, " failed", NULL);
            writeLog(lf, msg);
            rcurr = respStatus("db_user", 'D', curr->dbid);
        }
        if(curr->status == 'U') {
            if(dbusermgr(curr, curr->status, os))
                msg = mkString("[INFO] (reciver) Database user: ", curr->login, " updated", NULL);
            else
                msg = mkString("[ERROR] (reciver) Updating database user: ", curr->login, " failed", NULL);
            writeLog(lf, msg);           
            rcurr = respStatus("db_user", 'A', curr->dbid);
        }
        if(rhead == NULL)
            rhead = rcurr;
        else
            rprev->next = rcurr;
        rprev = rcurr;
            
        curr = curr->next;
    }
    while(respdata) {
        if(respdata->next == NULL) {
            respdata->next = rhead;
            break;
        }
        respdata = respdata->next;
    }
    if(rpos != NULL)
        respdata = rpos;
    else
        respdata = rhead;
    
    return respdata;
}
resp * DatabaseUserGrantsSetup(grants * db, char * os, FILE * lf, resp * respdata) {
    grants * curr = db;
    char * msg = NULL;
    
    // response to server
    resp * rhead = NULL;
    resp * rcurr = NULL;
    resp * rprev = NULL;
    resp * rpos  = respdata;
    
    while(curr) {
        if(curr->status == 'N') {
            if(dbgrantsmgr(curr, curr->status, os))
                msg = mkString("[INFO] (reciver) Adding permissions to ", curr->dbname, " for ", curr->user, NULL);
            else
                msg = mkString("[ERRPR] (reciver) Adding permissions to ", curr->dbname, " for ", curr->user, " failed", NULL);
            writeLog(lf, msg);            
            rcurr = respStatus("db_privs", 'A', curr->dbid);
        }
        if(curr->status == 'D') {
            if(dbgrantsmgr(curr, curr->status, os))
                msg = mkString("[INFO] (reciver) Revoke permissions to ", curr->dbname, " for ", curr->user, NULL);
            else
                msg = mkString("[ERROR] (reciver) Revoke permissions to ", curr->dbname, " for ", curr->user, " failed", NULL);
            writeLog(lf, msg);
            rcurr = respStatus("db_privs", 'D', curr->dbid);
        }
        if(curr->status == 'U') {
            if(dbgrantsmgr(curr, curr->status, os))
                msg = mkString("[INFO] (reciver) Updating permissions to ", curr->dbname, " for ", curr->user, NULL);
            else
                msg = mkString("[INFO] (reciver) Updating permissions to ", curr->dbname, " for ", curr->user, " failed", NULL);
            writeLog(lf, msg);
            rcurr = respStatus("db_privs", 'A', curr->dbid);
        }
        if(rhead == NULL)
            rhead = rcurr;
        else
            rprev->next = rcurr;
        rprev = rcurr;
            
        curr = curr->next;
    }
    while(respdata) {
        if(respdata->next == NULL) {
            respdata->next = rhead;
            break;
        }
        respdata = respdata->next;
    }
    if(rpos != NULL)
        respdata = rpos;
    else
        respdata = rhead;
    
    return respdata;
}
int dbmgr(char * dbname, char action, char * os) {
    MYSQL * mysqlh = mysqlconn(os);
    char * query = NULL;
    int status = 0;
    
    if(action == 'N')
        query = mkString("CREATE DATABASE ", dbname, NULL);
    else if(action == 'D')
        query = mkString("DROP DATABASE ", dbname, NULL);
    
    if(!mysqlh)
        return 0;
    if(!mysql_query(mysqlh, query))
        status = 1;
    
    mysql_close(mysqlh);
    free(query);
    
    return status;
}
int dbusermgr(dbuser * db, char action, char * os) {
    MYSQL * mysqlh = mysqlconn(os);
    char * query = NULL;
    char * passColName = NULL;
    int status = 0;
    
    if(!strcmp(os, "Ubuntu"))
        passColName = "authentication_string";
    else
        passColName = "Password";
    
    if(action == 'N')
        query = mkString("CREATE USER '", db->login, "'@'localhost' IDENTIFIED ",
                         "BY PASSWORD '",db->pass, "'" , NULL);
    else if(action == 'D')
        query = mkString("DROP USER '", db->login, "'@'localhost'", NULL);
    else if(action == 'U')
        query = mkString("UPDATE mysql.user SET ", passColName, " = '",db->pass,
                         "' WHERE User = '",db->login,"' AND host = 'localhost'",  NULL);
    
    if(!mysqlh)
        return 0;
    if(!mysql_query(mysqlh, query)) {
        mysql_query(mysqlh, "flush privileges");
        status = 1;
    }
    
    mysql_close(mysqlh);
    free(query);
    
    return status;
}
int dbgrantsmgr(grants * db, char action, char * os) {
    MYSQL * mysqlh = mysqlconn(os);
    
    if(!mysqlh)
        return 0;
    
    char * grant = mkString("GRANT ", db->privs, " ON ", db->dbname, ".* TO '",
                        db->user, "'@'localhost'", NULL);
    char * revoke = mkString("REVOKE ALL ON ", db->dbname, ".* FROM '", db->user, 
                        "'@'localhost'", NULL);
    char * query = NULL;
    int status = 0;
    
    
    if(action == 'N')
        query = grant;
    else if(action == 'D') {
        if(mysqlUserExist(mysqlh, db->user))
            query = revoke;
        else
            status = 1;
    }
    else if(action == 'U') {
        mysql_query(mysqlh, revoke);
        query = grant;
    }
    if(query) {
        if(!mysql_query(mysqlh, query)) {
            mysql_query(mysqlh, "flush privileges");
            status = 1;
        }
    }
    
    mysql_close(mysqlh);
    free(grant);
    free(revoke);
    
    return status;
}
int mysqlUserExist(MYSQL * dbh, char * login) {
    MYSQL_RES * res = NULL;
    char * query = mkString("SELECT User FROM mysql.user WHERE User = '", login, "'", NULL);
    int status = 0;
    
    if(!mysql_query(dbh, query)) {
        if((res = mysql_store_result(dbh))) {
            if(mysql_num_rows(res))
                status = 1;
            mysql_free_result(res);
        }
    }
    free(query);
    
    return status;
 }
MYSQL * mysqlconn(char * os) {
   MYSQL * db = mysql_init(NULL);
   char * sockpath = NULL;
   
   if(!strcmp(os, "Ubuntu"))
       sockpath = "/var/run/mysqld/mysqld.sock";
   else
       sockpath = "/var/lib/mysql/mysql.sock";
   
   if(mysql_real_connect(db, "localhost", "root", NULL, "mysql" ,0, sockpath,0) != NULL)
      return db;
   else
      return NULL;
}