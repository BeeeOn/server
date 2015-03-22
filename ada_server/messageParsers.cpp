/*
 * messageParsers.cpp
 *
 *  Created on: Feb 18, 2015
 *      Author: tuso
 */



#include "messageParsers.h"

using namespace pugi;

tmessage* MessageParser::ReturnMessage()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::ReturnMessage");
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ReturnMessage");
	return (_message);

}

	/**********************************************End of MessageParser section******************************************************/

	/**********************************************Start of ProtocolV1MessageParser section******************************************************/


/** Metoda spracovanie spravy od adaptera verzie protokolu 1
 * @param *message - ukazatel na zaciatok prijatej spravy
 * @param length - dlzka spravy
 * @return na zaklade uspechu/neuspechu vrati true/false
    */



bool ProtocolV1MessageParser::ParseMessage(xml_node *adapter,float FM,float CP)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::parseMessage");
	this->_adapter = adapter;
	this->_device = this->_adapter->child("device");
	//ulozime si ID adaptera v znakovej forme
	this->GetID();
	//uloznie verzie firmwaru
	this->_message->cp_version=CP;
	this->_log->WriteMessage(INFO,"CP version " + std::to_string(_message->cp_version));
	//ulozenie verzie komunikacneho protokolu
	this->_message->fm_version=FM;
	this->_log->WriteMessage(INFO,"FM version " + std::to_string(_message->fm_version));
	this->GetState();
	//spracujeme a ulozime casove razitko
	this->GetTimeStamp();
	if (_message->state == 0)
	{
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::parseMessage");
		return (true);
	}
	//ulozime si IP adresu zariadenia (verzia pre emulatory)
	this->GetDeviceID();
	this->GetBattery();

	//ulozime hodnotu kvality signalu
	this->GetSignal();
	//ulozime pocet posielanych dvojic typ hodnota
	this->GetValues();
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::parseMessage");
	return (true);
}

void ProtocolV1MessageParser::GetTimeStamp()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetTimeStamp");
	time_t tmpTime = 0;

	tmpTime = 0;
    //double tempd = 0.0; //pretoze python pouziva na ulozenie casoveho razitka datovy typ s plavajucou desatinnou ciarkou musime vykonat pretypovanie
	this->_message->timestamp = _adapter->attribute("time").as_llong();
	char timebuf[200];
	struct tm *tmp;
	tmp = gmtime(&(this->_message->timestamp));
	this->_log->WriteMessage(MSG,"Timestamp int val:"+ std::to_string(tmpTime) + " UTC");
	strftime(timebuf,sizeof(timebuf),"%Y-%m-%d %T",tmp);
	std::string msg;
	msg.append(timebuf);
	this->_log->WriteMessage(MSG,"Timestamp :"+ msg + " UTC");
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetTimeStamp");
}

void ProtocolV1MessageParser::GetID()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetID");
	this->_message->adapterINTid = std::stoll(_adapter->attribute("adapter_id").as_string(),nullptr,16);
    this->_log->WriteMessage(MSG,"Adapter int ID:" + std::to_string(this->_message->adapterINTid));
    this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetID");
}

void ProtocolV1MessageParser::GetFM()
{

}

void ProtocolV1MessageParser::GetCP()
{


}

void ProtocolV1MessageParser::GetState()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetState");
	std::string temp_state = _adapter->attribute("state").as_string();

	if (temp_state.compare("register")==0)
	{
		_message->state = 0;
	}
	else
	{
		if (temp_state.compare("data")==0)
		{
			_message->state = 1;
		}
		else
		{
			_message->state = 2;
		}
	}
	this->_log->WriteMessage(MSG,"STATE :" + std::to_string(this->_message->state));
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetState");
}

void ProtocolV1MessageParser::GetDeviceID()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetDeviceID");
	this->_message->sensor_id = _device.attribute("id").as_ullong();
			//std::stoll(_device.attribute("id").as_string(),nullptr,16);
	in_addr_t temp = htonl (_message->sensor_id);
	this->_log->WriteMessage(MSG,"Device id :" + std::to_string(this->_message->sensor_id));
	struct sockaddr_in antelope;
	antelope.sin_addr.s_addr = temp;
	this->_message->DeviceIDstr = inet_ntoa (antelope.sin_addr);
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetDeviceID");
}


