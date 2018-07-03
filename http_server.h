#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<iostream>
using namespace std;
struct Server_info
{
    int port;
    char *ip;

    int server_socket;
};
struct Requests
{
    string headers;
    string body;
    string url;
    int Content_Length=0;
    int headers_length;
    int body_length;
};
struct Epoll_info
{
    int epoll_fd;
    int process_num;
    int events_num=10;
};
class Cserver
{
private:
    sockaddr_in serever_in;
    Server_info server_info;
    Requests requests;
    Epoll_info epoll_info;
public:
    void init();
    void server_bind();
    void server_accept();
    void server_recv(int coon);
    void server_epoll_ctl();
    void epoll_while();
    bool get_requests_head(char *req);
    void set_body(char *req,int i);
    int get_Content_Length();
    int str_to_int(string str);
    void Content_Length_analysis(int coon);
    void chunked_analysis(int coon);
    void close_socket(int coon);
    void run();
    Cserver(Server_info serinfo);

};


