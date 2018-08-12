//
// Created by wj_de on 18-8-11.
//

#ifndef UNTITLED5_CSERVER_H
#define UNTITLED5_CSERVER_H

#include "Application.h"
class CHTTP {
public:
    void server_recv(int coon);
    void recv_websocket(int coon,Application*app);
    bool recv_http(int coon,Cparser * parser);
    void Upgrade_websocket(int coon,Cparser * parser);
    static void to_http_handel(int coon,Cparser * parser);

};


#endif //UNTITLED5_CSERVER_H
