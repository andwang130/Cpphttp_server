//
// Created by meidai on 18-7-9.
//
#pragma once
#ifndef UNTITLED5_APPLICATION_H
#define UNTITLED5_APPLICATION_H


#include "RequestHandler.h"
#include "WebsokcetHandelr.h"
class Application
{
private:

    BaseHandler *handler;
    Requests *requests;
    const char *app_Url;
    bool path(char *url);
    void set_BaseHandler(BaseHandler *base);
    void set_BaseHandler(WebsocketHandelr *websocket);
    void Route();
    void response_404(string &str);
public:
    WebsocketHandelr *ws_handler;
    ~Application();
    Application();
    void set_requtest(Requests *requests1);
    void set_Url(const char *url);
    void  implemen();
    void ws_implemen(const char *url);
    string response_body();



};
#endif //UNTITLED5_APPLICATION_H
