//
// Created by meidai on 18-7-10.
//
#include"RequestHandler.h"

void BaseHandler::_get_argument_form_urlencoded()
{
    int size=requests->body.size();
    int flag=0;
    for(int i=0;i<size;i++)
    {
        if(requests->body[i]=='&')
        {
         string str(requests->body,flag,i);
         flag=i+1;
         int str_size=str.size();
         for(int j=0;j<str_size;j++)
         {
             if(str[j]=='=')
             {
                 string first(str,0,j);
                 string end(str,j+1,size+1);
                 map_data[first]=end;
                 break;
             }
         }
        }
    }
    string back_str(requests->body,flag,size+1);
    int back_str_size=back_str.size();
    for(int j=0;j<back_str_size;j++)
    {
        if(back_str[j]=='=')
        {
            string first(back_str,0,j);
            string end(back_str,j+1,back_str_size+1);

            map_data[first]=end;
            break;
        }
    }
    data_flage= true;
//        map<string,string>::iterator ite;
//    for(ite=map_data.begin();ite!=map_data.end();ite++)
//    {
//       cout<<ite->first<<endl;
//    }
}
void BaseHandler::_get_argument__foram_data()
{

}
void BaseHandler::_get_argument_json()
{

}

string BaseHandler::get_argument(string key)
{
    cout<<requests->headers["Content-Type"]<<endl;
    if(data_flage==false)
    {
        if (requests->headers["Content-Type"] == "application/x-www-form-urlencoded")
        {
             _get_argument_form_urlencoded();
        } else if (requests->headers["Content-Type"] == "application/json")
        {

        } else if (requests->headers["Content-Type"] == "multipart/form-data")
        {

        } else if (requests->headers["Content-Type"] == "text/xml") {

        }
    }
    return map_data[key];

}