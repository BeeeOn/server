/* 
 * File:   Server.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 15. March 2016
 */

#include "Server.h"

#include "Logger.h"

Server::Server(asio::io_service& io_service, int port, int threads):
    m_io_service(io_service),
    m_acceptor(io_service, asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), port)),
    m_threads(threads)
{
    logger.LOGFILE("server", "INFO") << "Server started on port: " << port << std::endl;
}

void Server::run()
{
    std::vector<std::shared_ptr<std::thread>> threads;

    // Create threads and run io_service on them.
    for (unsigned int i = 0; i < m_threads; i++) {
      
        std::shared_ptr<std::thread> thread(std::make_shared<std::thread>([&](){ m_io_service.run(); }));
      
        threads.push_back(thread);
    }
    // After server is stopped, join all the threads.
    for (unsigned int i = 0; i < threads.size(); i++) {
        threads[i]->join();
    }
}

void Server::handleAccept(const asio::error_code& error)
{
    if (!error) {
        // If new connection was made, start session process.
        m_new_session->onStart();

        // Start acccepting new connection.
        startAccept();
    }
    else {
        logger.LOGFILE("server", "ERROR") << "Accepting was not successful: " << error.message() << std::endl;
        m_new_session.reset();
    }
}

void Server::stop()
{
    m_io_service.stop();
    logger.LOGFILE("server", "INFO") << "Server stopped!" << std::endl;
}
