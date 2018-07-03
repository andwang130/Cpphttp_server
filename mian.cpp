#include<iostream>
using namespace std;

#include "http_server.h"
int main()
{
Server_info server_info;
server_info.ip="127.0.0.1";
server_info.port=8000;
Cserver cserver(server_info);
cserver.run();
return 0;
}

