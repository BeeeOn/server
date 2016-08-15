/*
 * File:   UserServer.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 19. March 2016
 */

#ifndef USERSERVER_H
#define USERSERVER_H

#include "server/Server.h"

class UserServer: public Server
{
public:
	/**
	 * Constructor of class UserServer.
	 * @param io_service Input output service.
	 * @param port Port number on which to estabilish server.
	 * @param threads Number of working threads for server.
	 */
	UserServer(asio::io_service& io_service, int port, int threads);
	/**
	 * Function to start accepting connections.
	 */
	void startAccept() override;
};

#endif /* USERSERVER_H */
