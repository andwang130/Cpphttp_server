#include<iostream>
using namespace std;

#include <string.h>
#include "http_server.h"
#include "Application.h"
void Application::Route()
{
    if(path("/baidu"))
    {
        BaseHandler *base1=new loginHandel;
        set_BaseHandler(base1);
    }
    else if(path("/desgin"))
    {
        BaseHandler *design=new desingnhandel;
        set_BaseHandler(design);
    }

}
int main()
{

Server_info server_info;
server_info.ip="127.0.0.1";
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