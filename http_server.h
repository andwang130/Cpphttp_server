#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<iostream>
using namespace std;
class Cserver
{
private:
    Cserver();
    int port;
    char *ip;
    int server_socket;
    string headers;
    string body;
    string url;
    sockaddr_in serever_in;
    int epoll_fd;
    int process_num;
    int events_num;
    int Content_Length;
    int headers_length;
    int body_length;
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
    void chunked_analysis(int coon));
};


