/*
 * File:   GatewayServer.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 29. March 2016
 */

#ifndef DATAMSGSERVER_H
#define DATAMSGSERVER_H

#include "server/Server.h"

class GatewayServer: public Server
{
public:
	/**
	 * Constructor of class UserServer.
	 * @param io_service Input output service.
	 * @param port Port number on which to estabilish server.
	 * @param threads Number of working threads for server.
	 */
	GatewayServer(asio::io_service& io_service, int port, int threads);
	/**
	 * Function to start accepting connections.
	 */
	void startAccept() override;
};

#endif /* DATAMSGSERVER_H */
