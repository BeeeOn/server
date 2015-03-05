/*
 * sender.cpp
 *
 *  Created on: Feb 22, 2015
 *      Author: tuso
 */

#include "sender.h"


bool Sender::Connect (std::string Message,std::string IP)  //pripojenie na server a komunikacia s nim
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::Connect");
	sockaddr_in SvSoc;
	int s,Err;
	s = socket(AF_INET, SOCK_STREAM, 0);//vytvorenie socketu
	if(s < 0)
	{
		this->_log->WriteMessage(ERR,"Creating of socket failed ");
		return  (false); //chyba pri vytvarani socketu
	}
	std::string opt;
	opt = "tap0";
	if (setsockopt(s, SOL_SOCKET, SO_BINDTODEVICE, opt.c_str(), 4) < 0)
	{
		this->_log->WriteMessage(ERR,"Unable to switch socket to VPN tap0");
	}
	in_addr adapterIP;
	if (inet_pton(AF_INET, IP.c_str(), &adapterIP)!=1)
	{
		this->_log->WriteMessage(ERR,"Unable to convert IP address");
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Connect");
		return (false);
	}
	SvSoc.sin_port = htons(_port);
	SvSoc.sin_family = AF_INET;  //nastavenie socketu servra
	SvSoc.sin_addr.s_addr = adapterIP.s_addr;
	//memcpy(&SvSoc.sin_addr.s_addr ,Adapter->h_addr, Adapter->h_length);
	if(connect(s,(sockaddr *) &SvSoc, sizeof(SvSoc)) < 0)//pripojenie na server
	{
		this->_log->WriteMessage(ERR,"Unable to connect to : " + IP);
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Connect");
		return (false);  //nepodarilo sa pripojit na server
	}
	if ((Err=send(s, Message.c_str(), Message.size(), 0))<0)  //odoslanie poziadavky na server
	{
		this->_log->WriteMessage(ERR,"Enable to send message to : " + IP);
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Connect");
		return (false);
	}
	close (s);
	return (true);
}

Sender::Sender(Loger *l, int p)
{
	l->WriteMessage(TRACE,"Entering " + this->_Name + "::Sender");
	_log = l;
	_port = p;
	s = -1;
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Sender");
}

Sender::~Sender()
{}
