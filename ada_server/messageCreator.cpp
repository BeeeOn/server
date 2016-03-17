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

template< typename T >
std::string int_to_hex( T i )
{
    std::stringstream stream;
    stream << "0x"
    << std::hex << i;
    return stream.str();
}

std::string MessageCreator::CreateDeleteMessage(std::string deviceID)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::CreateDeleteMessage");
	xml_document *resp = new xml_document();
	xml_node server_adapter = resp->append_child("server_adapter");
	server_adapter.append_attribute("protocol_version");
	server_adapter.append_attribute("state");
    server_adapter.append_attribute("euid");
	
	server_adapter.attribute("protocol_version") = "1.0";
	server_adapter.attribute("state") = "clean";
	/*struct sockaddr_in antelope;
	inet_aton(deviceID.c_str(), &antelope.sin_addr);
	in_addr_t DeviceIP = antelope.sin_addr.s_addr;
	unsigned int DeviceIPint = ntohl (DeviceIP);*/
	unsigned long long DeviceIPint = std::stoll(deviceID);
    this->_log->WriteMessage(DEBUG,"Device euid in message will be:" + int_to_hex(DeviceIPint));
	server_adapter.attribute("euid") = int_to_hex(DeviceIPint).c_str();
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
	server_adapter.attribute("protocol_version") = "1.0";
	server_adapter.attribute("state") = "listen";
	server_adapter.attribute("adapter_id") = AdapterID.c_str();
	tstringXMLwriter writer;
	resp->print(writer);
	delete(resp);
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::CreateListenMessage");
	return ("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" + writer.result);

};

std::string MessageCreator::CreateSearchMessage(std::string AdapterID, std::string deviceIp, std::string deviceId)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::CreateListenMessage");
	xml_document *resp = new xml_document();
	xml_node server_adapter = resp->append_child("server_adapter");
  
	server_adapter.append_attribute("protocol_version");
	server_adapter.append_attribute("state");
	server_adapter.append_attribute("adapter_id");     
  
	server_adapter.attribute("protocol_version") = "1.0";
	server_adapter.attribute("state") = "search";
	server_adapter.attribute("adapter_id") = AdapterID.c_str();
  
  pugi::xml_node deviceNode = server_adapter.append_child();
  deviceNode.set_name("device");  
  
	deviceNode.append_attribute("ip");
	deviceNode.append_attribute("euid");
  
	deviceNode.attribute("ip") = deviceIp.c_str();
	deviceNode.attribute("euid") = deviceId.c_str();
  
	tstringXMLwriter writer;
	resp->print(writer);
	delete(resp);
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::CreateListenMessage");
	return ("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" + writer.result);

};

std::string MessageCreator::CreateSwitchMessage(tmessageV1_0 *Message)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::CreateSwitchMessage");
	xml_document *resp = new xml_document();
	xml_node server_adapter = resp->append_child("server_adapter");
	/*struct sockaddr_in antelope;
	inet_aton(Message->DeviceIDstr.c_str(), &antelope.sin_addr);
	in_addr_t DeviceIP = antelope.sin_addr.s_addr;
	unsigned int DeviceIPint = ntohl (DeviceIP);*/
	server_adapter.append_attribute("protocol_version") = "1.0";
	server_adapter.append_attribute("state") = "set";
    unsigned long long DeviceIPint = std::stoll(Message->DeviceIDstr);
    this->_log->WriteMessage(DEBUG,"Device euid in swtich message will be:" + int_to_hex(DeviceIPint));
    server_adapter.append_attribute("euid") = std::to_string(DeviceIPint).c_str();
	server_adapter.append_attribute("adapter_id") = std::to_string(Message->adapterINTid).c_str();

	for (int i = 0; i < Message->values_count; i++)
	{
		xml_node value_node = server_adapter.append_child("value");
		/*std::string stringType;
		std::ostringstream os;
		os << std::hex << Message->values[i].module_id;
		stringType = os.str();*/
		value_node.append_attribute("module_id") = int_to_hex(Message->values[i].module_id).c_str();
        this->_log->WriteMessage(DEBUG,"Module id in swtich message will be:" + int_to_hex(Message->values[i].module_id));
        value_node.text().set(Message->values[i].measured_value);


		//value_node.append_attribute("type") = stringType.c_str();
		//value_node.append_attribute("offset") = std::to_string(Message->values[i].offset).c_str();
		
		/*
		switch(Message->values[i].type)
		{
			case ENUM:
			case HUMIDITY:
			case PRESSURE:
			case CO2:
			case BATTERY:
			case RSSI:
			case REFRESH:
				value_node.text().set(Message->values[i].ival);
				break;
			case TEMPERATURE:
			case LIGHT:
			case NOISE:
				value_node.text().set(Message->values[i].fval);
				break;
			default:
				break;
			
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
			
		}*/
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


