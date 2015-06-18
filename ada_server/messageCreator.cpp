/**
 * @file messageCreator.h
 * 
 * @brief implementation of MessageCreator class
 *
 * @author Matus Blaho 
 * @version 1.0
 */

#include "messageCreator.h"

using namespace pugi;

std::string MessageCreator::CreateDeleteMessage(std::string deviceID)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::CreateDeleteMessage");
	xml_document *resp = new xml_document();
	xml_node server_adapter = resp->append_child("server_adapter");
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

std::string MessageCreator::CreateSwitchMessage(tmessage *Message)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::CreateSwitchMessage");
	xml_document *resp = new xml_document();
	xml_node server_adapter = resp->append_child("server_adapter");
	struct sockaddr_in antelope;
	inet_aton(Message->DeviceIDstr.c_str(), &antelope.sin_addr);
	in_addr_t DeviceIP = antelope.sin_addr.s_addr;
	unsigned int DeviceIPint = ntohl (DeviceIP);
	server_adapter.append_attribute("protocol_version") = "0.1";
	server_adapter.append_attribute("state") = "set";
	server_adapter.append_attribute("id") = std::to_string(DeviceIPint).c_str();
	server_adapter.append_attribute("debug_adapter_id") = std::to_string(Message->adapterINTid).c_str();

	for (int i = 0;i<Message->values_count;i++)
	{
		xml_node value_node = server_adapter.append_child("value");
		std::string stringType;
		std::ostringstream os;
		os << std::hex << Message->values[i].type;
		stringType = os.str();
		value_node.append_attribute("type") = stringType.c_str();
		value_node.append_attribute("offset") = std::to_string(Message->values[i].offset).c_str();
		switch(Message->values[i].type)
		{
			case ONON:
			case TOG:
			case ONOFFSEN:
			case ONOFSW:
				value_node.text().set(Message->values[i].bval);
				break;
			case BT:
				value_node.text().set(Message->values[i].fval);
				break;
			case BOT:
			case BOM:
				value_node.text().set(Message->values[i].ival);
				break;
			default:
				break;
				}
	}
	tstringXMLwriter writer;
	resp->print(writer);
	delete(resp);
	delete [] Message->values;
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::CreateSwitchMessage");
	return ("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" + writer.result);
};

MessageCreator::MessageCreator(Loger *l)
{
	_log = l;
};


