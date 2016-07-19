/*
 * File:   Server.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 15. March 2016
 */

#ifndef SERVER_H
#define SERVER_H

#include <memory>

#include <asio.hpp>

#include "Session.h"

class Server {
public:
	/**
	 * Constructor of class Server.
	 * @param io_service Input output service.
	 * @param port Port number on which to estabilish server.
	 * @param threads Number of working threads for server.
	 */
	Server(asio::io_service& io_service, int port, int threads);
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
	/**
	 * Adress of io_service object on which server is running.
	 */
	asio::io_service& m_io_service;
	/**
	 * Acceptor allows to receive connections on certain adress and port.
	 */
	asio::ip::tcp::acceptor m_acceptor;
	/**
	 * Pointer to new session.
	 */
	std::shared_ptr<Session> m_new_session;
	/**
	 * Number of working threads to run server.
	 */
	int m_threads;
	/**
	 * Port number on which the server is estabilished.
	 */
	int m_port;
};

#endif /* SERVER_H */
