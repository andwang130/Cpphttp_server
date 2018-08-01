//
// Created by meidai on 18-8-1.
//
#include "WebsokcetHandelr.h"
#include "http_server.h"

void WebsocketHandelr::write_message(string message)
{

    Cserver::wx_send(message,wx_requests.coon);
}


 void WebsocketHandelr::open()
{

}
 void WebsocketHandelr::on_message(string message)
{

}
 void WebsocketHandelr::on_close()
{

}
 void WebsocketHandelr::check_origin()
{

}

