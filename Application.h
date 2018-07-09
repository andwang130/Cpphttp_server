//
// Created by meidai on 18-7-9.
//
#pragma once
#ifndef UNTITLED5_APPLICATION_H
#define UNTITLED5_APPLICATION_H

#endif //UNTITLED5_APPLICATION_H
#include "RequestHandler.h"
class Application
{
private:
    BaseHandler *handler;
    Requests *requests;
    bool path(char *url);

    BaseHandler * Route();
public:
    ~Application();
    void set_requtest(Requests *requests1);
    char * implemen();


};
