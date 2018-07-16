//
// Created by meidai on 18-7-16.
//

#ifndef UNTITLED5_MYSQLDB_H
#define UNTITLED5_MYSQLDB_H

#endif //UNTITLED5_MYSQLDB_H

#include <mysql/mysql.h>
class cppMysql
{
private:
    MYSQL *_mysql;
    MYSQL_RES *result= NULL;
public:
    cppMysql(const char *host,const char *user, const char *passwd, const char *db, unsigned int port);
    void select(char *sql);
    int create_db(char * dbname);

};