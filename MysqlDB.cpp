//
// Created by meidai on 18-7-16.
//
#include "MysqlDB.h"
#include <iostream>
using namespace std;
cppMysql::cppMysql(const char *host,const char *user, const char *passwd, const char *db, unsigned int port)
{
    _mysql=mysql_init(NULL);  //初始化mysql句柄
    if(_mysql==NULL){
        cout<<"mysql句柄初始化失败"<<endl;
    }
    if(!mysql_real_connect(_mysql,host,user,passwd,db,port,NULL,0))
    {
        cout<<"数据库链接失败"<<endl;
    }


}
void cppMysql::select(char *sql)
{
    if(mysql_query(_mysql,sql))//查询,失败返回非0
    {
        cout<<"查询失败"<<endl;
    }

    result=mysql_store_result(_mysql);
    if(result)
    {
        int row=mysql_num_rows(result);//获取行数
        int clo=mysql_num_fields(result);//获取列数
        MYSQL_FIELD *fd;
        while (fd=mysql_fetch_field(result))
        {
            cout<<fd->name<<'\t';
        }


    }

}
