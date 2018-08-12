#include "http_server.h"
#include <unistd.h>
#include <string.h>
#include <regex>
#include<unistd.h>
#include "my_http_parser.h"
#include "Application.h"
#include<sys/time.h>
#include "MyException.h"
#include "my_websocket_parser.h"
#include <mutex>
#include "CHTTP.h"
#include <signal.h> //信号处理
map<int,Application*> Cserver::websocket_coon;
Epoll_info Cserver::epoll_info;
mutex Mutex;
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

                                CHTTP http;
                                http.recv_websocket(event_list[i].data.fd,ite->second);

                        }
                        else
                            //这里范了一个错误，lamdbam函数使用了一个引用
                            {

                                CHTTP http;
                                http.server_recv(event_list[i].data.fd);
                        }


                    }
                    else
                    {
                        Cserver::close_socket(event_list[i].data.fd);
                    }

                }
            }
        }

    }
}
/**
 +------------------------------------------------------------------------------
 * @name close_socket
 +------------------------------------------------------------------------------
 * @desc        	: 关闭套接字，移除epool监听事件，如果该套接字存在websocket中，也删除map中的
 +------------------------------------------------------------------------------
 * @参数1	      	: 一个socket
 * @author	      	: wangjing
 * @since       	: 18-6-11
 * @return      	: void
 +------------------------------------------------------------------------------
**/
void my_epoll_ctl(int coon,int CTL)
{
    Mutex.lock();//加锁
    int myEPOLL_CTL_DEL;
    struct epoll_event event;
    event.data.fd=coon;
    event.events=EPOLLIN|EPOLLET;
    if(CTL==EPOLL_CTL_DEL)
    {
        myEPOLL_CTL_DEL=EPOLL_CTL_DEL;
    }
    else if(CTL==EPOLL_CTL_ADD)
    {
        myEPOLL_CTL_DEL=EPOLL_CTL_ADD;
    }

    epoll_ctl(Cserver::epoll_info.epoll_fd,myEPOLL_CTL_DEL,event.data.fd,&event);
    Mutex.unlock();//解锁
}
void Cserver::close_socket(int coon)
{


    my_epoll_ctl(coon,EPOLL_CTL_DEL);
    Mutex.lock();//加锁
    map<int,Application*>::iterator ite;
    ite=Cserver::websocket_coon.find(coon);
    if(ite!=Cserver::websocket_coon.end())
    {   delete ite->second;
        Cserver::websocket_coon.erase(ite);
    }
    close(coon);
    Mutex.unlock();//解锁
}

/**
 +------------------------------------------------------------------------------
 * @name server_accept
 +------------------------------------------------------------------------------
 * @desc        	: 读取建立连接，将新的socket添加到epool轮询
 +------------------------------------------------------------------------------
 * @author	      	: wangjing
 * @since       	: 18-6-11
 * @return      	: void
 +------------------------------------------------------------------------------
**/
void Cserver::server_accept()
{
    struct sockaddr_in coo_in;
    socklen_t lent= sizeof(coo_in);
    int con=accept(server_info.server_socket,(struct sockaddr *)&coo_in,&lent);
    my_epoll_ctl(con,EPOLL_CTL_ADD);

    //EPOLLIN水平触发。EPOLLET边缘触发，默认是水平触发
    //水平触发，每次有可读事件都会触发
    //边缘触发，只会触发一次，后面要继续监听需要在添加到epool事件中
    //这里使用线程池所有使用边缘触发


    //cout<<con<<"建立链接"<<endl;

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

/**
 +------------------------------------------------------------------------------
 * @name run
 +------------------------------------------------------------------------------
 * @desc        	: 启动服务器
 +------------------------------------------------------------------------------
 * @author	      	: wangjing
 * @param           : 链接的套节字
 * @param           :该链接对应的app
 * @since       	: 18-6-11
 * @return      	: void
 +------------------------------------------------------------------------------
**/
void recvSignal(int sig)
{
    printf("received signal %d !!!\n",sig);
}
void Cserver::run()
{

    init(); //初始化
    server_bind(); //绑定ip
    server_epoll_ctl(); //epool初始化
    epoll_while(); //开启epool轮询
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
int Cserver::Cserver_recv(int coon,char *buf,int size)
{
    return recv(coon,buf,size,0);

}
/**
 +------------------------------------------------------------------------------
 * @name websokcet_epoll_ctl
 +------------------------------------------------------------------------------
 * @desc        	: websocket是长连接，因为使用的是边触发，所以要把socket重新加入epool监听队列
 +------------------------------------------------------------------------------
 * @author	      	: wangjing
 * @param           : 链接的套节字
 * @since       	: 18-6-11
 * @return      	: int
 +------------------------------------------------------------------------------
**/
int Cserver::websokcet_epoll_ctl(int coon)
{
    //EPOLLIN水平触发。EPOLLET边缘触发，默认是水平触发
    //水平触发，每次有可读事件都会触发
    //边缘触发，只会触发一次，后面要继续监听需要在添加到epool事件中
    //这里使用线程池所有使用边缘触发
    my_epoll_ctl(coon,EPOLL_CTL_ADD);
    return 0;
}