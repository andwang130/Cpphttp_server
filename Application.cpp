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
void Application::implemen()
{

    Route();
    if(handler== nullptr)
    {
        return ;
    }
    else if(requests->method=="GET")
    {
        handler->get();
    }
    else if(requests->method=="POST")
    {
        handler->post();
    }
}
string get_time()
{
    time_t timep;
    time (&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%a, %d %b %Y %H:%M:%S GMT",localtime(&timep) );
    return tmp;
}
string Application::response_body()
{
    string app_response;
    if(handler== nullptr)
    {
        app_response="HTTP/1.1 404 OK\r\n";
        return app_response;
    }
    else
    {

        app_response="HTTP/1.1 "+handler->reqspone.status_code+" OK\r\n";

        app_response+="Content-Type: text/html; charset=UTF-8\r\n";
        string Gtm_time=get_time();
        app_response+="Date: "+Gtm_time+"\r\n";
        map<string,string>::iterator ite;
        for(ite=handler->reqspone.headers.begin();ite!=handler->reqspone.headers.end();ite++)
        {
            app_response+=ite->first+": "+ite->second+"\r\n";
        }
        if(handler->reqspone.body.size()>0)
        {
            string body_size=std::to_string(handler->reqspone.body.size());
            app_response+="Content-Length: "+body_size +"\r\n";
        }
        app_response+="\r\n";
        if(!handler->reqspone.body.empty()) {
            app_response += handler->reqspone.body;
        }


    }
    return app_response;
}

void Application::set_requtest(Requests *requests1)
{
    requests=requests1;

}

void Application::set_BaseHandler(BaseHandler *base) {
    handler = base;
    handler->requests = requests;

}
