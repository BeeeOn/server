/*
 * messageCreator.cpp
 *
 *  Created on: Feb 22, 2015
 *      Author: tuso
 */

#include "messageCreator.h"

using namespace pugi;

std::string MessageCreator::CreateDeleteMessage(std::string deviceID)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::CreateDeleteMessage");
	xml_document *resp = new xml_document();
	xml_node server_adapter = resp->append_child("server_adapter");
	//server_adapter->set_name("server_adapter");
	server_adapter.append_attribute("protocol_version");
	server_adapter.append_attribute("state");
	server_adapter.append_attribute("id");
	server_adapter.attribute("protocol_version") = "0.1";
	server_adapter.attribute("state") = "clean";
	struct sockaddr_in antelope;
	inet_aton(deviceID.c_str(), &antelope.sin_addr);
	in_addr_t DeviceIP = antelope.sin_addr.s_addr;
	unsigned int DeviceIPint = ntohl (DeviceIP);
	server_adapter.attribute("id") = std::to_string(DeviceIPint).c_str();
	tstringXMLwriter writer;
	resp->print(writer);
	delete(resp);
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::CreateDeleteMessage");
	return ("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" + writer.result);

};

std::string MessageCreator::CreateListenMessage(std::string AdapterID)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::CreateListenMessage");
	xml_document *resp = new xml_document();
	xml_node server_adapter = resp->append_child("server_adapter");
	//server_adapter->set_name("server_adapter");
	server_adapter.append_attribute("protocol_version");
	server_adapter.append_attribute("state");
	server_adapter.append_attribute("adapter_id");
	server_adapter.attribute("protocol_version") = "0.1";
	server_adapter.attribute("state") = "listen";
	server_adapter.attribute("adapter_id") = AdapterID.c_str();
	tstringXMLwriter writer;
	resp->print(writer);
	delete(resp);
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::CreateListenMessage");
	return ("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" + writer.result);

};

std::string MessageCreator::CreateSwitchMessage(std::string Adapter, std::string deviceId, std::string value, std::string type, std::string offset)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::CreateSwitchMessage");
	xml_document *resp = new xml_document();
	xml_node server_adapter = resp->append_child("server_adapter");
	//server_adapter->set_name("server_adapter");
	struct sockaddr_in antelope;
	inet_aton(deviceId.c_str(), &antelope.sin_addr);
	in_addr_t DeviceIP = antelope.sin_addr.s_addr;
	unsigned int DeviceIPint = ntohl (DeviceIP);
	server_adapter.append_attribute("protocol_version") = "0.1";
	server_adapter.append_attribute("state") = "set";
	server_adapter.append_attribute("id") = std::to_string(DeviceIPint).c_str();
	server_adapter.append_attribute("debug_adapter_id") = Adapter.c_str();
	//server_adapter.attribute("protocol_version") = "0.1";
	//server_adapter.attribute("state")
	xml_node value_node = server_adapter.append_child("value");
	value_node.text().set(value.c_str());
	value_node.append_attribute("type") = type.c_str();
	value_node.append_attribute("offset") = offset.c_str();
	tstringXMLwriter writer;
	resp->print(writer);
	delete(resp);
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::CreateSwitchMessage");
	return ("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" + writer.result);
};

MessageCreator::MessageCreator(Loger *l)
{
	_log = l;
};


