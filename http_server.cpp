#include"http_server.h"
#include <unistd.h>
#include <string.h>
#include <regex>
void Cserver::init()
{
    //serever_in初始化
    serever_in.sin_family=AF_INET;
    serever_in.sin_port=htons(port);
    serever_in.sin_addr.s_addr=inet_addr(ip);

    //epoll_fd初始化
    epoll_fd=epoll_create1(EPOLL_CLOEXEC);

    //socket初始化
    server_socket=socket(AF_INET,SOCK_STREAM,0);

    int opt=1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); //socket复用
}
void Cserver::server_bind()
{
  if(bind(server_socket,(struct sockaddr *)&serever_in,sizeof(serever_in))==-1)
  {
    cout<<strerror(errno)<<endl;
  }
  if(listen(server_socket,10)==-1)
  {
      cout<<strerror(errno)<<endl;
  }
}
void Cserver::server_epoll_ctl()
{
    struct epoll_event event;
    event.events=EPOLLIN;
    event.data.fd=server_socket;
    epoll_ctl(epoll_fd,EPOLL_CTL_ADD,event.data.fd,&event);
}

void Cserver::epoll_while()
{
    struct epoll_event *event_list=new epoll_event[events_num];
    while(true)
    {
        int ret=epoll_wait(epoll_fd,event_list,events_num,-1);
        if(ret==0)
        {
            continue;
        }
        if(ret==-1)
        {
            cout<<"time out"<<endl;
        }
        else
        {
            for(int i=0;i<ret;i++)
            {
                if(event_list[i].data.fd==server_socket)
                {
                    server_accept();
                }
                else
                {
                    if(event_list[i].events==EPOLLIN)
                    {
                        server_recv();
                    }
                }
            }
        }

    }
}
void Cserver::server_accept()
{
    struct sockaddr_in coo_in;
    socklen_t lent= sizeof(coo_in);
    int con=accept(server_socket,(struct sockaddr *)&coo_in,&lent);
    struct epoll_event event;
    event.data.fd=con;
    event.events=EPOLLIN;
    epoll_ctl(epoll_fd,EPOLL_CTL_ADD,event.data.fd,&event);

}
void Cserver::server_recv(int coon)
{   char req[1024];
    while(true)
    {
        memset(req,0, sizeof(req));
        int ret=recv(coon,req,1024,0);
        if(get_requests_head(req))
        {
            break;
        }
    }
    Content_Length=get_Content_Length();
    if (Content_Length!=0)
    {
        Content_Length_analysis(coon);
    }
    else
    {
        chunked_analysis(coon);
    }
}
void Cserver::Content_Length_analysis(int coon)
{
    char req[1024];
    int ret;
    while(body_length<Content_Length)
    {
        memset(req,0,1024);
        ret=recv(coon,req,1024,0);
        body_length+=ret;
        body+=req;
    }
}
void Cserver::chunked_analysis(int coon)
{

    cmatch st;
    regex chunked("\r\n0\r\n\r\n");
    char req[1024];
    while (true)
    {
        memset(req, 0, 1024);
        recv(coon, req, 1024, 0);
        body += req;
        if (regex_search(req, st, chunked))
        {
            break;
        }

    }

}
bool Cserver::get_requests_head(char *req)
{
    //这个函数用来查找到\r\n\r\n的位置，取到head；
    //
    char flag[5] = "\r\n\r\n";
    int req_lent = sizeof(req);
    int flag_lent = sizeof(flag);
    int i=0;
    bool _return=false;
    for (; i < req_lent; i++)
    {
        int j=0;
        headers+=req[i];
        for (; j< flag_lent; j++)
        {
            if (req[i+j] != flag[j])
            {
                break;
            }

        }
        if (j == flag_lent)
        {
            _return=true;
        }
    }
    headers_length=i+flag_lent;  //headers长度
    if(headers_length<req_lent)
    {

        for(int i=headers_length;i<req_lent;i++)
        {
            body+=req[i];
        }
        body_length=req_lent-headers_length;

    }
    return _return;
}
int Cserver::get_Content_Length()
{
    if(headers!=NULL)
    {
        cmatch st;
        regex length("Content-Length: (\\d+)");

        if (regex_search(headers.c_str(), st, length))
        {
            return str_to_int(st[1]);
        }
        else
        {
            return 0;
        }

    }
}
int Cserver::str_to_int(string str) //字符串转int函数
{
    if (str.empty())
    {
        throw "不能传入一个空的字符串";   //弹出异常
    }
    int Int = 0;
    for (int i = 0; i<str.size(); i++)
    {

        switch (str[i])
        {
            case '0':
                Int = Int * 10 + 0;
                break;
            case '1':
                Int = Int * 10 + 1;
                break;
            case '2':
                Int = Int * 10 + 2;
                break;
            case '3':
                Int = Int * 10 + 3;
                break;
            case '4':
                Int = Int * 10 + 4;
                break;
            case '5':
                Int = Int * 10 + 5;
                break;
            case '6':
                Int = Int * 10 + 6;
                break;
            case '7':
                Int = Int * 10 + 7;
                break;
            case '8':
                Int = Int * 10 + 8;
                break;
            case '9':
                Int = Int * 10 + 9;
                break;

            default:
                throw "无法转换成整型，参数有错误";
        }
    }
    return Int;
}