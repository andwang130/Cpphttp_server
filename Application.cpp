//
// Created by meidai on 18-7-9.
//
#include "RequestHandler.h"
#include "Application.h"
#include <regex>
Application::~Application()
{
    if(handler!= nullptr)
    {
        delete handler;
    }

};
bool Application::path(char *url)
    {
        cmatch st;
        regex re_url(url);
        if(regex_match(requests->url.c_str(),st,re_url))
        {
            return true;
        } else
        {
            return false;
        }

    }
char * Application::implemen()
{
    handler=Route();
    handler->requests=requests;
    if(handler== nullptr)
    {
        cout<<"404"<<endl;
    }
    if(requests->method=="GET")
    {
        handler->get();
    }
    else if(requests->method=="POST")
    {
        handler->post();
    }

}


void Application::set_requtest(Requests *requests1)
{
    requests=requests1;

}