void ProtocolV1MessageParser::GetBattery()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetBattery");
	xml_node battery = (_device.child("battery"));
	_message->battery=battery.attribute("value").as_uint();
	this->_log->WriteMessage(MSG,"Sensor battery :" + std::to_string(_message->battery));
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetBattery");
}
void ProtocolV1MessageParser::GetSignal()
{
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetSignal");
	xml_node signal = (_device.child("rssi"));
	_message->signal_strength=signal.attribute("value").as_uint();
	this->_log->WriteMessage(MSG,"Sensor signal :" + std::to_string(_message->signal_strength));
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetSignal");
}
bool ProtocolV1MessageParser::GetValues()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetValues");
	//vytvotime si v pamati miesto na ulozenie dvojic typ hodnota
	xml_node values = _device.child("values");
	_message->values_count = values.attribute("count").as_uint();
	xml_node value = values.first_child();
	try
	{
		_message->values = new tvalue[_message->values_count];
	}
	catch(std::exception &e)
	{
		this->_log->WriteMessage(ERR,"Unable to allocate memory for values client won't be served properly!" );
		_message->values = NULL;
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetValues");
		return (false);
	}
	for (int i = 0; i < _message->values_count; i++) //v cykle spracujeme prijate dvojice typ hodnota
	{
		if (value==NULL)
		{
			_message->values_count = i;
			break;
		}
		std::bitset<16> type (value.attribute("type").as_uint());
		std::bitset<16> offset (value.attribute("offset").as_uint());
		std::bitset<16> result(0);
		offset<<=(8);
		result = offset | type;
		_message->values[i].intType = (unsigned short int)result.to_ulong();
		this->_log->WriteMessage(MSG,"Type + offset :" + std::to_string(_message->values[i].intType));
		_message->values[i].type = static_cast<tvalueTypes>(type.to_ulong());
		this->_log->WriteMessage(MSG,"Type :" + std::to_string(_message->values[i].type));
		this->_log->WriteMessage(MSG,"Offset :" + std::to_string(_message->values[i].offset));
		switch(_message->values[i].type)
		{
			case TEMP:
			case LUM:
			case REZ:
			case POS:
				_message->values[i].fval = value.text().as_float();
				this->_log->WriteMessage(MSG,"Value :" + std::to_string(_message->values[i].fval));
				if (_message->devType==UNDEF)
				{
					_message->devType=SEN;
				}
				else
				{
					if (_message->devType==ACT)
					{
						_message->devType=SENACT;
					}
				}
				break;
			case ONON:
			case TOG:
			case ONOFFSEN:
			case ONOFSW:
				_message->values[i].bval = false;
				if (value.text().as_float()==1.0)
					_message->values[i].bval = true;
				this->_log->WriteMessage(MSG,"Value :" + std::to_string(_message->values[i].bval));
				if (_message->devType==UNDEF)
				{
					if(_message->values[i].type==ONOFFSEN)
						_message->devType=SEN;
					else
						_message->devType=ACT;
				}
				else
				{
					if (((_message->devType==ACT)&&(_message->values[i].type==ONOFFSEN))||((_message->devType==SEN)&&(_message->values[i].type==ONOFSW)))
					{
						_message->devType=SENACT;
					}
				}
				break;
			case EMI:
			case HUM:
			case BAR:
			case RGB:
			case RAN:
				_message->values[i].ival = value.text().as_int();
				this->_log->WriteMessage(MSG,"Value :" + std::to_string(_message->values[i].ival));
				if (_message->devType==UNDEF)
				{
					_message->devType=SEN;
				}
				else
				{
					if (_message->devType==ACT)
					{
						_message->devType=SENACT;
					}
				}
				break;
			default:
				_message->values[i].type = UNK;
				this->_log->WriteMessage(WARN,"Received unknown value type from " + this->_message->DeviceIDstr);
				break;
		}
	value = value.next_sibling();
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetValues");
	return (true);
}
void ProtocolV1MessageParser::GetValuesCount()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetValuesCount");
	this->_log->WriteMessage(MSG,"Values count :" + std::to_string(_message->values_count));
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetValuesCount");
}

/** Metoda vytvarajuca odpoved adapteru
 * @param value - hodnota dalsieho zobudenia senzoru
 * @return resp - ukazatel na spracovanu spravu
    */

std::string ProtocolV1MessageParser::CreateAnswer(int value)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::CreateAnswer");
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
	if (tmp[tmp.length()-1]=='\n')
	{
		tmp.erase(tmp.length()-1,1);
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::CreateAnswer");
	return ("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" + tmp);
};


ProtocolV1MessageParser::~ProtocolV1MessageParser()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::Destructor");
	delete(this->_message);
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Destructor");
}
ProtocolV1MessageParser::ProtocolV1MessageParser(Loger *L)
{
	L->WriteMessage(TRACE,"Entering " + this->_Name + "::Constructor");
	this->_message = new tmessage();
	this->_log = L;
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Constructor");
}


