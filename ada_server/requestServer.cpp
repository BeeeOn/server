/**
 * @file requestServer.cpp
 * 
 * @brief implementation of RequestServer class
 *
 * @author Matus Blaho 
 * @version 1.0
 */


#include "requestServer.h"

bool RequestServer::HandleRequest ()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::HandleConnection");
	char buffer[1024];  //set buffer buffer
	ssize_t DataSize=0;
	int res = 0;
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
	if (!UIp->ParseMessage(data))
	{
		return (false);
	}
	tmessageV1_0 *m = UIp->ReturnMessage();
	tadapter *ada = nullptr;
	if ((ada = this->_sslcont->GetSSL(m->adapterINTid))==NULL)
	{
		this->_log->WriteMessage(ERR,"No connection for adapter in container");
		res = 1;
		this->SendResponse(res);
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
		return (res);
	}

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
		case SEARCH:
			message = MC->CreateSearchMessage(std::to_string(m->adapterINTid), m->DeviceIPstr, m->DeviceIDstr);
			break;
		case PING:
			if (ada->protocol_version<1.1)
			{
				res = 5;
			}
			else
			{
				message = MC->CreatePingMessage(ada);
			}
			break;
		default:
			close (com_s);
			return (false);
	}

	this->_log->WriteMessage(MSG,"Message: \n" + message);

	if (res==0)
	{
		res = this->s->Send(message,ada->connection);
	}
	if ((res==0)&&(m->state==DELETE))
	{
		this->_log->WriteMessage(TRACE,"Deleting device ID" + m->DeviceIDstr);
		this->database->DeleteFacility(m->DeviceIDstr, m->adapterINTid);
	}
	if ((res==0)&&(m->state==PING))
	{
		res = s->Receveive(ada->connection);
	}
	this->SendResponse(res);
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

void RequestServer::SendResponse(int code)
{
	std::string Message = this->UIp->CreateReply(code);
	this->_log->WriteMessage(MSG,"Message to ui_server: \n" + Message);
	if ((send(com_s, Message.c_str(), Message.size(), 0))<0)  //se
	{
		char errorbuf[200];
		strerror_r(errno,errorbuf,200);
		close (com_s);
		this->_log->WriteMessage(WARN,"Unable to send message to ui_server with code : " + std::to_string(errno) + " : " + errorbuf);
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ReceiveConnection");
	}
}
