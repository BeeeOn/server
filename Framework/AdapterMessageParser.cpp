


#include "AdapterMessageParser.h"

using namespace pugi;
/**********************************************Start of MessageParser section******************************************************/

/** Metoda vracajuca obsah spracovanej spravy
* @return _message
*/
tmessage* MessageParser::ReturnMessage()
{
	return _message;
}

/**********************************************End of MessageParser section******************************************************/

/**********************************************Start of ProtocolV1MessageParser section******************************************************/


/** Metoda spracovanie spravy od adaptera verzie protokolu 1
* @param *message - ukazatel na zaciatok prijatej spravy
* @param length - dlzka spravy
* @return na zaklade uspechu/neuspechu vrati true/false
*/
bool ProtocolV1MessageParser::parseMessage(xml_node *adapter, float FM, float CP)
{
	this->_adapter = adapter;
	this->_device = this->_adapter->child("device");
	//ulozime si ID adaptera v znakovej forme
	this->GetID();
	//uloznie verzie firmwaru 
	this->_message->cp_version = CP;
	//ulozenie verzie komunikacneho protokolu
	this->_message->fm_version = FM;
	this->GetState();
	//spracujeme a ulozime casove razitko
	this->GetTimeStamp();
	if (_message->state == 0)
	{
		return true;
	}
	//ulozime si IP adresu zariadenia (verzia pre emulatory)
	this->GetDeviceID();
	this->GetBattery();

	//ulozime hodnotu kvality signalu
	this->GetSignal();
	//ulozime pocet posielanych dvojic typ hodnota
	this->GetValues();
	return true;
}

void ProtocolV1MessageParser::GetTimeStamp()
{
	this->_message->timestamp = _adapter->attribute("time").as_llong();
	char timebuf[200];
	struct tm *tmp;
	tmp = gmtime(&(this->_message->timestamp));
	strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %T", tmp);
	std::string msg;
	msg.append(timebuf);
}

void ProtocolV1MessageParser::GetID()
{
	this->_message->adapterINTid = std::stoll(_adapter->attribute("adapter_id").as_string(), nullptr, 16);
}

void ProtocolV1MessageParser::GetFM()
{
}

void ProtocolV1MessageParser::GetCP()
{
}

void ProtocolV1MessageParser::GetState()
{
	std::string temp_state = _adapter->attribute("state").as_string();

	if (temp_state.compare("register") == 0)
	{
		_message->state = 0;
	}
	else
	{
		if (temp_state.compare("data") == 0)
		{
			_message->state = 1;
		}
		else
		{
			_message->state = 2;
		}
	}
}

void ProtocolV1MessageParser::GetDeviceID()
{
	this->_message->sensor_id = std::stoll(_device.attribute("id").as_string(), nullptr, 16);
	in_addr_t temp = htonl(_message->sensor_id);
	struct sockaddr_in antelope;
	antelope.sin_addr.s_addr = temp;
	this->_message->DeviceIDstr = inet_ntoa(antelope.sin_addr);
}


