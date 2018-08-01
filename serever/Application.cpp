//
// Created by meidai on 18-7-9.
//
#include "RequestHandler.h"
#include "Application.h"
#include <regex>
#include "umit.h"
Application:: Application()
{
    handler=nullptr;
    ws_handler= nullptr;
}
Application::~Application()
{
    cout<<"析苟application"<<endl;
    if(handler!=nullptr)
    {
        delete handler;
    }
    if(ws_handler!= nullptr)
    {
        delete ws_handler;
    }

};
bool Application::path(char *url)
    {
        cmatch st;
        regex re_url(url);
        if(regex_match(app_Url,st,re_url))
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
    else if(requests->method=="PUT")
    {
        handler->put();
    }
    else if(requests->method=="DELETE")
    {
        handler->Delete();
    }
    else if(requests->method=="CONNECT")
    {
        handler->connect();
    }
    else if(requests->method=="OPTIONS")
    {
        handler->options();
    }
    else if(requests->method=="TRACE")
    {
        handler->trace();
    }
    else if(requests->method=="HEAD")
    {
        handler->head();
    }


}
void Application::ws_implemen(const char *url)
{
    set_Url(url);
    Route();
}
void Application::response_404(string &str)
{
    str="HTTP/1.1 404 OK\r\n";
    string Gtm_time=umit::get_time();
    str+="Date: "+Gtm_time+"\r\n";
    str+="\r\n";
}
string Application::response_body()
{
    string app_response;
    if(handler== nullptr)
    {
        response_404(app_response);
        return app_response;
    }
    else
    {

        app_response="HTTP/1.1 "+handler->reqspone.status_code+" OK\r\n";
        map<string,string>::iterator ite;
        for(ite=handler->reqspone.headers.begin();ite!=handler->reqspone.headers.end();ite++)
        {
            app_response+=ite->first+": "+ite->second+"\r\n";
        }
        for (ite=handler->reqspone.cookies.begin();ite!=handler->reqspone.cookies.end();ite++)
        {
          app_response+="set-cookie: "+ite->second+"\r\n";
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
    app_Url=requests->url.c_str();

}
void Application::set_Url(const char *url)
{
    app_Url=url;

}
void Application::set_BaseHandler(BaseHandler *base) {
    handler = base;
    handler->requests = requests;

}
void Application::set_BaseHandler(WebsocketHandelr *websocket) {
    ws_handler = websocket;
}
