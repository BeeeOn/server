/* 
 * File:   Server.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 15. March 2016
 */

#ifndef SERVER_H
#define SERVER_H

#include <memory>
#include <string>
#include <thread>

#include <asio.hpp>

#include "Session.h"

class Server {
public:
    Server(asio::io_service& io_service, unsigned short port,
    //std::string clientDelim, std::string serverDelim,
    unsigned int threads);

    void run();
  
    void virtual startAccept() = 0;
    
    void handleAccept(const asio::error_code& error);
  
    void handleStop();

protected:
    
    asio::io_service& m_io_service;
  
    asio::ip::tcp::acceptor m_acceptor;
  
    int m_threads_count;
  
    std::shared_ptr<Session> m_new_session;
};

#endif /* SERVER_H */