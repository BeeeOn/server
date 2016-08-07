/*
 * File:   GatewayInterface.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 13. April 2016
 */

#ifndef GATEWAYINTERFACE_H
#define GATEWAYINTERFACE_H

#include <string>

#include <asio.hpp>

class GatewayInterface
{
public:
	/**
	 * Constructor of class GatewayInterface.
	 */
	GatewayInterface();
	/**
	 * Sends message to ada_server_sender to change state of actuator.
	 * @param gateway_id ID of gateway on which is actuator present.
	 * @param device_euid EUID of device on which is actuator present.
	 * @param module_id ID of actuator modul.
	 * @param new_value New value to set on actuator.
	 */
	void sendSetState(long long gateway_id, long device_euid, int module_id, int new_value);

	/**
	 * Pings gateway.
	 * @param gateway_id ID of gateway on which is actuator present.
	 * @return If gateway is available returns true, otherwise false.
	 */
	bool pingGateway(long long gateway_id);

private:
	/**
	 * Connects to ada_server_sender.
	 */
	void connect();
	/**
	 * Sends message to ada_server_sender.
	 * @param request Message which is send.
	 */
	void send(std::string request);
	/**
	 * Receives message from ada_server_sender and processes it.
	 * @return If request was successfull true, else false.
	 */
	bool requestSuccessful();
	/**
	 * Parses response from ada_server_sender.
	 * @param response Received response.
	 * @return Code from response.
	 */
	int parseResponse(std::string response);
	/**
	 * Input output service to communicate with OS.
	 */
	asio::io_service m_io_service;
	/**
	 * Socket for connection.
	 */
	asio::ip::tcp::socket m_socket;
	/**
	 * Object for resolving host and port.
	 */
	asio::ip::tcp::resolver m_resolver;
	/**
	 * String to store response from ada_server_sender.
	 */
	std::string m_response;
};

#endif /* GATEWAYINTERFACE_H */