void ProtocolV1MessageParser::GetBattery()
{
	xml_node battery = (_device.child("battery"));
	_message->battery = battery.attribute("value").as_uint();
}
void ProtocolV1MessageParser::GetSignal()
{
	xml_node signal = (_device.child("rssi"));
	_message->signal_strength = signal.attribute("value").as_uint();
}
bool ProtocolV1MessageParser::GetValues()
{
	//vytvotime si v pamati miesto na ulozenie dvojic typ hodnota
	xml_node values = _device.child("values");
	_message->values_count = values.attribute("count").as_uint();
	xml_node value = values.first_child();
	try
	{
		_message->values = new tvalue[_message->values_count];
	}
	catch (std::exception &e)
	{
		_message->values = NULL;
		return false;
	}
	for (int i = 0; i < _message->values_count; i++) //v cykle spracujeme prijate dvojice typ hodnota
	{
		if (value == NULL)
		{
			_message->values_count = i;
			break;
		}
		unsigned short int tempType = value.attribute("type").as_uint();
		_message->values[i].offset = value.attribute("offset").as_uint();
		tconcatenate temp;
		temp.input[0] = tempType;
		temp.input[1] = _message->values[i].offset;
		_message->values[i].intType = temp.result;
		_message->values[i].type = static_cast<tvalueTypes>(tempType);
		switch (_message->values[i].type)
		{
		case TEMP:
		case LUM:
		case REZ:
		case POS:
			_message->values[i].fval = value.text().as_float();
			if (_message->devType == UNDEF)
			{
				_message->devType = SEN;
			}
			else
			{
				if (_message->devType == ACT)
				{
					_message->devType = SENACT;
				}
			}
			break;
		case ONON:
		case TOG:
		case ONOFFSEN:
		case ONOFSW:
			_message->values[i].bval = false;
			if (value.text().as_float() == 1.0)
				_message->values[i].bval = true;
			if (_message->devType == UNDEF)
			{
				if (_message->values[i].type == ONOFFSEN)
					_message->devType = SEN;
				else
					_message->devType = ACT;
			}
			else
			{
				if (((_message->devType == ACT) && (_message->values[i].type == ONOFFSEN)) || ((_message->devType == SEN) && (_message->values[i].type == ONOFSW)))
				{
					_message->devType = SENACT;
				}
			}
			break;
		case EMI:
		case HUM:
		case BAR:
		case RGB:
		case RAN:
			_message->values[i].ival = value.text().as_int();
			if (_message->devType == UNDEF)
			{
				_message->devType = SEN;
			}
			else
			{
				if (_message->devType == ACT)
				{
					_message->devType = SENACT;
				}
			}
			break;
		default:
			_message->values[i].type = UNK;
			break;
		}
		value = value.next_sibling();
	}
	return true;
}
void ProtocolV1MessageParser::GetValuesCount()
{
}

/** Metoda vytvarajuca odpoved adapteru
* @param value - hodnota dalsieho zobudenia senzoru
* @return resp - ukazatel na spracovanu spravu
*/

std::string ProtocolV1MessageParser::CreateAnswer(int value)
{

	xml_document *resp = new xml_document();
	xml_node server_adapter = resp->append_child("server_adapter");
	//server_adapter->set_name("server_adapter");
	server_adapter.append_attribute("protocol_version");
	server_adapter.append_attribute("state");
	server_adapter.append_attribute("id");
	server_adapter.append_attribute("time");
	std::stringstream s;
	s.precision(2);
	s << _message->cp_version;
	server_adapter.attribute("protocol_version") = s.str().c_str();
	server_adapter.attribute("state") = "update";
	server_adapter.attribute("id") = std::to_string(_message->sensor_id).c_str();
	server_adapter.attribute("time") = std::to_string(value).c_str();
	tstringXMLwriter writer;
	resp->print(writer);
	delete(resp);
	std::string tmp = writer.result;
	if (tmp[tmp.length() - 1] == '\n')
	{
		tmp.erase(tmp.length() - 1, 1);
	}
	return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" + tmp;
};


ProtocolV1MessageParser::~ProtocolV1MessageParser()
{
	delete(this->_message);
}
ProtocolV1MessageParser::ProtocolV1MessageParser()
{
	this->_message = new tmessage();
}

/**********************************************End of ProtocolV1MessageParser section******************************************************/



/**********************************************Start of message section*********************************************************/

message::message()
{
	this->values = NULL;
	this->devType = UNDEF;
	this->DeviceIDstr = "";
	this->adapterINTid = 0;
	inet_pton(AF_INET, "0.0.0.0", &(this->adapter_ip));
	this->battery = 0;
	this->cp_version = 0.0;
	this->signal_strength = 0;
	this->state = 0;
	this->values_count = 0;
	this->fm_version = 0;
	this->sensor_id = 0;
	this->timestamp = 0;

}

message::~message()
{
	delete[] values;
	values = NULL;
}

/**********************************************End of message section**********************************************************/