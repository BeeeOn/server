/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Server.h
 * Author: mrmaidx
 *
 * Created on 20. ledna 2016, 15:11
 */

#ifndef SERVER_H
#define SERVER_H

#include "u_server.h"

class Server : public u_server
{
public:   
    Server(asio::io_service& io_service, unsigned short port,
    std::string clientDelim, std::string serverDelim,
    int threadNum, int timePeriod);   
    void startAccept();
};

#endif /* SERVER_H */
