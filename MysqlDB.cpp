//
// Created by meidai on 18-7-16.
//
#include "MysqlDB.h"
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
vector<vector<char *>> cppMysql::Select(char *sql)
{
    vector<vector<char *>> vector_sum;
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
        cout<<endl;
        MYSQL_ROW slq_row;
        vector<char *> vector1;
        while (slq_row=mysql_fetch_row(result))
        {
            for (int i = 0; i <clo; ++i) {
                vector1.push_back(slq_row[i]);
            }
            vector_sum.push_back(vector1);
            
        }

    }
    return vector_sum;


}
int cppMysql::Insert(char *sql)
{
    if(mysql_query(_mysql,sql))
    {
        return -1;
    }
    cout<<"共插入"<<mysql_affected_rows(_mysql)<<"行    "<<endl;
}
int cppMysql::Delete(char *sql)
{
    if(mysql_query(_mysql,sql))
    {
        return -1;
    }
    cout<<"共删除"<<mysql_affected_rows(_mysql)<<"行    "<<endl;
}
int cppMysql::Update(char *sql)
{
    if(mysql_query(_mysql,sql))
    {
        return -1;
    }
    cout<<"共修改"<<mysql_affected_rows(_mysql)<<"行    "<<endl;
}
int cppMysql::transaction() //开始事务
{
    if(mysql_query(_mysql,"START TRANSACTION"))
    {
        return -1;
    }
}
int cppMysql::commit()
{
    if(mysql_query(_mysql,"COMMIT"))
    {
        return -1;
    }
}
int cppMysql::RollBack()
{
    if(mysql_query(_mysql,"ROLLBACK"))
    {
        return -1;
    }
}
