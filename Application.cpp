//
// Created by meidai on 18-7-9.
//
#include "RequestHandler.h"
#include "Application.h"
#include <regex>
Application:: Application()
{
    handler=nullptr;
}
Application::~Application()
{
    cout<<"析苟application"<<endl;
    if(handler!=nullptr)
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

    Route();
    if(handler== nullptr)
    {
        cout<<"404"<<endl;
    }
    else if(requests->method=="GET")
    {
        handler->get();
    }
    else if(requests->method=="POST")
    {
        handler->post();
    }
    char *str="404";
    return str;
}


void Application::set_requtest(Requests *requests1)
{
    requests=requests1;

}

void Application::set_BaseHandler(BaseHandler *base) {
    handler = base;
    handler->requests = requests;

}
