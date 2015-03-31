/*
 * requestHandler.cpp
 *
 *  Created on: Feb 22, 2015
 *      Author: tuso
 */


#include "requestServer.h"

using namespace pugi;

bool RequestServer::HandleRequest ()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::HandleConnection");
	char buffer[1024];  //natavime buffer
	ssize_t DataSize=0;
	std::string data;
	data.clear();
	bool del =false;
	while (1)
	{
		if ((DataSize = read(this->com_s, &buffer, 1024)) < 0) //prijmeme dlzku prichadzajucich dat (2 byty kratkeho integera bez znamienka)
		{
			char errorbuf[200];
			strerror_r(errno,errorbuf,200);
			this->_log->WriteMessage(ERR,"Reading data from client failed with code : " + std::to_string(errno) + errorbuf);
			this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::HandleConnection");
			return (false);
		}
		if (DataSize==0)  //ak nic neprislo ukoncime proces
		{
			return (false);
		}
		data.append(buffer,DataSize);
		if (data.find("</request>")!=std::string::npos) //TODO : /> + poll
		{
			break;
		}
	}
	this->_log->WriteMessage(INFO,"Data: " + data);
	xml_document doc;
	xml_parse_result result = doc.load_buffer(data.data(), data.size());
	if (result.status!=status_ok)
	{
		std::string Err = result.description();
		this->_log->WriteMessage(WARN,"Message XML error :" + Err);
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
		return (false);
	}
	xml_node request = doc.child("request");
	std::string reqType = request.attribute("type").as_string();
	xml_node subject = request.first_child();
	std::string message;
	int AdapterIP = -1;
	this->_log->WriteMessage(INFO,"Request type :" + reqType);
	long long aID;
	std::string DEVID = "-1";
	if(reqType.compare("delete")==0)
	{
		std::string ID = subject.attribute("id").as_string();
		DEVID = ID;
		del = true;
		long long AdapterId = subject.attribute("onAdapter").as_llong();
		aID = AdapterId;
		this->_log->WriteMessage(INFO,"Adapter ID :" + std::to_string(AdapterId));
		this->_log->WriteMessage(INFO,"Device ID :" + ID);
		message = MC->CreateDeleteMessage(ID);
		//database->GetAdapterData(&AdapterIP,AdapterId);
	}
	else
	{
		if (reqType.compare("switch")==0)
		{
			std::string ID = subject.attribute("id").as_string();
			std::bitset<16> type(subject.attribute("type").as_int()),typeoffset(subject.attribute("type").as_int());
			type.reset(8);
			type.reset(9);
			type.reset(10);
			type.reset(11);
			typeoffset>>=(8);
			long long AdapterId = subject.attribute("onAdapter").as_llong();
			aID = AdapterId;
			std::string value = subject.first_child().text().as_string();
			this->_log->WriteMessage(INFO,"Adapter ID :" + std::to_string(AdapterId));
			this->_log->WriteMessage(INFO,"Device ID :" + ID);
			this->_log->WriteMessage(INFO,"Device type :" + std::to_string(type.to_ulong()));
			this->_log->WriteMessage(INFO,"Device offset :" + std::to_string(typeoffset.to_ulong()));
			this->_log->WriteMessage(INFO,"Device value :" + value);
			std::string stringType;
			std::ostringstream os;
			os << std::hex << type.to_ulong();
			stringType = os.str();
			this->_log->WriteMessage(INFO,"Device hexa type :" + stringType);
			message = this->MC->CreateSwitchMessage(std::to_string(AdapterId),ID,value,stringType,std::to_string(typeoffset.to_ulong()));
			//database->GetAdapterData(&AdapterIP,AdapterId);
		}
		else
		{
			long long int ID = subject.attribute("id").as_llong();
			aID = ID;
			this->_log->WriteMessage(INFO,"Adapter ID :" + std::to_string(ID));
			message = MC->CreateListenMessage(std::to_string(ID));
			//database->GetAdapterData(&AdapterIP,ID);
		}
	}
	bool res;
	//this->_log->WriteMessage(INFO,"Adapter socket :" + std::to_string(AdapterIP));
	this->_log->WriteMessage(INFO,"Message: \n" + message);
	SSL *s = NULL;
	if ((s = this->_sslcont->GetSSL(aID))==NULL)
	{
		res = false;
	}
	else
	{
		res = this->s->Send(message,s);
	}
	std::string Message = "<reply>true</reply>";
	if (!res)
	{
		 Message = "<reply>false</reply>";
	}
	if ((send(com_s, Message.c_str(), Message.size(), 0))<0)  //odoslanie poziadavky na server
	{
		char errorbuf[200];
		strerror_r(errno,errorbuf,200);
		close (com_s);
		this->_log->WriteMessage(WARN,"Unable to send message to ui_server with code : " + std::to_string(errno) + " : " + errorbuf);
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ReciveConnection");
	}
	if ((res==true)&&(del==true))
	{
		this->_log->WriteMessage(TRACE,"Deleting device ID" + DEVID);
		this->database->DeleteFacility(DEVID);
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
	return (res);
}

void RequestServer::HandleRequestCover()
{
	this->_log->WriteMessage(TRACE,"Process created!");
	/*session *Conn = NULL;
	while (Conn==NULL)
	{
		Conn=cont->GetConnection();
		if (Conn==NULL)
		{
			this->_log->WriteMessage(INFO,"ALL CONNECTIONS ARE USED!");
		}
	}*/

	std::string Message = "<reply>true</reply>";
	if (!this->HandleRequest())
	{
		 Message = "<reply>false</reply>";
	}
	if ((send(com_s, Message.c_str(), Message.size(), 0))<0)  //odoslanie poziadavky na server
	{
		char errorbuf[200];
		strerror_r(errno,errorbuf,200);
		close (com_s);  //zavreme socket //todo:replace
		this->_log->WriteMessage(WARN,"Unable to send message to ui_server with code : " + std::to_string(errno) + " : " + errorbuf);
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ReciveConnection");
	}
	//sem_post(&connectionSem);
}

RequestServer::RequestServer(Loger *l, soci::session *SQL, SSLContainer *sslcont)
{
	l->WriteMessage(TRACE,"Entering " + this->_Name + "::RequestServer");
	this->database = new DBHandler(SQL,l);
	this->_sslcont = sslcont;
	this->_log = l;
	this->com_s = -1;
	MC = new MessageCreator(l);
	s = new Sender(l);
	s->LoadCertificates();
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::RequestServer");
}

RequestServer::~RequestServer()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::~RequestServer");
	delete this->database;
	delete this->MC;
	delete this->s;
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::~RequestServer");
}

void RequestServer::SetSocket(int Soc)
{
	this->com_s = Soc;
}
