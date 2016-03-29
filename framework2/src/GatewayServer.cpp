/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DataMsgServer.cpp
 * Author: mrmaidx
 * 
 * Created on 29. března 2016, 10:46
 */

#include "GatewayServer.h"

#include "GatewaySession.h"

GatewayServer::GatewayServer(asio::io_service& io_service, unsigned short port, unsigned int threads):
    Server(io_service, port, threads)
{
}

void GatewayServer::startAccept() {
    
    //m_new_session = new UserSession(m_io_service);
    m_new_session = std::make_shared<GatewaySession>(m_io_service);
    /*
    m_acceptor.async_accept(m_new_session->getSocket(),
        boost::bind(&Server::handleAccept, this,
        asio::placeholders::error));
*/
    m_acceptor.async_accept(m_new_session->getSocket(),
    [&](const asio::error_code& error){ handleAccept(error); } );
}