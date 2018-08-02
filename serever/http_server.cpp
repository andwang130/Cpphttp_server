#include "http_server.h"
#include <unistd.h>
#include <string.h>
#include <regex>
#include<unistd.h>
#include "my_http_parser.h"
#include "Application.h"
#include<sys/time.h>
#include "MyException.h"
#include "SHA1.cpp"
#include "base64.h"
#include "my_websocket_parser.h"
int64_t getCurrentTime()      //直接调用这个函数就行了，返回值最好是int64_t，long long应该也可以
{
    struct timeval tv;
    gettimeofday(&tv,NULL);    //该函数在sys/time.h头文件中
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
Cserver::Cserver(Server_info serinfo)
{
    server_info.ip=serinfo.ip;
    server_info.port=serinfo.port;


}
void Cserver::init()
{
    //serever_in初始化
    serever_in.sin_family=AF_INET;
    serever_in.sin_port=htons(server_info.port);
    serever_in.sin_addr.s_addr=inet_addr(server_info.ip);

    //epoll_fd初始化
    epoll_info.epoll_fd=epoll_create1(EPOLL_CLOEXEC);

    //socket初始化
    server_info.server_socket=socket(AF_INET,SOCK_STREAM,0);

    int opt=1;
    setsockopt(server_info.server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); //socket复用
}
void Cserver::server_bind()
{
  if(bind(server_info.server_socket,(struct sockaddr *)&serever_in,sizeof(serever_in))==-1)
  {
    cout<<"bind"<<strerror(errno)<<endl;
  }
  if(listen(server_info.server_socket,10)==-1)
  {
      cout<<strerror(errno)<<endl;
  }
}
void Cserver::server_epoll_ctl()
{
    struct epoll_event event;
    event.events=EPOLLIN;
    event.data.fd=server_info.server_socket;
    epoll_ctl(epoll_info.epoll_fd,EPOLL_CTL_ADD,server_info.server_socket,&event);
}

/**
 +------------------------------------------------------------------------------
 * @name epoll_while
 +------------------------------------------------------------------------------
 * @desc        	: 开启eopll的循环函数
 *                  :判断epoll返回的sokcet连接是客户端还是服务端
 *                  :服务端链接进入server_accept函数建立一个
 *                  ;客户端端来判断是否可读，如果可读在判断是HTPP连接还是websokcet
 *                  :websoket通过当前scoket适合存在于websocket_coon中
 +------------------------------------------------------------------------------
 * @author	      	: wangjing
 * @since       	: 18-6-11
 * @return      	: void
 +------------------------------------------------------------------------------
**/
void Cserver::epoll_while()
{

    struct epoll_event *event_list=new epoll_event[epoll_info.events_num];
    while(true)
    {
        int ret=epoll_wait(epoll_info.epoll_fd,event_list,epoll_info.events_num,-1);
        if(ret==0)
        {
            continue;
        }
        if(ret==-1)
        {
            cout<<ret<<endl;
            cout<<"time out"<<endl;
        }
        else
        {
            for(int i=0;i<ret;i++)
            {
                if(event_list[i].data.fd==server_info.server_socket)
                {
                    server_accept();
                }
                else
                {
                    if(event_list[i].events==EPOLLIN)
                    {
                        map<int,Application*>::iterator ite=websocket_coon.find(event_list[i].data.fd);
                        if(ite!=websocket_coon.end())
                        {
                            recv_websocket(event_list[i].data.fd,ite->second);
                        }
                        else
                            {
                                server_recv(event_list[i].data.fd);
                        }


                    }
                    else
                    {
                     close_socket(event_list[i].data.fd);
                    }

                }
            }
        }

    }
}
void Cserver::close_socket(int coon)
{
    struct epoll_event event;
    event.data.fd=coon;
    event.events=EPOLLIN;
    epoll_ctl(epoll_info.epoll_fd,EPOLL_CTL_DEL,event.data.fd,&event);

    map<int,Application*>::iterator ite;
    ite=websocket_coon.find(coon);
    if(ite!=websocket_coon.end())
    {   delete ite->second;
        websocket_coon.erase(ite);
    }
    close(coon);
}
void Cserver::server_accept()
{
    struct sockaddr_in coo_in;
    socklen_t lent= sizeof(coo_in);
    int con=accept(server_info.server_socket,(struct sockaddr *)&coo_in,&lent);
    struct epoll_event event;
    event.data.fd=con;
    event.events=EPOLLIN;
    epoll_ctl(epoll_info.epoll_fd,EPOLL_CTL_ADD,event.data.fd,&event);
    cout<<con<<"建立链接"<<endl;

}
/**
 +------------------------------------------------------------------------------
 * @name recv_websocket
 +------------------------------------------------------------------------------
 * @desc        	: 读取websocket数据
 +------------------------------------------------------------------------------
 * @author	      	: wangjing
 * @param           : 链接的套节字
 * @param           :该链接对应的app
 * @since       	: 18-6-11
 * @return      	: void
 +------------------------------------------------------------------------------
**/

void Cserver::recv_websocket(int coon,Application *app)
{
    char req[2048];
    int ret=recv(coon,req,1024,0);
    if(ret==-1||ret==0)
    {
        close_socket(coon);
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
        close_socket(coon);
    }



    delete container;
}
void Cserver::recv_http(int coon,Cparser * parser)
{


    char req[1024];
    while(true)
    {

        memset(req,0, sizeof(req));
        int ret=recv(coon,req,1024,0);
        string str=string(req,0,ret);
        if(ret==-1||ret==0)
        {
            close_socket(coon);
            delete parser;
            return;
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
                close_socket(coon);
                delete parser;
                return;
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
                close_socket(coon);
                delete parser;
                return;
            }
            if(parser->chunked_analysis(req,ret))
            {

                break;
            }
        }
        parser->requests->body_length-parser->get_first_chunked_size();

    }
}
void Cserver::Upgrade_websocket(int coon,Cparser * parser)
{
    /*升级为websokcet协议
     *
     */
    Application *application=new Application;

    application->ws_implemen(parser->requests->url.c_str());
    if(application->ws_handler== nullptr)
    {
        close_socket(coon);
        return;
    }
    string web_socket_key=parser->requests->headers["Sec-WebSocket-Key"];
    web_socket_key+="258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    string Upgrade_respon="HTTP/1.1 101 Switching Protocols\r\n";
    Upgrade_respon+="Connection: Upgrade\r\n";
    Upgrade_respon+="Upgrade: WebSocket\r\n";
    cout<<web_socket_key<<endl;
    CSHA1 sha1;
    sha1.Update((unsigned char*)web_socket_key.c_str(),strlen(web_socket_key.c_str()));
    sha1.Final();
    unsigned char chSha1[20] = "";
    sha1.GetHash(chSha1);
    Base64 base64;
    string req_web_socket_key=base64.Encode(chSha1,20);
    cout<<req_web_socket_key<<endl;
    Upgrade_respon+="Sec-WebSocket-Accept: "+req_web_socket_key+"\r\n";
    Upgrade_respon+="Sec-WebSocket-Version: 13\r\n";
    Upgrade_respon+="\r\n";
    if(send(coon,Upgrade_respon.c_str(),Upgrade_respon.size(),0)==-1)
    {
        cout<<"send"<<strerror(errno)<<endl;
        close_socket(coon);
    }
    application->ws_handler->wx_requests.coon=coon;
    application->ws_handler->wx_requests.url=parser->requests->url;
    websocket_coon[coon]=application;
    application->ws_handler->open();

}
void Cserver::to_http_handel(int coon,Cparser * parser)
{
    cout<<"****************"<<endl;
    cout<<parser->requests->url<<endl;
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


    cout<<resp<<endl;
    if(send(coon,resp.c_str(),resp.size(),0)==-1)
    {   cout<<"send"<<strerror(errno)<<endl;
        close_socket(coon);
    }

    delete app;
    close_socket(coon);
}
void Cserver::server_recv(int coon)
{   int64_t stara_time=getCurrentTime();
    Cparser * parser=new Cparser;
    recv_http(coon,parser);

    cout<<"Connection:"<<parser->requests->headers["Connection"]<<endl;
    if(parser->requests->headers["Connection"]=="Upgrade"||parser->requests->headers["Connection"]=="keep-alive, Upgrade") //协议升级为websocket
    {
        if(parser->requests->headers["Upgrade"]=="websocket")
        {

            Upgrade_websocket(coon, parser);
        }
    }
    else
    {
        to_http_handel(coon,parser);
    }
    delete parser;
    int64_t end_times=getCurrentTime();
    cout<<end_times-stara_time<<endl;
    cout<<end_times<<endl;

}
void Cserver::run()
{
    init();
    server_bind();
    server_epoll_ctl();
    epoll_while();
}
int Cserver::wx_send(string message,int coon)
{
    unsigned  char* oupt=new unsigned char;
    size_t slen = 0;
    if(packData((unsigned char *)message.c_str(),message.size()+1,0x1,0x1,0,&oupt,&slen)==0)
    {
        if(send(coon,oupt,slen,0)==-1)
        {

            return 1;
        }
    } else
    {
        return 1;
    }
    delete oupt;
    return 0;

}