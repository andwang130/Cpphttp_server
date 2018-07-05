#include<iostream>
using namespace std;

#include <string.h>
#include "http_server.h"
#include "http_parser.h"
const char * http_get_raw = "GET /favicon.ico HTTP/1.1\r\n"
                            "Host: 0.0.0.0=5000\r\n"
                            "User-Agent: Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9) Gecko/2008061015 Firefox/3.0\r\n"
                            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                            "Accept-Language: en-us,en;q=0.5\r\n"
                            "Accept-Encoding: gzip,deflate\r\n"
                            "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n"
                            "Keep-Alive: 300\r\n"
                            "Connection: keep-alive\r\n"
                            "\r\n";

struct http_request {
    char *method, *url, *body;
    unsigned int flags;
    unsigned short http_major, http_minor;
    struct http_header *headers;
};
int my_on_url (http_parser* parsea, const char *at, size_t length)
{

  cout<<at<<endl;


}
int my_on_headers(http_parser * parsea,const char *at,size_t length)
{
    cout<<at<<endl;
}

static inline struct http_request *new_http_request() {
    struct http_request *request = new http_request;
    request->headers = NULL;
    request->url = NULL;
    request->body = NULL;
    request->flags = 0;
    request->http_major = 0;
    request->http_minor = 0;
    return request;
}
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
Server_info server_info;
server_info.ip="127.0.0.1";
server_info.port=8000;
Cserver cserver(server_info);
cserver.run();
return 0;
}

