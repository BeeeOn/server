/* 
 * File:   Server.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 15. March 2016
 */

#ifndef SERVER_H
#define SERVER_H

#include <memory>
#include <thread>

#include <asio.hpp>

#include "Session.h"

class Server {
public:
    Server(asio::io_service& io_service, int port, int threads);

    virtual ~Server();
    
    /**
     * Function which creates threads to run server
     * and then waits for connections.
     */
    void run();
  
    /**
     * Function to start accepting connections.
     */
    void virtual startAccept() = 0;
    
    /**
     * Handler after connection was made.
     */
    void handleAccept(const asio::error_code& error);
    
    /**
     * Function to stop server.
     */
    void stop();

protected:
  
    /** Adress of io_service object on which server is running. */
    asio::io_service& m_io_service;
  
    /** Acceptor allows to receive connections on certain adress and port. */
    asio::ip::tcp::acceptor m_acceptor;
  
    /** Pointer to new session. */
    std::shared_ptr<Session> m_new_session;
    
    int m_threads;
    
    int m_port;
};

#endif /* SERVER_H */
