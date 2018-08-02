#include<iostream>
using namespace std;
#include <string.h>
#include "serever/http_server.h"
#include "serever/Application.h"
//#include "serever/MysqlDB.h"
#include "serever/my_handelr.h"
void Application::Route()
{
    if(path("/test"))
    {
        BaseHandler *base1=new loginHandel;
        set_BaseHandler(base1);
    }
    else if(path("/desgin"))
    {
        BaseHandler *design=new desingnhandel;
        set_BaseHandler(design);
    }
    else if(path("/chat"))
    {
        WebsocketHandelr *wxhander=new chat;
        set_BaseHandler(wxhander);
    }
    else if(path("/chat1"))
    {
        WebsocketHandelr *wxhander=new chat1;
        set_BaseHandler(wxhander);
    }

}
int main()
{
//    cppMysql cppmysql("127.0.0.1","root","wangjing123","newdb",3306);
//    auto ve=cppmysql.Select("select * from wang");
//    for(vector<char *> v:ve)
//    {
//        for(char * v_char:v)
//        {
//            cout<<v_char<<' ';
//        }
//        cout<<endl;
//
//    }
Server_info server_info;
server_info.ip="0.0.0.0";
server_info.port=8800;
Cserver cserver(server_info);
cserver.run();


//    BaseHandler *base;
//    BaseHandler *base1=new loginHandel;
//    base=base1;
//    delete base;
//    if(base!= nullptr)
//    {
//        cout<<"delete base"<<endl;
//        delete base1;
//
//    }
//    Requests re;
//    re.method="GET";
//    re.url="baidu";
//    Application app;
//    app.set_requtest(&re);
//    app.implemen();
//    Application app1;
//    re.url="/desgin";
//    app1.set_requtest(&re);
//    app1.implemen();
//    return 0;
}