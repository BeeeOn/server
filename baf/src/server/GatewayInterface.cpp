/*
 * File:   GatewayInterface.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 13. April 2016
 */

#include "server/GatewayInterface.h"

#include <sstream>
#include <string>
#include <stdexcept>

#include "pugixml.hpp"

#include "app/Config.h"

GatewayInterface::GatewayInterface():
	m_io_service(),
	m_socket(m_io_service),
	m_resolver(m_io_service)
{
}

void GatewayInterface::sendSetState(long long gateway_id, long device_euid, int module_id, int new_value)
{
	// Connect to ada_server_sender.
	connect();

	// Create switch message from ui_server-ada_server protocol.
	pugi::xml_document doc;

	pugi::xml_node request_node = doc.append_child();
	request_node.set_name("request");
	request_node.append_attribute("type") = "switch";

	pugi::xml_node sensor_node = request_node.append_child();
	sensor_node.set_name("sensor");
	// Cast is needed, because append_attribute doesn't support long for some reason.
	sensor_node.append_attribute("id") = static_cast<long long>(device_euid);
	// This attribute in protocol is outdated. It is used to describe device_euid.
	sensor_node.append_attribute("type") = module_id;
	sensor_node.append_attribute("onAdapter") = gateway_id;

	// Append value.
	pugi::xml_node value_node = sensor_node.append_child();
	value_node.set_name("value");
	value_node.append_child(pugi::node_pcdata).set_value(std::to_string(new_value).c_str());

	// Convert DOM to XML into stream.
	std::ostringstream stream;
	doc.save(stream, "\t", pugi::format_raw | pugi::format_no_declaration);

	// Send request to ada_server_sender.
	send(stream.str());
	// Receive and process response.
	if (!requestSuccessful()) {
		throw std::runtime_error("Request to set state of actuator was unsuccessful.");
	}
}

bool GatewayInterface::pingGateway(long long gateway_id)
{
	// Connect to ada_server_sender.
	connect();

	// Create switch message from ui_server-ada_server protocol.
	pugi::xml_document doc;

	pugi::xml_node request_node = doc.append_child();
	request_node.set_name("request");
	request_node.append_attribute("type") = "ping";

	pugi::xml_node adapter_node = request_node.append_child();
	adapter_node.set_name("adapter");
	adapter_node.append_attribute("id") = gateway_id;

	// Convert DOM to XML into stream.
	std::ostringstream stream;
	doc.save(stream, "\t", pugi::format_raw | pugi::format_no_declaration);

	// Send request to ada_server_sender.
	send(stream.str());
	// Receive and process response.

	if (!requestSuccessful()) {
		// Gateway is not available.
		return false;
	}
	else {
		// Gateway is not available.
		return true;
	}
}

void GatewayInterface::connect()
{
	// Connect to localhost and port of ada_server_sender.
	std::string localhost("127.0.0.1");
	std::string ada_server_sender_port = std::to_string(7081);

	asio::ip::tcp::resolver::query query(localhost, ada_server_sender_port);

	asio::connect(m_socket, m_resolver.resolve(query));
}

void GatewayInterface::send(std::string request)
{
	// Send the request to ada_server_sender.
	asio::write(m_socket, asio::buffer(request, request.length()));
}

bool GatewayInterface::requestSuccessful()
{
	asio::streambuf response_buffer;
	asio::read_until(m_socket, response_buffer, "</reply>");

	// Convert received message to std::string.
	std::iostream response_stream(&response_buffer);

	std::string response{std::istreambuf_iterator<char>(response_stream),
						 std::istreambuf_iterator<char>()};

	int response_code = parseResponse(response);
	// If response code is 0 request was correctly received, otherwise it's an error.
	if (response_code == 0) {
		return true;
	}
	else {
		return false;
	}
}

int GatewayInterface::parseResponse(std::string response)
{
	// Parse received message.
	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_buffer(response.c_str(), response.size());

	if (!result) {
		throw std::runtime_error("Response received from ada_server_sender was not correctly parsed.");
	}
	// Find <reply> tag.
	pugi::xml_node reply_node = doc.child("reply");
	if (reply_node == NULL) {
		throw std::runtime_error("Response received from ada_server_sender doesn't contain reply tag.");
	}
	// Find errorCode attribute.
	pugi::xml_attribute reply_code = reply_node.attribute("errorCode");
	if (reply_code == NULL) {
		throw std::runtime_error("Response received from ada_server_sender doesn't contain attribute errorCode.");
	}
	// Return parsed response code.
	return reply_code.as_int();
}
