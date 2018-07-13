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
#include <vector>
using namespace std;
struct Fies{
    string key;
    string name;
    string type;
    string data;
};
struct Response{
    int status_code;
    map<string,string>headers;
    string body;

};
class BaseHandler
{
protected:
    Response reqspone;
    bool data_flage= false;
    map<string,string> map_data;
    vector<Fies> FIES;
    string boundary;
    int boundary_size;
    //***********解析body****************
    void _get_argument__foram_data();
    void _get_argument_json();
    void _get_argument_form_urlencoded();
    void get_Content_Disposition(int &len,int x);
    void get_files_Disposition(int &len,int x);
    bool juge_end(int len);
    string get_argument(string key);
    //************************

    //*********设置reqspone*******
    void set_headers(string first,string sucode);
    void set_status(int code);
    //****************************
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
        //cout<<requests->headers_str<<endl;
       cout<<requests->body<<endl;
      cout<< "get_argument"<<get_argument("action")<<endl;
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