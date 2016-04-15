/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DataMsgServer.h
 * Author: mrmaidx
 *
 * Created on 29. března 2016, 10:46
 */

#ifndef DATAMSGSERVER_H
#define DATAMSGSERVER_H

#include "Server.h"

class GatewayServer: public Server
{
public:
    GatewayServer(asio::io_service& io_service, int port, int threads);   
    
    void startAccept() override;
};

#endif /* DATAMSGSERVER_H */
