/**
 * @file messageParsers.cpp
 * 
 * @brief implementation of MessageParser UIServerMessageParser ProtocolV1MessageParser classes
 *
 * @author Matus Blaho
 * @version 1.0
 */

#include "messageParsers.h"

using namespace pugi;

tmessage* MessageParser::ReturnMessage()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::ReturnMessage");
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ReturnMessage");
	return (_message);

}

template< typename T >
std::string int_to_hex( T i )
{
  std::stringstream stream;
  stream << "0x" 
         << std::hex << i;
  return stream.str();
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
	//GEt adapter ID
	this->GetID();
	this->_message->cp_version=CP;
	this->_log->WriteMessage(INFO,"CP version " + std::to_string(_message->cp_version));
	this->_message->fm_version=FM;
	this->_log->WriteMessage(INFO,"FM version " + std::to_string(_message->fm_version));
	this->GetState();
	this->GetTimeStamp();
	//register message ends here
	if (_message->state == REGISTER)
	{
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::parseMessage");
		return (true);
	}
	this->GetDeviceID();
	this->GetValues();
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::parseMessage");
	return (true);
}

void ProtocolV1MessageParser::GetTimeStamp()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetTimeStamp");
	time_t tmpTime = 0;

	tmpTime = 0;
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
		_message->state = REGISTER;
	}
	else
	{
		if (temp_state.compare("data")==0)
		{
			_message->state = DATA;
		}
		else
		{
			_message->state = UNKNOWN;
		}
	}
	this->_log->WriteMessage(MSG,"STATE :" + std::to_string(this->_message->state));
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetState");
}

void ProtocolV1MessageParser::GetDeviceID()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetDeviceID");
	
	this->_message->device_type = std::stoll(_device.attribute("device_id").as_string(),nullptr,16);   
	       
	this->_message->device_euid = std::stoll(_device.attribute("euid").as_string(),nullptr,16);
  
  
	in_addr_t temp = htonl (_message->device_euid);
	this->_log->WriteMessage(MSG,"Device id :" + std::to_string(this->_message->device_euid));
	struct sockaddr_in antelope;
	antelope.sin_addr.s_addr = temp;
	this->_message->DeviceIDstr = inet_ntoa (antelope.sin_addr);
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetDeviceID");
}

bool ProtocolV1MessageParser::GetValues()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetValues");
	xml_node values = _device.child("values");
	_message->values_count = values.attribute("count").as_uint();
	xml_node value = values.first_child();
	try
	{
		_message->values = new tvalue[_message->values_count]; //try to create space for values
	}
	catch(std::exception &e)
	{
		this->_log->WriteMessage(ERR,"Unable to allocate memory for values client won't be served properly!" );
		_message->values = NULL;
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetValues");
		return (false);
	}
	for (int i = 0; i < _message->values_count; i++) //go through all values and save them
	{
		if (value==NULL)
		{
			_message->values_count = i;
			break;
		}

		int new_module_id = value.attribute("module_id").as_uint();
		_message->values[i].module_id = (unsigned short int)new_module_id;
		this->_log->WriteMessage(MSG,"Module ID :" + std::to_string(_message->values[i].module_id));

		//for test purposes just float value
		_message->values[i].measured_value = value.text().as_float();
		this->_log->WriteMessage(MSG,"Measured value :" + std::to_string(_message->values[i].measured_value));

		_message->values[i].status = value.attribute("status").value();
    //set default status
    if(_message->values[i].status == "")
      _message->values[i].status = "available";
		this->_log->WriteMessage(MSG,"status :" + std::to_string(_message->values[i].measured_value));

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

std::string ProtocolV1MessageParser::CreateAnswer(int value)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::CreateAnswer");
	xml_document *resp = new xml_document();
	xml_node server_adapter = resp->append_child("server_adapter");
	//server_adapter->set_name("server_adapter");
	server_adapter.append_attribute("protocol_version");
	server_adapter.append_attribute("state");
	server_adapter.append_attribute("euid");
	server_adapter.append_attribute("time");
	std::stringstream s;
	s.precision(2);
	s << _message->cp_version;
	server_adapter.attribute("protocol_version") = s.str().c_str();
	server_adapter.attribute("state") = "update";
  
	server_adapter.attribute("euid") = int_to_hex(_message->device_euid).c_str();
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

bool UIServerMessageParser::GetSubjectID()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetSubjectID");
	this->_Message->DeviceIDstr= subject.attribute("id").as_string();
	this->_log->WriteMessage(INFO,"Device ID : " + _Message->DeviceIDstr);
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetSubjectID");
	return (true);
}

bool UIServerMessageParser::GetAdapterID()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetAdapterID");
	if(this->_Message->state==LISTEN)
	{
		this->_Message->adapterINTid = subject.attribute("id").as_llong();
	}
	else
	{
		this->_Message->adapterINTid = subject.attribute("onAdapter").as_llong();
	}
	this->_log->WriteMessage(INFO,"Adapter ID : " + std::to_string(_Message->adapterINTid));
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetAdapterID");
	return (true);
}

