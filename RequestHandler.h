//
// Created by meidai on 18-7-2.
//
#pragma once
#ifndef UNTITLED5_REQUESTHANDLER_H
#define UNTITLED5_REQUESTHANDLER_H

#endif //UNTITLED5_REQUESTHANDLER_HC

#include <iostream>
#include "my_http_parser.h"
#include <map>
using namespace std;
class BaseHandler
{

protected:
    bool data_flage= false;
    map<string,string> map_data;
    void _get_argument__foram_data();
    void _get_argument_json();
    void _get_argument_form_urlencoded();
    string get_argument(string key);
    string get_body();
    void wirte();

public:
    Requests *requests;
    void set_requtest(Requests *requests1)
    {
        requests=requests1;
    }
    string a;
    virtual ~BaseHandler()
    {

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

    void post()
    {
        cout<<requests->headers_str<<endl;
       cout<<requests->body<<endl;
      cout<< get_argument("sdsdsd")<<endl;
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
        cout<<requests->body<<endl;
    }


};