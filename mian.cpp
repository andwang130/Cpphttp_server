#include<iostream>
using namespace std;

#include <string.h>
#include "http_server.h"
#include "RequestHandler.h"
int main()
{
//    http_parser_settings stettings;
//    stettings.on_url=my_on_url;
//    stettings.on_header_field=my_on_headers;
//    http_parser *parser=new http_parser;
//    http_parser_init(parser,HTTP_REQUEST);
//    http_request *request=new_http_request();
//    parser->data=request;
//
//    int lent= strlen(http_get_raw);
//    int npar=http_parser_execute(parser,&stettings,http_get_raw,lent);
//    cout<<npar<<endl;
//Server_info server_info;
//server_info.ip="127.0.0.1";
//server_info.port=8000;
//Cserver cserver(server_info);
//Cserver cserver1=new (cserver);
//cserver.run();
    BaseHandler *base=new loginHandel;
    base->a="sdsds";
    base->get();
    BaseHandler base1=base1;
    base1.a="aaaa";
    cout<<base1.a<<endl;
    base1.get();
return 0;
}

