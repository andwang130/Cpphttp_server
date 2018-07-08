//
// Created by meidai on 18-7-2.
//
#pragma once
#ifndef UNTITLED5_REQUESTHANDLER_H
#define UNTITLED5_REQUESTHANDLER_H

#endif //UNTITLED5_REQUESTHANDLER_HC

#include <iostream>
#include "my_http_parser.h"
using namespace std;
class BaseHandler
{

protected:
    Requests *requests;

    string get_argument(string key,string Default=NULL);
    string get_body();
    void wirte();
public:
    string a;
    virtual ~BaseHandler()
    {cout << "father 的构造函数" << "\n";}

    BaseHandler()
    {

    }

     BaseHandler(BaseHandler &handler)
    {

//        requests->body=handler.requests->body;
//        requests->body_off=handler.requests->body_off;
//        requests->body_length=handler.requests->body_length;
//        requests->analysis=handler.requests->analysis;
//        requests->url=handler.requests->url;
//        requests->headers=handler.requests->headers;
//        requests->method=handler.requests->method;
//        requests->headers_str=handler.requests->headers_str;
//        requests->version=handler.requests->version;
         //this->a=handler.a;

    }
    virtual void init()
    {

    }

    virtual void post()
    {
        cout<<"basepost"<<endl;
    }

    virtual void get()
    {
        cout<<"base get"<<endl;
    }
};

class loginHandel:public BaseHandler
{
public:

    loginHandel()
    {
        cout<<"loginHandel 构造"<<endl;
    }
//    void get()
//    {
//        cout<<a<<endl;
//
//    }
    void post()
    {
        cout<<"post"<<endl;
    }
    void init()
    {
        cout<<"init"<<endl;
    }
    ~loginHandel()
    {
        cout<<"loginHandel"<<endl;
    }
};