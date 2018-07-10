//
// Created by meidai on 18-7-6.
//
#include "my_http_parser.h"
#include <regex>
#include <math.h>
Cparser::Cparser()
{
    requests=new Requests;
}
Cparser::~Cparser()
{
    if(requests!= nullptr)
    {
        cout<<"Cparser 析构"<<endl;
        delete requests;
    }
}
bool Cparser::Content_Length_analysis(char *req,int ret)
{


    requests->body_length+=ret;
    requests->body+=string(req,0,ret);
    if(requests->body_length>=requests->Content_Length)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Cparser::chunked_analysis(char *req,int ret)
{

    cmatch st;
    regex chunked("\r\n0\r\n\r\n");
    if(ret>0)
    {
        requests->body += string(req, 0,ret);
    }
    if(regex_search(req, st, chunked))
    {
        requests->body_off=1;
        return true;
    }
    else
    {
        return false;
    }

}

int  hex2num(string s) {
    int count = s.length();
    int sum = 0;
    for (int i = count - 1; i >= 0; i--)//从十六进制个位开始，每位都转换成十进制
    {
        if (s[i] >= '0' && s[i] <= '9')//数字字符的转换
        {
            sum += (s[i] - 48) * pow(16, count - i - 1);
        } else if (s[i] >= 'A' && s[i] <= 'F')//字母字符的转换
        {
            sum += (s[i] - 55) * pow(16, count - i - 1);
        }
        else if (s[i] >= 'a' && s[i] <= 'f')//字母字符的转换
        {
            sum += (s[i] - 87) * pow(16, count - i - 1);
        }
    }
    return sum;
}
int Cparser::get_first_chunked_size()
{
        string str;
        int stara=0;
        int end=0;
        int chunked_len=0;
        int chunk_size=0;
        while (true) {

             chunk_size = chunked((char *) requests->body.c_str(), chunk_size, chunked_len);
            if(chunk_size==0)
            {
                requests->body_off=1; //body_off接受完毕
                break;
            }
            end+=chunk_size+stara+chunked_len;
            if(end>requests->body_length)
            {
                break;
            }

//            cout<<"chunk_size"<<chunk_size<<endl;
//            cout<<"end"<<end<<endl;
//            cout<<"chunked_len"<<chunked_len<<endl;
//            cout<<requests->body_length<<endl;

            string _str;
            str += string(requests->body,stara+chunked_len,chunk_size);
            cout<<strlen(str.c_str())<<endl;
            stara=end;
        }

        requests->body=str;
    return chunk_size;


}
int Cparser::chunked(char *buf,int len,int &chunked_len)
{
        string str;
        chunked_len=0;
        for(int i=len;i<7;i++)
        {

            if((buf[i] >= 'a' && buf[i] <= 'f')||(buf[i]>= 'A' &&buf[i] <= 'F')||(buf[i] >= '0' && buf[i] <= '9'))
            {
                str+=requests->body[i];
                chunked_len++;
            }
            else
            {
                break;
            }
        }

        chunked_len+=2;
        return hex2num(str);



}

bool Cparser::get_requests_head(string &req,int ret)
{
    //这个函数用来查找到\r\n\r\n的位置，取到head；
    //
    char flag[5] = "\r\n\r\n";
    int req_lent = ret;
    int flag_lent = strlen(flag);
    int i=0;
    bool _return=false;
    //取第一行，拿到请求类型，htpp版本
    for(;i<req_lent;i++)
    {
        if (req[i] == '\r' && req[i + 1] == '\n')
        {
            break;
        }
    }
    string first_head=string(req,0,i);
    cmatch st;
    regex regex_head("(\\S+) (\\S+) (\\S+)");
    if(regex_match(first_head.c_str(),st,regex_head))
    {
        requests->method=st[1];
        requests->url=st[2];
        requests->version=st[3];

    }



    i+=2; //跳过/r/n
    int start=i;
    regex regex_headers("(\\S+): (.*?)");
    for (; i < req_lent; i++)
    {
        requests->headers_str+=req[i];
        if(req[i]=='\r'&&req[i+1]=='\n')
        {

            string str=string(req,start,i-start);
            //正则匹配headers
            if(regex_match(str.c_str(),st,regex_headers))
            {
                requests->headers[st[1]]=st[2];  //将headers添加到map当中

            }

            start=i+2;

        }
        if(req[i]=='\r'&&req[i+1]=='\n'&&req[i+2]=='\r'&&req[i+3]=='\n')
        {

            _return=true;
            break;

        }
    }
    requests->Content_Length=get_Content_Length();
    //cout<<requests->headers_str<<endl;
    requests->headers_length=i+flag_lent;  //headers长度
    if(requests->headers_length<req_lent)
    {



        requests->body=string(req,requests->headers_length,req_lent-requests->headers_length);

        requests->body_length=req_lent-requests->headers_length;

    }
    return _return;
}
int Cparser::get_Content_Length()
{
    if(requests->headers.find("content-length")!=requests->headers.end())
    {


        requests->analysis="content-length";
        return  str_to_int(requests->headers["content-length"]);



    }
    else
    {
        if(requests->headers.find("Transfer-Encoding")!=requests->headers.end())
        {
            requests->analysis="Transfer-Encoding";
        } else
        {
            requests->analysis="null";
        }
    }
    return 0;
}

int Cparser::str_to_int(string str) //字符串转int函数
{
    if (str.empty())
    {
        throw "不能传入一个空的字符串";   //弹出异常
    }
    int Int = 0;
    for (int i = 0; i<str.size(); i++)
    {

        switch (str[i])
        {
            case '0':
                Int = Int * 10 + 0;
                break;
            case '1':
                Int = Int * 10 + 1;
                break;
            case '2':
                Int = Int * 10 + 2;
                break;
            case '3':
                Int = Int * 10 + 3;
                break;
            case '4':
                Int = Int * 10 + 4;
                break;
            case '5':
                Int = Int * 10 + 5;
                break;
            case '6':
                Int = Int * 10 + 6;
                break;
            case '7':
                Int = Int * 10 + 7;
                break;
            case '8':
                Int = Int * 10 + 8;
                break;
            case '9':
                Int = Int * 10 + 9;
                break;

            default:
                throw "无法转换成整型，参数有错误";
        }
    }
    return Int;
}