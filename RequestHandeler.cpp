//
// Created by meidai on 18-7-10.
//
#include"RequestHandler.h"
#include <fstream>
#include "MyException.h"
#include <string.h>
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
string get_boundary(string Content_Type)
{
    string boundary(Content_Type,30,Content_Type.size()-30);
    return boundary;

}

void BaseHandler::get_Content_Disposition(int &len,int x)
{
    string first(requests->body, len + 1, x-len-2);
    //cout<<first<<endl;
    len = x + 4;
    int stara=len;
    for(;len<requests->body.size();len++)
    {
        if(requests->body[len]=='\r'&&requests->body[len+1]=='\n')
        {
            break;
        }

    }
    string content(requests->body,stara,len-stara);
    //cout<<content<<endl;
    len+=2;
    map_data[first]=content;

}
bool BaseHandler::juge_end(int len)
{

    if(string(requests->body,len+2,boundary_size)==boundary)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool BaseHandler::juge_end2(int len)
{
    int num=len+2+boundary_size;
    if(string(requests->body,len+2,boundary_size)==boundary&&requests->body[num]=='\r'&&requests->body[num+1]=='\n')
    {
        return true;
    }
    else
    {
        return false;
    }
}
void BaseHandler::get_files_Disposition(int &len,int x)
{
    string file_key(requests->body,len+1,x-len-2);
    len=x+11;
    int stara=len;
    for(;len<requests->body.size();len++) {
        if (requests->body[len] == '\r' || requests->body[len + 1] == '\n') {
            break;
        }

    }
    string filename(requests->body,stara+1,len-stara-2); //有"加1减1跳过
    len+=2;
    len+=14; //跳过Content-Type:
    stara=len;
    ;
    for(;len<requests->body.size();len++)
    {
        if(requests->body[len]=='\r'||requests->body[len+1]=='\n')
        {
            break;
        }

    }
    string filetype(requests->body,stara,len- stara);
    len+=4;


    stara=len;
    for(;len<requests->body.size();len++)
    {
        if(requests->body[len]=='\r'&&requests->body[len+1]=='\n'&&juge_end(len+2))
        {
            break;
        }
    }
    len+=2;
    string files(requests->body,stara,len-stara-1);
//    cout<<filename<<endl;
//    cout<<filetype<<endl;
//    cout<<files<<endl;
    Fies My_fies;
    My_fies.name=filename;
    My_fies.type=filetype;
    My_fies.data=files;
    My_fies.key=file_key;
    FIES.push_back(My_fies);

}
void BaseHandler::_get_argument__foram_data()
{
    boundary=get_boundary(requests->headers["Content-Type"]);
    boundary_size=boundary.size();
    int body_size=requests->body.size();
    int i=0;
    while (i<requests->body.size())
    {
        if(juge_end2(i)) {
            i += boundary_size + 2 + 37+2;
            for (int x = i; x < body_size; x++) {
                if (requests->body[x] == '\r' && requests->body[x+1] == '\n') {

                    get_Content_Disposition(i,x);
                    break;

                }
                if (requests->body[x] == ';') {
                    get_files_Disposition(i,x);
                    break;


                }

            }
        }
        else
        {
            break;
        }

    }
    data_flage= true;
//    map<string,string>::iterator ite;
//    for(ite=map_data.begin();ite!=map_data.end();ite++)
//    {
//        cout<<ite->first<<endl;
//        cout<<ite->second<<endl;
//    }

}

void BaseHandler::_get_argument_json()
{

}
bool re_form_data(string &type)
{
    string str1(type,0,20);
    if(str1=="multipart/form-data;")
    {
        return true;
    }
    else
    {
        return false;
    }

}
string BaseHandler::get_argument(string key)
{
    //cout<<requests->headers["Content-Type"]<<endl;
    if(data_flage==false&&requests->headers["Content-Type"] == "application/x-www-form-urlencoded") {

        _get_argument_form_urlencoded();
    }
    else if(data_flage==false&&re_form_data(requests->headers["Content-Type"]))
    {
        _get_argument__foram_data();
    }

    return map_data[key];

}
string BaseHandler::get_body()
{
    return requests->body;
}

//*********************************
void BaseHandler::set_headers(string first,string sucode)
{
    reqspone.headers[first]=sucode;
}
void BaseHandler::set_status(string code)
{
    reqspone.status_code=code;
}
void BaseHandler::set_cookies(string key, string val)
{
    reqspone.cookies[key]=val;
}
//***********************************

//**********************************
void BaseHandler::wirte(char *str)
{
    reqspone.body=str;
}
//********************************
void BaseHandler::render(string path)
{
    ifstream fs(path);
    char buff[1024];
    if(!fs.is_open()) {
        cout << "render eero" << endl;
        throw MyException(1024);

    }
    while(!fs.eof())
    {

        memset(buff,0,1024);
        fs.getline(buff,1024);
        reqspone.body+=buff;
    }
    fs.close();

}

//********************************


void BaseHandler::redirect(string url)
{
    reqspone.headers["Location"]=url;
    reqspone.status_code="301";

}
BaseHandler::BaseHandler()
{
    reqspone.headers["Content-Type"]="text/html; charset=UTF-8";
    string Gtm_time=umit::get_time();
    reqspone.headers["Date"]=Gtm_time;

}

void BaseHandler::set_requtest(Requests *requests1)
{
    requests=requests1;
}