/* 
 * File:   Server.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
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