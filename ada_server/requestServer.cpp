/**
 * @file requestServer.cpp
 * 
 * @brief implementation of RequestServer class
 *
 * @author Matus Blaho 
 * @version 1.0
 */


#include "requestServer.h"

using namespace pugi;

bool RequestServer::HandleRequest ()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::HandleConnection");
	char buffer[1024];  //set buffer buffer
	ssize_t DataSize=0;
	std::string data;
	data.clear();
	while (1)
	{
		if ((DataSize = read(this->com_s, &buffer, 1024)) < 0) //receive request
		{
			char errorbuf[200];
			strerror_r(errno,errorbuf,200);
			this->_log->WriteMessage(ERR,"Reading data from client failed with code : " + std::to_string(errno) + errorbuf);
			this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::HandleConnection");
			return (false);
		}
		if (DataSize==0)  //if there is nothing to read end
		{
			return (false);
		}
		data.append(buffer,DataSize);
		if (data.find("</request>")!=std::string::npos) 
		{
			break;
		}
	}
	this->_log->WriteMessage(INFO,"Data: " + data);
	std::string message;
	std::string stringType;
	std::ostringstream os;
	if (!UIp->ParseMessage(data))
	{
		return (false);
	}
	tmessage *m = UIp->ReturnMessage();
	switch(m->state)
	{
		case DELETE:
			message = MC->CreateDeleteMessage(m->DeviceIDstr);
			break;
		case SWITCH:

			message = this->MC->CreateSwitchMessage(m);
			break;
		case LISTEN:
			message = MC->CreateListenMessage(std::to_string(m->adapterINTid));
			break;
		default:
			return (false);
	}
	bool res;
	this->_log->WriteMessage(INFO,"Message: \n" + message);
	SSL *s = NULL;
	if ((s = this->_sslcont->GetSSL(m->adapterINTid))==NULL)
	{
		this->_log->WriteMessage(ERR,"No connection for adapter in container");
		res = false;
	}
	else
	{
		res = this->s->Send(message,s);
	}
	if ((res==true)&&(m->state==DELETE))
	{
		this->_log->WriteMessage(TRACE,"Deleting device ID" + m->DeviceIDstr);
		this->database->DeleteFacility(m->DeviceIDstr, m->adapterINTid);
	}
	std::string Message = "<reply>true</reply>";
	if (!res)
	{
		 Message = "<reply>false</reply>";
	}
	if ((send(com_s, Message.c_str(), Message.size(), 0))<0)  //se
	{
		char errorbuf[200];
		strerror_r(errno,errorbuf,200);
		close (com_s);
		this->_log->WriteMessage(WARN,"Unable to send message to ui_server with code : " + std::to_string(errno) + " : " + errorbuf);
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ReciveConnection");
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
	return (res);
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
	UIp = new UIServerMessageParser(l);
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::RequestServer");
}

RequestServer::~RequestServer()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::~RequestServer");
	delete this->database;
	delete this->UIp;
	delete this->MC;
	delete this->s;
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::~RequestServer");
}

void RequestServer::SetSocket(int Soc)
{
	this->com_s = Soc;
}
