//
// Created by meidai on 18-8-1.
//

#ifndef UNTITLED5_MY_HANDELR_H
#define UNTITLED5_MY_HANDELR_H

#include <set>
#include "../serever/umit.h"
#include <unistd.h>
#include "../serever/WebsokcetHandelr.h"
#include "../serever/RequestHandler.h"
class loginHandel:public BaseHandler
{
public:
    static int num;
    loginHandel()
    {
        //cout<<"loginHandel 构造"<<endl;
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
        //num+=1;
        int num=1;
        string strnum=to_string(num);
        cout<<"get_argument"<<get_argument("action")<<endl;
        set_cookies("name","wangjing");
        string req="<h1>这是一个测试页码</h1>\n<div>当前被访问了：";
        req+=strnum+"次</div>";
        wirte((char *)req.c_str());
        //set_status("200");
        //redirect("/desgin");
        //render("/home/meidai/桌面/len.html");
    }
    void init()
    {
        cout<<"init"<<endl;
    }
    ~loginHandel()
    {
        //cout<<"loginHandel 析构"<<endl;

    }
};
int loginHandel::num=0;
class desingnhandel:public BaseHandler
{
public:
    ~desingnhandel()
    {

    }
    void get()
    {   usleep(10000);
        render("/home/wj_de/test.html");

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
    static vector<chat1*> set_handelr;

    ~chat1()
    {

    }
    void on_message(string message)
    {
        cout<<"收到消息："<<message<<endl;
        for(int i=0;i<chat1::set_handelr.size();i++)
        {   if(set_handelr[i]!=this)
            {
                set_handelr[i]->write_message(message);
            }
        }
    }
    void on_close()
    {

    }
    void open()
    {
    chat1::set_handelr.push_back(this);
    }
};
vector<chat1*> chat1::set_handelr;
#endif //UNTITLED5_MY_HANDELR_H