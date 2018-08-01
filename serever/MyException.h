//
// Created by meidai on 18-7-16.
//
#pragma once
#ifndef UNTITLED5_MYEXCEPTION_H
#define UNTITLED5_MYEXCEPTION_H

#endif //UNTITLED5_MYEXCEPTION_H
#include "umit.h"
class MyException
{

private:
    int code;
    string exce_str;
public:
    MyException(int _code)
    {
        code=_code;
    }
    void Exception_1024() {
        exce_str = "HTTP/1.1 500 OK\r\n";
        string Gtm_time = umit::get_time();
        exce_str += "Date: " + Gtm_time + "\r\n";
        exce_str += "\r\n";
        exce_str += "render eeor";
    }
    void Exception_404() {
        exce_str="HTTP/1.1 404 OK\r\n";
        string Gtm_time=umit::get_time();
        exce_str+="Date: "+Gtm_time+"\r\n";
        exce_str+="\r\n";
    }
    const char *what()
    {
        switch (code) {
            case 1024:
                Exception_1024();
            return exce_str.c_str();
            case 404:

            default:
                return "default eero";
        }

    }

};