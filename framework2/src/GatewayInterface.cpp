/* 
 * File:   GatewayInterface.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 13. April 2016
 */

#include "GatewayInterface.h"

#include <sstream>
#include <string>

#include "pugixml.hpp"


GatewayInterface::GatewayInterface():
    m_io_service(),
    m_socket(m_io_service)
{
    // Connect to ada_server_sender.
    connect();
}

int GatewayInterface::sendPingGateway(long long gateway_id)
{
    // Create message to send.
    pugi::xml_document doc;
    
    pugi::xml_node request_node = doc.append_child();
    request_node.set_name("request");
    request_node.append_attribute("type") = "ping";
    
    pugi::xml_node adapter_node = request_node.append_child();
    adapter_node.set_name("adapter");
    adapter_node.append_attribute("id") = gateway_id;
     
    std::ostringstream stream;
    doc.save(stream);
    
    // Receive response.
    receive();
    
    // Here will be parsing of response when it's
    // implemented in the rest of the BeeeOn system.

    // Clear response string.
    m_response.clear();
    
    return 1;
}

void GatewayInterface::sendSetState(long long gateway_id, long device_euid, int module_id, int new_value)
{
    // Create message to send.
    pugi::xml_document doc;
    
    pugi::xml_node request_node = doc.append_child();
    request_node.set_name("request");
    request_node.append_attribute("type") = "switch";
    
    pugi::xml_node adapter_node = request_node.append_child();
    adapter_node.set_name("sensor");
    adapter_node.append_attribute("id") = static_cast<int>(device_euid);
    // This attribute in protocol is outdated. It is used to describe device_euid.
    adapter_node.append_attribute("type") = module_id;
    adapter_node.append_attribute("onAdapter") = gateway_id;
    
    std::ostringstream stream;
    doc.save(stream);
    
    // Send message.
    send(stream.str());
}

void GatewayInterface::connect()
{
    // Connect to localhost and port of ada_server_sender.
    m_socket.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 7081));
}

void GatewayInterface::send(std::string request)
{
    // Convert std::string to stream.
    asio::streambuf request_streambuf;
    std::ostream request_stream(&request_streambuf);
    request_stream << request;

    // Send the message.
    asio::write(m_socket, request_streambuf);
}

void GatewayInterface::receive()
{
    asio::streambuf response;
    asio::read_until(m_socket, response, "</reply>");

    // Convert received message to std::string.
    std::iostream response_stream(&response);

    std::string content{ std::istreambuf_iterator<char>(response_stream),
                     std::istreambuf_iterator<char>() };
    
    m_response = content;
}
