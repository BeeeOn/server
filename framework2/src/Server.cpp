/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Server.cpp
 * Author: mrmaidx
 * 
 * Created on 20. ledna 2016, 15:11
 */


#include "ServerSession.h"
#include "Server.h"

Server::Server(asio::io_service& io_service, unsigned short port, std::string clientDelim, std::string serverDelim, int threadNum, int timePeriod):
    u_server(io_service, port, clientDelim, serverDelim, threadNum, timePeriod)
    {
    }

void Server::startAccept()
    {
    //std::cout << "u_classic_server::startAccept";
    new_session_ = new ServerSession(io_service_, clientDelim_, serverDelim_, timePeriod_);
    acceptor_.async_accept(new_session_->getSocket(),
      boost::bind(&u_server::handleAccept, this,
      asio::placeholders::error));
    }