//
// Created by meidai on 18-7-2.
//
#pragma once
#ifndef UNTITLED5_REQUESTHANDLER_H
#define UNTITLED5_REQUESTHANDLER_H

#endif //UNTITLED5_REQUESTHANDLER_HC

#include <mysql/mysql.h>
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
    string status_code="200";
    map<string,string>headers;
    map<string,string>cookies;
    string body;

};
class BaseHandler
{
protected:

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
    bool juge_end2(int len);
    string get_argument(string key);
    //************************

    //*********设置reqspone*******
    void set_headers(string first,string sucode);
    void set_status(string code);
    void set_cookies(string key,string val);
    //****************************

    //**************render******
    void render(string path);
    //**************************

    void redirect(string url);
    string get_body();
    void wirte(char *str);

public:
    Requests *requests;
    Response reqspone;
    BaseHandler();
    void set_requtest(Requests *requests1);

    virtual ~BaseHandler()
    {
    cout<<"执行BaseHandler虚构"<<endl;
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
    virtual void put()
    {

    }
    virtual void Delete()
    {

    }
    virtual void connect()
    {

    }
    virtual void trace()
    {

    }
    virtual void options()
    {

    }
    virtual void head()
    {

    }


};

