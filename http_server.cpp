#include "http_server.h"
#include <unistd.h>
#include <string.h>
#include <regex>
#include<unistd.h>
#include "my_http_parser.h"
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
                        server_recv(event_list[i].data.fd);

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
void Cserver::server_recv(int coon)
{   char req[1024];
    Cparser * parser=new Cparser;
    while(true)
    {

        memset(req,0, sizeof(req));
        int ret=recv(coon,req,1024,0);
        if(ret==-1||ret==0)
        {
            close_socket(coon);
            delete parser;
            return;
        }
        if(parser->get_requests_head(req))
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

        while (true)
        {

            memset(req, 0, 1024);
            int ret = recv(coon, req, 1024, 0);
            if(ret==-1||ret==0)
            {
                close_socket(coon);
                delete parser;
                return;
            }
            if(parser->chunked_analysis(req))
            {

                break;
            }
        }
        parser->requests->body_length-parser->get_first_chunked_size();


    }

    cout<<"****************"<<endl;

    delete parser;
}
void Cserver::run()
{
    init();
    server_bind();
    server_epoll_ctl();
    epoll_while();
}