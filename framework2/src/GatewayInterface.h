/* 
 * File:   GatewayInterface.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 13. April 2016
 */

#ifndef GATEWAYINTERFACE_H
#define GATEWAYINTERFACE_H

#include <string>
#include <memory>
#include <sstream>
#include <string>

#include <asio.hpp>

class GatewayInterface
{
public:
    GatewayInterface();
    
    virtual ~GatewayInterface();
    
    void sendSetState(long long gateway_id, long device_euid, int module_id, int new_value);
    
private:
    int sendPingGateway(long long gateway_id);
    
    void connect();

    void send(std::string request);
    
    void receive();
    
    asio::io_service m_io_service;
    
    asio::ip::tcp::socket m_socket;
    
    std::string m_response;
};

#endif /* GATEWAYINTERFACE_H */
