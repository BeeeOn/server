/*
 * conncetionServer.cpp
 *
 *  Created on: Feb 18, 2015
 *      Author: tuso
 */

#include "connectionServer.h"

using namespace soci;
using namespace pugi;

/** Metoda pre prijatie spravy a jej spracovanie a odpovedanie na nu
 * @param IP - ip adresa zariadenia s ktorym sa komunikuje
    */
void ConnectionServer::HandleConnection (in_addr IP)
{
	_log->WriteMessage(TRACE,"Entering " + this->_Name + "::HandleConnection");
	char buffer[1024];  //natavime buffer
	int pollRes;
	ssize_t DataSize=0;
	std::string data;
	data.clear();
	struct pollfd ufds;
	ufds.fd = com_s;
	ufds.events = POLLIN;
	while (1)
	{
		pollRes = poll(&ufds, 1, this->_timeTimeOut*1000);
		if(pollRes==-1)
		{
			_log->WriteMessage(WARN,"Reading data from client failed with code " + std::to_string(errno) + " : " + std::strerror(errno));
			_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
			close(this->com_s);
			return;
		}
		if(pollRes==0)
		{
			_log->WriteMessage(WARN,"Connection to client timed out!");
			_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
			close(this->com_s);
			return;
		}
		else
		{
			if ((DataSize = read(this->com_s, &buffer, 1024)) < 0) //prijmeme dlzku prichadzajucich dat (2 byty kratkeho integera bez znamienka)
			{
				_log->WriteMessage(WARN,"Reading data from client failed");
				_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
				close(this->com_s);
				return;
			}
			if (DataSize==0)  //ak nic neprislo ukoncime proces
			{
				_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
				close(this->com_s);
				return;
			}
			data.append(buffer,DataSize);
			if ((data.find("</adapter_server>")!=std::string::npos)||((data[data.size()-2]=='/')&&(data[data.size()-2]=='>')))
			{
				break;
			}
		}
	}
	_log->WriteMessage(MSG,"Message :" + data);
	xml_document doc;
	xml_parse_result result = doc.load_buffer(data.data(), data.size());
	if (result.status!=status_ok)
	{
		std::string tmp = "Wrong message XML format : ";
		tmp.append(result.description());
		_log->WriteMessage(WARN,tmp);
		_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
		close(this->com_s);
		return;
	}
	xml_node adapter = doc.child("adapter_server");
	float CPversion =  adapter.attribute("protocol_version").as_float();
	int FMversion =  adapter.attribute("fw_version").as_int();
	if ((CPversion == (float)0.1)||(CPversion == (float)1.0))
	{
		if (FMversion == 0)
		{
			MP = new ProtocolV1MessageParser(_log);
		}
		else
		{
			if (FMversion == 1)
			{
				try
				{
					MP = new ProtocolV1MessageParser(_log);
				}
				catch (std::exception &e)
				{
					_log->WriteMessage(ERR,"Unable to create space for Protocol parser exiting client won't be server!");
					_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
					delete this->MP;
					close(this->com_s);
					return;
				}
			}
			else
			{
				try
				{
					MP = new ProtocolV1MessageParser(_log);
				}
				catch (std::exception &e)
				{
					_log->WriteMessage(ERR,"Unable to create space for Protocol parser exiting client won't be server!");
					_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
					delete this->MP;
					close(this->com_s);
					return;
				}
			}
		}
	}
	else
	{
		_log->WriteMessage(WARN,"Unsupported protocol version");
		_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
		delete this->MP;
		close(this->com_s);
		return;
	}
	if (!MP->ParseMessage(&adapter,FMversion,CPversion))  //pomocou parsera spracujeme spravu
	{

		_log->WriteMessage(WARN,"Wrong request format");
		delete this->MP;
		close(this->com_s);
		_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
		return;
	}
	//MP->setAdapterSocket();  //ulozime IP adresu adapteru do spravy
	parsedMessage = MP->ReturnMessage();  //spracovanu spravu si ulozime lokalne
	if (parsedMessage->state!=0)
	{
		response = MP->CreateAnswer(this->GetData());  //zavolame metodu na vytvorenie odpovede a metodu na ulozenie dat do DB
		int Err;
		_log->WriteMessage(MSG,"Response message: \n" + response);
		if ((Err=send(this->com_s, response.c_str(), response.size() , 0))<0)  //odoslanie odpovede klientovi
		{
			_log->WriteMessage(WARN,"Sending answer failed with code : " + std::to_string(errno) + " : " + std::strerror(errno));
		}
		close (this->com_s);
	}
	else
	{
	    std::string resp;
	    parsedMessage->socket = this->com_s;
	    _log->WriteMessage(TRACE,"Client socket desrciptor :" + std::to_string(this->com_s));
	    int oldSocket = -1;
	    database->GetAdapterData(&oldSocket,parsedMessage->adapterINTid);
	    if (oldSocket<=0)
	    {
			if (database->InsertAdapter(this->parsedMessage))
			{
			  resp="<server_adapter protocol_version=\"0.1\" state=\"register\" response=\"true\"/>";
			}
			else
			{
			  resp="<server_adapter protocol_version=\"0.1\" state=\"register\" response=\"false\"/>";
			}
	    }
	    else
	    {
	    	close (oldSocket);
	    	if (database->UpdateAdapter(this->parsedMessage))
			{
			  resp="<server_adapter protocol_version=\"0.1\" state=\"register\" response=\"true\"/>";
			}
			else
			{
			  resp="<server_adapter protocol_version=\"0.1\" state=\"register\" response=\"false\"/>";
			}
	    }
	    int Err;
	    if ((Err=send(this->com_s, resp.c_str(), resp.size() , 0))<0)  //odoslanie odpovede klientovi
	    {
			_log->WriteMessage(WARN,"Sending registration answer failed with code : " + std::to_string(errno) + " : " + std::strerror(errno));
	    }
	}
	if (parsedMessage->state!=0)
	{
		this->Notify(data);
	}

	if (parsedMessage->state!=0)
	{
		this->StoreData();
		database->LogValue(parsedMessage); //zavolame metodu ktora zisti ci je pre dane zariadenie zapnute logovanie hodnot a pripadne ich ulozi
	}
	_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
	return;
}

