#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<iostream>
#include <map>
#include "RequestHandler.h"
#include "Application.h"
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
    map<int,Application*>websocket_coon;
    sockaddr_in serever_in;
    Server_info server_info;
    Epoll_info epoll_info;
    void recv_websocket(int coon,Application*app);
    void recv_http(int coon,Cparser * parser);
    void Upgrade_websocket(int coon,Cparser * parser);
    void to_http_handel(int coon,Cparser * parser);

public:
    void init();
    void server_bind();
    void server_accept();
    void server_recv(int coon);
    void server_epoll_ctl();
    void epoll_while();
    void close_socket(int coon);
    void run();
    Cserver(Server_info serinfo);
    static int wx_send(string message,int coon);

};