//
// Created by wj_de on 18-8-12.
//
#include<mysql++/mysql++.h>
#include <mysql++/ssqls.h>
#include <iostream>
#include <vector>
using namespace std;

/*
 * 编译相关
 * //下载mysql++
 * sudo apt-get install libmysqlclient-dev //安装mysql的so连接库
 执行告诉libmysqlclient的位置 ./configure --prefix=/usr --with-mysql-lib=/usr/lib/x86_64-linux-gnu
 然后make
 make install
         */
//建立连接
mysqlpp::Connection mysql_coon()
{
    //false关闭异常，一个正常程序中建议开启异常处理。
    mysqlpp::Connection coon(false);
    /*
     * bool connect(const char* db = 0, const char* server = 0,
			const char* user = 0, const char* password = 0,
			unsigned int port = 0);
     */
    //参数1，要连接的数据库名称
    //参数2，要连接的ip
    //参数3，用户名
    //参数4，密码
    //参数5，端口
    //返回的是一个bool类型，连接成功为true,反之flase
    if(!coon.connect("testdb","127.0.0.1","root","WANGJING123",3306)) //连接函数
    {
        cout<<"连接失败"<<endl;


    }
    return coon;

}
void ms_qeury()
{
    //query函数，执行增删该查
    //参数1，一个sql的语句
    //返回一个Query类型用来获得sql语句的结果
    mysqlpp::Connection coon=mysql_coon();
    mysqlpp::Query query = coon.query("INSERT INTO stock VALUES(\"WANGJING\",23,\"s\");");
    //语句执行完毕后，可以通过 Query::execute(), Query::store()和 Query::use() 三种类型，来获得返回值

    /*execute()用于不返回数据的查询.例如执行CREATE INDEX，你并没有从服务器获取数据，
     * 程序将返回一个SimpleResult对象，以及一个标识执行成功与否的标识，该对象还包含命令执行所影响的行数，
     * 如果只要成功与否的标识，可以使用Query::exec()，其返回结果为bool.*/

    mysqlpp::SimpleResult result =query.execute();
    //受到影响的行数
    cout<<result.rows()<<endl;
    //自动增量字段的最后一个值
    cout<<result.insert_id()<<endl;

    mysqlpp::Query query1 = coon.query("select * from stock"); //执行一个查询


    //StoreQueryResult,返回查询的数据集合
    mysqlpp::StoreQueryResult store_Result=query1.store();
    cout<<store_Result.num_rows()<<endl; //num_rows行数
    for(int i=0;i<store_Result.num_rows();i++)
    {
        cout<<store_Result[i]["name"]<<endl;
    }
//UseQueryResult编译会报错
//  mysqlpp::UseQueryResult use_Result=query1.use();
//    while (mysqlpp::Row row=use_Result.fetch_row())
//    {
//        cout<<row["name"]<<endl;
//    }

}


//模板查询
void template_qeury()
{
    mysqlpp::Connection coon=mysql_coon();
    mysqlpp::Query query = coon.query("select * from stock where name = %0q");
    query.parse();//开启模板查询
    mysqlpp::StoreQueryResult storeQueryResult=query.store("zhang");
    for(int i=0;i<storeQueryResult.num_rows();i++)
    {
        cout<<storeQueryResult[i]["name"]<<endl;
    }
    query.reset();//query重置,复用一个query
    query<<"UPDATE stock set name=%0q where name=%1q";//新的sql语句
    query.parse();
    mysqlpp::SimpleResult result=query.execute("zhangfei","zhang");
    cout<<result.rows()<<endl;
    //占位符介绍
    //“###” 为最大三位的数字，表示给SQLQueryParms对象的参数序列，从0开始
    //modifier可以是下面任意一个：
    //%打印一个实际的“%”符号
    //“”不管什么，都不要引号或者忽略
    //q将被用单引号括起来，不被作为关键字解释
    //Q将被用引号括起来，不过不会避免关键字检测
    //比如 %0q,第一个参数，这个参数会被双引号括起来
    //%%  实际的%
    //%1q:name
    //
    // 模板查询机制，允许你设置默认参数.你只需要给Query::template_defaultsarray对应元素赋值就可以了
    //query.template_defaults[1] = "zhang";
    //query.template_defaults["name"] = "zhangfei";
}
/*专用SQL结构
//假如你有如下SQL表
CREATE TABLE stock (name varchar(30),age int,sex char(1))
*/

//description MEDIUMTEXT NULL)
//        你可以定义一个对应的C++结构如下
//

//sql_create_#是ssqls.h头文件定义的一个宏
//sql_create_3有3个字段
sql_create_3(
        stock,1,3,
mysqlpp::sql_varchar,name,  //数据库对于的类型
mysqlpp::sql_int,age,
mysqlpp::sql_char,sex
);
void strut_query()
{
    mysqlpp::Connection coon=mysql_coon();
    mysqlpp::Query query=coon.query("select * from stock");
    vector<stock> res;
    query.storein(res);
    for(auto i=res.begin();i!=res.end();i++)
    {
        cout<<"name:"<<i->name<<"age:"<<i->age<<"sex:"<<i->sex<<endl;
    }



}
