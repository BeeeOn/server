/*
 * sender.cpp
 *
 *  Created on: Feb 22, 2015
 *      Author: tuso
 */

#include "sender.h"


bool Sender::Send(std::string Message,SSL *s) //pripojenie na server a komunikacia s nim
{
	int Err;
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::Send");
	int soc = SSL_get_fd(s);
	int error_code;
	unsigned int error_code_size = sizeof(error_code);
	getsockopt(soc, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);
	if (error_code!=0)
	{
		this->_log->WriteMessage(ERR,"Unable to send message sockeet is closed");
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Send");
		return (false);
	}
	/*
	if ((Err=send(soc, Message.c_str(), Message.size(), 0))<0)  //odoslanie poziadavky na server
	{
		this->_log->WriteMessage(ERR,"Unable to send message to : " + std::to_string(soc) + "error code : " + std::to_string(errno));
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Connect");
		return (false);
	}*/
	 if((Err=SSL_write(s, Message.c_str(), Message.size()))<=0)
	{
		 std::string msg;
		 int x = SSL_get_error(s,Err);
		 switch(x)
		 {
		 case SSL_ERROR_NONE:
			 _log->WriteMessage(ERR,"Sending message to adapter failed with code : " + std::to_string(x) + "None");
			 break;
		 case SSL_ERROR_ZERO_RETURN:
			 _log->WriteMessage(ERR,"Sending message to adapter failed with code : " + std::to_string(x) + "Zero return");
			 break;
		 case SSL_ERROR_WANT_WRITE:
			 _log->WriteMessage(ERR,"Sending message to adapter failed with code : " + std::to_string(x) + "Want write");
			 break;
		 case SSL_ERROR_WANT_CONNECT:
			 _log->WriteMessage(ERR,"Sending message to adapter failed with code : " + std::to_string(x) + "Want connect");
			 break;
		 case SSL_ERROR_WANT_X509_LOOKUP:
			 _log->WriteMessage(ERR,"Sending message to adapter failed with code : " + std::to_string(x) + "Want lookup");
			 break;
		 case SSL_ERROR_SYSCALL:
			 _log->WriteMessage(ERR,"Sending message to adapter failed with code : " + std::to_string(x) + "Err syscall");
			 break;
		 case SSL_ERROR_SSL:
			 _log->WriteMessage(ERR,"Sending message to adapter failed with code : " + std::to_string(x) + "Err SSL");

		    msg.append(ERR_error_string(x,NULL));
			 _log->WriteMessage(ERR,msg);
			 break;
		 default:
			 _log->WriteMessage(ERR,"Sending message to adapter failed with code : " + std::to_string(x) + "Another");
			 break;
		 }
		//close(com_s);
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Send");
		return (false);
	}
	else
	{
		_log->WriteMessage(INFO,"Response msg successfully sent with number of bytes : " + std::to_string(Err));
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Send");
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
{

}
