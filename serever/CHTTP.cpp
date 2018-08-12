//
// Created by wj_de on 18-8-11.
//

#include "CHTTP.h"
#include<sys/time.h>
#include "http_server.h"
#include "my_websocket_parser.h"
#include <string.h>
#include "SHA1.cpp"
#include "base64.h"
#include "MyException.h"
ThreadPool Thpoool(10);
int64_t getCurrentTime()      //直接调用这个函数就行了，返回值最好是int64_t，long long应该也可以
{
    struct timeval tv;
    gettimeofday(&tv,NULL);    //该函数在sys/time.h头文件中
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
void CHTTP::recv_websocket(int coon,Application *app)
{ Thpoool.enqueue([=]{
    char req[1024];
    memset(req,0, sizeof(req));
    int ret=recv(coon,req,1024,0);
    if(ret==-1||ret==0)
    {
        Cserver::close_socket(coon);
        return;
    }

    WSHeader header;
    parsePack((unsigned char*)req,2048, &header);
    int reallength=header.reallength + 1;
    unsigned char* container = new unsigned char[reallength];
    getPackPayloadData(coon, (unsigned char*)req, 2048, container, &header);
    string str=string((char *)container,0,reallength);
    if(header.mark.opcode==0x1)
    {
        app->ws_handler->on_message(str);
    } else if(header.mark.opcode==0x8)  //opcode为关闭
    {
        app->ws_handler->on_close();
        Cserver::close_socket(coon);
    }
    Cserver::websokcet_epoll_ctl(coon);//重新加入epool轮询
    delete container;
    });
}
void CHTTP::server_recv(int coon)
{   int64_t stara_time=getCurrentTime();
    Cparser * parser=new Cparser;
    if(recv_http(coon,parser))
    {
        //cout<<"Connection:"<<parser->requests->headers["Connection"]<<endl;
        if (parser->requests->headers["Connection"] == "Upgrade" ||
            parser->requests->headers["Connection"] == "keep-alive, Upgrade") //协议升级为websocket
        {
            if (parser->requests->headers["Upgrade"] == "websocket") {

                Upgrade_websocket(coon, parser);
            }
        } else {


            Thpoool.enqueue([=] {
                CHTTP::to_http_handel(coon, parser);
            });

        }

        int64_t end_times = getCurrentTime();
        cout << end_times - stara_time << endl;
    }

}

bool CHTTP::recv_http(int coon,Cparser * parser)
{


    char req[1024];
    while(true)
    {

        memset(req,0, sizeof(req));
        int ret=recv(coon,req,1024,0);
        string str=string(req,0,ret);
        if(ret==-1||ret==0)
        {
            Cserver::close_socket(coon);
            delete parser;
            return false;
        }
        if(parser->get_requests_head(str,ret))
        {
            break;
        }

    }



    if (parser->requests->body_length<parser->requests->Content_Length&&parser->requests->analysis=="content-length")
    {
        while (true) {
            memset(req,0,1024);
            int ret=recv(coon,req,1024,0);
            if(ret==-1||ret==0)
            {
                Cserver::close_socket(coon);
                delete parser;
                return false;
            }

            if(parser->Content_Length_analysis(req,ret))
            {
                break;
            }
        }
    }
    if(parser->requests->analysis=="Transfer-Encoding")
    {
        parser->chunked_analysis(req,0);
        while (parser->requests->body_off!=1)
        {

            memset(req, 0, 1024);
            int ret = recv(coon, req, 1024, 0);
            if(ret==-1||ret==0)
            {
                Cserver::close_socket(coon);
                delete parser;
                return false;
            }
            if(parser->chunked_analysis(req,ret))
            {

                break;
            }
        }
        parser->requests->body_length-parser->get_first_chunked_size();

    }
    return  true;
}
void CHTTP::Upgrade_websocket(int coon,Cparser * parser)
{
    /*升级为websokcet协议
     *
     */
    Application *application=new Application;

    application->ws_implemen(parser->requests->url.c_str());
    if(application->ws_handler== nullptr)
    {
        Cserver::close_socket(coon);
        return;
    }
    string web_socket_key=parser->requests->headers["Sec-WebSocket-Key"];
    web_socket_key+="258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    string Upgrade_respon="HTTP/1.1 101 Switching Protocols\r\n";
    Upgrade_respon+="Connection: Upgrade\r\n";
    Upgrade_respon+="Upgrade: WebSocket\r\n";
    CSHA1 sha1;
    sha1.Update((unsigned char*)web_socket_key.c_str(),strlen(web_socket_key.c_str()));
    sha1.Final();
    unsigned char chSha1[20] = "";
    sha1.GetHash(chSha1);
    Base64 base64;
    string req_web_socket_key=base64.Encode(chSha1,20);
    Upgrade_respon+="Sec-WebSocket-Accept: "+req_web_socket_key+"\r\n";
    Upgrade_respon+="Sec-WebSocket-Version: 13\r\n";
    Upgrade_respon+="\r\n";
    if(send(coon,Upgrade_respon.c_str(),Upgrade_respon.size(),0)==-1)
    {
        cout<<"send"<<strerror(errno)<<endl;
        Cserver::close_socket(coon);
    }
    application->ws_handler->wx_requests.coon=coon;
    application->ws_handler->wx_requests.url=parser->requests->url;
    Cserver::websocket_coon[coon]=application;
    application->ws_handler->open();
    delete parser;
    Cserver::websokcet_epoll_ctl(coon);//重新加入epool轮询

}
void CHTTP::to_http_handel(int coon,Cparser * parser)
{
    //cout<<parser->requests->url<<endl;
    string resp;
    Application *app = new Application;
    try {
        app->set_requtest(parser->requests);
        app->implemen();
        resp = app->response_body();
    }
    catch (MyException & e)
    {
        resp=e.what();
    }
    catch (...)
    {
        resp="HTTP/1.1 500 OK";

    }


   //cout<<resp<<endl;
    if(send(coon,resp.c_str(),resp.size(),0)==-1)
    {   cout<<"send"<<strerror(errno)<<endl;
        Cserver::close_socket(coon);
    }

    delete app;
    delete parser;
    Cserver::close_socket(coon);
}