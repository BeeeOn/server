/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UserServer.h
 * Author: mrmaidx
 *
 * Created on 19. března 2016, 21:11
 */

#ifndef USERSERVER_H
#define USERSERVER_H

#include "Server.h"

class UserServer: public Server
{
public:   
    UserServer(asio::io_service& io_service, int port, int threads);   
    
    void startAccept() override;
};

#endif /* USERSERVER_H */