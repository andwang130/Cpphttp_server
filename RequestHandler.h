//
// Created by meidai on 18-7-2.
//

#ifndef UNTITLED5_REQUESTHANDLER_H
#define UNTITLED5_REQUESTHANDLER_H

#endif //UNTITLED5_REQUESTHANDLER_HC

#include <iostream>
using namespace std;
class requestHandler
{
    string get_argument(string key,string Default=NULL);
    string get_body();
    void wirte();
public:
    void init();
   void post();
   void get();
};
