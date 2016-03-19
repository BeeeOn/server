/* 
 * File:   UserServer.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 15. March 2016
 */

#include "UserServer.h"

#include "Server.h"
#include "UserSession.h"

UserServer::UserServer(asio::io_service& io_service, unsigned short port, unsigned int threads):
    Server(io_service, port, threads)
{
}

void UserServer::startAccept() {
    
    //m_new_session = new UserSession(m_io_service);
    m_new_session = std::make_shared<UserSession>(m_io_service);
    /*
    m_acceptor.async_accept(m_new_session->getSocket(),
        boost::bind(&Server::handleAccept, this,
        asio::placeholders::error));
*/
    m_acceptor.async_accept(m_new_session->getSocket(),
    [&](const asio::error_code& error){ handleAccept(error); } );
}