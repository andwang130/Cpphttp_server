//
// Created by meidai on 18-7-6.
//
#pragma once
#ifndef UNTITLED5_MY_HTTP_PARSER_H
#define UNTITLED5_MY_HTTP_PARSER_H

#endif //UNTITLED5_MY_HTTP_PARSER_H

#include <iostream>
#include <map>

using namespace std;
struct Requests
{
    map<string,string> headers;
    string headers_str;
    string body;
    string url;
    string method;
    string version;
    string analysis;
    int Content_Length=0;
    int headers_length=0;
    int body_length=0;
    int body_off=0;
};
class Cparser
{
public:
    Requests *requests;

    bool get_requests_head(char *req);
    void set_body(char *req,int i);
    int get_Content_Length();
    int str_to_int(string str);
    bool Content_Length_analysis(char *req,int ret);
    bool chunked_analysis(char *req);
    int get_first_chunked_size();
    int chunked(char *buf,int len,int &chunked_len);
    Cparser();
    ~Cparser();
};