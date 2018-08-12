#include<iostream>
#include "serever/http_server.h"
#include "serever/Application.h"
#include "myhandel/my_handelr.h"
using namespace std;
#include <string.h>
#include "DB/mysqll++test.h"
#include <mysql/mysql.h>
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



    strut_query();
//Server_info server_info;
//server_info.ip="127.0.0.1";
//server_info.port=8800;
//Cserver cserver(server_info);
//cserver.run();

}