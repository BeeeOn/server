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
     * @param gateway_id ID of gateway on which actuator is.
     * @param device_euid EUID of device on which actuator is.
     * @param module_id ID of actuator modul.
     * @param new_value New value to set on actuator.
     */
    void sendSetState(long long gateway_id, long device_euid, int module_id, int new_value);
    
private:
    /**
     * Sends message to ada_server_sender to ping a gateway.
     * Functionality in BeeeOn is not ready yet, so it doesn't work yet.
     * @param gateway_id ID of gateway to ping.
     * @return Value about a state of gateway.
     */
    int sendPingGateway(long long gateway_id);
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
     * Receives message from ada_server_sender.
     * Stores it in m_response.
     */
    void receive();
    /**
     * Input output service.
     */
    asio::io_service m_io_service;
    /**
     * Socket for connection.
     */
    asio::ip::tcp::socket m_socket;
    /**
     * Variable to store response from ada_server_sender.
     */
    std::string m_response;
};

#endif /* GATEWAYINTERFACE_H */
