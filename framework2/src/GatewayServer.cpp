/* 
 * File:   GatewayServer.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 29. March 2016
 */

#include "GatewayServer.h"

#include "GatewaySession.h"

GatewayServer::GatewayServer(asio::io_service& io_service, int port, int threads):
    Server(io_service, port, threads)
{
}

void GatewayServer::startAccept()
{
    // Create new session.
    m_new_session = std::make_shared<GatewaySession>(m_io_service);

    // Accept on the socket of the session.
    m_acceptor.async_accept(m_new_session->getSocket(),
    [&](const asio::error_code& error){ handleAccept(error); });
}