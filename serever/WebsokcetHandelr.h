//
// Created by meidai on 18-8-1.
//

#ifndef UNTITLED5_WEBSOKCETHANDELR_H
#define UNTITLED5_WEBSOKCETHANDELR_H

#include <iostream>
using  namespace std;
struct Wx_requests
{
  int coon;
  string url;
};
class WebsocketHandelr
{
protected:
    void write_message(string message);
public:
    Wx_requests wx_requests;
    virtual void open();
    virtual void on_message(string message);
    virtual void on_close();
    virtual void check_origin();

};
#endif //UNTITLED5_WEBSOKCETHANDELR_H