bool UIServerMessageParser::GetValue()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetValue");
	xml_node value = subject.first_child();
	int n =0;
	while (value != NULL)
	{
		n++;
		value = value.next_sibling();
	}
	value = subject.first_child();
	_Message->values_count = n;
	_Message->values = new tvalue[_Message->values_count];
	for (int i = 0;i<n;i++)
	{	
		/*
		/////!!!!!!!!!!!! NEFUNGUJE !!!!!!!!!!!!! //////
		std::bitset<16> type(value.parent().attribute("type").as_int()),typeoffset(value.parent().attribute("type").as_int());
		type.reset(8);
		type.reset(9);
		type.reset(10);
		type.reset(11);
		typeoffset>>=(8);
		_Message->values[i].type = static_cast<tvalueTypes>(type.to_ulong());
		this->_Message->values[i].intType=type.to_ulong();
		this->_Message->values[i].offset = typeoffset.to_ulong();


		this->_log->WriteMessage(MSG,"offset : " + std::to_string(this->_Message->values[i].offset));
		this->_log->WriteMessage(MSG,"type : " + std::to_string(this->_Message->values[i].intType));
		*/

		this->_Message->values[i].module_id = value.parent().attribute("type").as_int();
		this->_Message->values[i].measured_value = value.text().as_float();

		this->_log->WriteMessage(MSG,"Module ID : " + std::to_string(this->_Message->values[i].module_id));

		/*
		switch(this->_Message->values[i].type)
		{
			case ONON:
			case TOG:
			case ONOFFSEN:
			case ONOFSW:
				_Message->values[i].bval = false;
				if (value.text().as_float()==1.0)
					_Message->values[i].bval = true;
				this->_log->WriteMessage(MSG,"Value :" + std::to_string(_Message->values[i].bval));
				break;
			case BT:
				_Message->values[i].fval = value.text().as_float();
				this->_log->WriteMessage(MSG,"Value :" + std::to_string(_Message->values[i].fval));
				break;
			case BOT:
			case BOM:
				_Message->values[i].ival = value.text().as_int();
				this->_log->WriteMessage(MSG,"Value :" + std::to_string(_Message->values[i].ival));
				break;
			default:
				_Message->values[i].type = UNK;
				
				this->_log->WriteMessage(WARN,"Unsupported type!");
				this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetValue");
				return (false);
				break;
		}
		*/
		
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetValue");
	return (true);
}

/*
bool UIServerMessageParser::GetOffsetType()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetTypeOffset");
	std::bitset<16> type(this->subject.attribute("type").as_int()),typeoffset(subject.attribute("type").as_int());
	type.reset(8); //clear offset from type
	type.reset(9);
	type.reset(10);
	type.reset(11);
	typeoffset>>=(8); //shift offset back to right
	_Message->values->type = static_cast<tvalueTypes>(type.to_ulong());
	this->_Message->values->intType=type.to_ulong();
	this->_Message->values->offset = typeoffset.to_ulong();
	this->_log->WriteMessage(MSG,"offset : " + std::to_string(this->_Message->values->offset));
	this->_log->WriteMessage(MSG,"type : " + std::to_string(this->_Message->values->intType));
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetTypeOffset");
	return (true);
}
*/
bool UIServerMessageParser::GetState()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetState");
	std::string reqType = request.attribute("type").as_string();
	if(reqType.compare("delete")==0)
	{
		this->_Message->state=DELETE;
		this->_log->WriteMessage(MSG,"Request type is DELETE");
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetState");
		return (true);
	}
	if(reqType.compare("switch")==0)
	{
		this->_Message->state=SWITCH;
		this->_log->WriteMessage(MSG,"Request type is SWITCH");
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetState");
		return (true);
	}
	if (reqType.compare("listen")==0)
	{
		this->_log->WriteMessage(MSG,"Request type is LISTEN");
		this->_Message->state=LISTEN;
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetState");
		return (true);
	}

	this->_log->WriteMessage(WARN,"Request type is UNKNOWN");
	this->_Message->state=UNKNOWN;
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetState");
	return (false);
}

bool UIServerMessageParser::ParseMessage(std::string MSG)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::ParseMessage");
	this->_toParse = MSG;
	bool res;
	xml_document doc;
	xml_parse_result result = doc.load_buffer(this->_toParse.data(), this->_toParse.size());
	if (result.status!=status_ok)
	{
		std::string Err = result.description();
		this->_log->WriteMessage(WARN,"Message XML error :" + Err);
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ParseMessage");
		return (false);
	}
	this->request = doc.child("request");
	this->subject = request.first_child();
	res = GetState();
	switch(this->_Message->state)
	{
		case DELETE:
			res = this->GetAdapterID();
			res = this->GetSubjectID();
			break;
		case SWITCH:
			res = this->GetAdapterID();
			res = this->GetSubjectID();
			res = this->GetValue();
			break;
		case LISTEN:
			res=this->GetAdapterID();
			break;
		default:
			this->_log->WriteMessage(WARN,"Something was wrong with request MSG!");
			this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ParseMessage");
			return (false);
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ParseMessage");
	return (res);
}

UIServerMessageParser::UIServerMessageParser(Loger *l)
{
	this->_Message = new tmessage();
	this->_log = l;
}

UIServerMessageParser::~UIServerMessageParser()
{
	delete this->_Message->values;
	this->_Message->values =NULL;
	delete this->_Message;
}
