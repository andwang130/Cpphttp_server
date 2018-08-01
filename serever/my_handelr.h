//
// Created by meidai on 18-8-1.
//

#ifndef UNTITLED5_MY_HANDELR_H
#define UNTITLED5_MY_HANDELR_H



#include "WebsokcetHandelr.h"
#include "RequestHandler.h"
class loginHandel:public BaseHandler
{
public:

    loginHandel()
    {
        cout<<"loginHandel 构造"<<endl;
    }

    void post()
    {
        //cout<<requests->headers_str<<endl;
        //cout<<requests->body<<endl;
        cout<<"get_argument"<<get_argument("action")<<endl;
        set_status("200");
        wirte("wwwwwwwwwwwwwwwwwww");
    }
    void get()
    {
        cout<<"get_argument"<<get_argument("action")<<endl;
        set_cookies("name","wangjing");
        //set_status("200");
        //redirect("/desgin");
        //render("/home/meidai/桌面/len.htmdl");
    }
    void init()
    {
        cout<<"init"<<endl;
    }
    ~loginHandel()
    {
        cout<<"loginHandel 析构"<<endl;

    }
};
class desingnhandel:public BaseHandler
{
public:
    ~desingnhandel()
    {

    }
    void get()
    {
        render("/home/meidai/桌面/len.htmdl");
    }


};
class chat:public WebsocketHandelr
{
public:
    ~chat()
    {

    }
    void on_message(string message)
    {
        cout<<"收到消息："<<message<<endl;
        write_message("你好");
    }
};

class chat1:public WebsocketHandelr
{
public:
    ~chat1()
    {

    }
    void on_message(string message)
    {
        cout<<"收到消息："<<message<<endl;
        write_message("chat 1");
    }
};

#endif //UNTITLED5_MY_HANDELR_H