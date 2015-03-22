/*
 * sender.cpp
 *
 *  Created on: Feb 22, 2015
 *      Author: tuso
 */

#include "sender.h"


bool Sender::Send(std::string Message,int soc) //pripojenie na server a komunikacia s nim
{
	int Err;
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::Connect");
	if ((Err=send(soc, Message.c_str(), Message.size(), 0))<0)  //odoslanie poziadavky na server
	{
		this->_log->WriteMessage(ERR,"Unable to send message to : " + std::to_string(soc) + "error code : " + std::to_string(errno));
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Connect");
		return (false);
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Connect");
	return (true);
}

Sender::Sender(Loger *l)
{
	l->WriteMessage(TRACE,"Entering " + this->_Name + "::Sender");
	_log = l;
	s = -1;
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Sender");
}

Sender::~Sender()
{}
