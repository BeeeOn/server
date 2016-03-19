/* 
 * File:   Server.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 15. March 2016
 */

#include "Server.h"

Server::Server(asio::io_service& io_service, unsigned short port, unsigned int threads_count):
    m_io_service(io_service),
    m_acceptor(io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
    m_threads_count(threads_count)
{
    std::cout << "Server started on port: " << port << std::endl;
}

void Server::run() {

    std::vector<std::shared_ptr<std::thread>> threads;

    for (unsigned int i = 0; i < m_threads_count; i++) {
      
        std::shared_ptr<std::thread> thread(std::make_shared<std::thread>([&](){ m_io_service.run(); }));
      
        threads.push_back(thread);
    }

    for (unsigned int i = 0; i < threads.size(); i++) {
        threads[i]->join();
    }
}

void Server::handleAccept(const asio::error_code& error) {
    
    if (!error) {
        m_new_session->onStart();
        startAccept();
    }
    else {
        std::cerr << "handleAccept error" << error.message() << std::endl;
        m_new_session.reset();
    }
}

void Server::handleStop() {
    
    std::cerr << "u_server::handleStop: Server stopped" << std::endl;
    m_io_service.stop();
    //delete m_new_session;
    //delete this;
}