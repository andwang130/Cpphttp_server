#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<iostream>
#include <map>
#include "RequestHandler.h"
#include "Application.h"
#include "ThreadPool.hpp"
using namespace std;
struct Server_info
{
    int port;
    char *ip;

    int server_socket;
};
struct Epoll_info
{
    int epoll_fd;
    int process_num;
    int events_num=10;
};
struct configure{
    int Process_number;
    map<string,BaseHandler> map_handler;

};
class Cserver
{
private:
    ThreadPool *Thpoool;
    sockaddr_in serever_in;
    Server_info server_info;



public:
    static map<int,Application*>websocket_coon;
    static Epoll_info epoll_info;
    void init();
    void server_bind();
    void server_accept();
    void server_epoll_ctl();
    void epoll_while();
    static void close_socket(int coon);
    void run();
    Cserver(Server_info serinfo);
    static int wx_send(string message,int coon);
    static int Cserver_recv(int coon,char *buf,int size);
    static int websokcet_epoll_ctl(int coon);


};