void ConnectionServer::Notify(std::string MSG)
{
	sockaddr_in SvSoc;
	_log->WriteMessage(TRACE,"Entering " + this->_Name + "::Notify");
	int s,Err;
	s = socket(AF_INET, SOCK_STREAM, 0);//vytvorenie socketu
	if(s < 0)
	{
		std::cerr<<"Creating socket failed\n";
		return; //chyba pri vytvarani socketu
	}
	SvSoc.sin_port = htons(7083);
	SvSoc.sin_family = AF_INET;  //nastavenie socketu servra
	in_addr adapterIP;
	inet_pton(AF_INET, "127.0.0.1",&adapterIP);
	SvSoc.sin_addr.s_addr = adapterIP.s_addr;
	//memcpy(&SvSoc.sin_addr.s_addr ,Adapter->h_addr, Adapter->h_length);
	if(connect(s,(sockaddr *) &SvSoc, sizeof(SvSoc)) < 0)//pripojenie na server
	{
		std::cerr<<"Unable to connect to framework\n";
		return;  //nepodarilo sa pripojit na server
	}
	if ((Err=send(s, MSG.c_str(), MSG.size(), 0))<0)  //odoslanie poziadavky na server
	{
		std::cerr<<"Unable to send message to framework\n";
		}
	_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Notify");
}

ConnectionServer::ConnectionServer(soci::session *SQL, Loger *L, int timeOut)
{
	L->WriteMessage(TRACE,"Entering " + this->_Name + "::Constructor");
	this->MP = nullptr;
	this->database = new DBHandler(SQL,L);
	this->_log = L;
	this->_timeTimeOut = timeOut;
	this->parsedMessage = NULL;
	this->com_s = -1;
	_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Constructor");
}

ConnectionServer::~ConnectionServer()
{
	_log->WriteMessage(TRACE,"Entering " + this->_Name + "::Destructor");
	delete this->database;
	_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Destructor");
}



/** Metoda volajuca funkcie na ulozenie a ziskanie dat z DB
 * @param message - spracovana sprava
    */

void ConnectionServer::StoreData()
{
	_log->WriteMessage(TRACE,"Entering " + this->_Name + "::StoreData");
	if (this->parsedMessage->state == 0)
	{
		if(!database->IsInDB("adapters","ID",std::to_string(this->parsedMessage->adapterINTid)))
		{
			database->InsertAdapter(this->parsedMessage);
		}
		else
		{
			database->UpdateAdapter(this->parsedMessage);
		}
		_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::StoreData");
		return;
	}
	else
	{
		database->UpdateAdapter(this->parsedMessage); //ak ano uz len updatneme jeho data
		if(!database->IsInDB("facilities","mac","'" + std::to_string(this->parsedMessage->sensor_id) + "'")) //to iste spravime aj pre senzor/aktor
		{
			database->InsertSenAct(this->parsedMessage);
		}
		else
		{
			database->UpdateSenAct(this->parsedMessage);
		}
	}
	_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::StoreData");
}

void ConnectionServer::SetSocket (int s)
{
	this->com_s = s;
}

int ConnectionServer::GetData()
{
	_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetData");
	int wakeUpTime = 5;
	if(!database->IsInDB("facilities","mac","'" + std::to_string(this->parsedMessage->sensor_id) + "'"))
	{
		wakeUpTime = 5;
	}
	else
	{
		wakeUpTime = database->GetWakeUpTime(std::to_string(this->parsedMessage->sensor_id));
	}
	_log->WriteMessage(TRACE,"refresh:" + std::to_string(wakeUpTime));
	if(wakeUpTime> 100000)
		wakeUpTime = 10;
	_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetData");
	return (wakeUpTime);
}
