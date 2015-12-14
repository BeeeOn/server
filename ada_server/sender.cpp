/**
 * @file sender.cpp
 * 
 * @brief implementation of sender class
 *
 * @author Matus Blaho 
 * @version 1.0
 */

#include "sender.h"


bool Sender::Send(std::string Message,SSL *s) //pripojenie na server a komunikacia s nim
{
	int Err;
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::Send");
	int soc = SSL_get_fd(s);
	int error_code;
	unsigned int error_code_size = sizeof(error_code);
	if (soc==-1) //if is socket set to -1 it was closed time ago
	{
		this->_log->WriteMessage(ERR,"Unable to send message socket is closed");
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Send");
		return (false);
	}
	getsockopt(soc, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);  //check socket for errors before writing to it
	if (error_code!=0)
	{
		this->_log->WriteMessage(ERR,"Unable to send message socket is closed");
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Send");
		SSL_set_fd(s,-1);
		return (false);
	}
	
	// It was decided with adapter team, that at the end of each message will be \0
	Message.push_back('\0');

	 if((Err=SSL_write(s, Message.c_str(), Message.size()))<=0) //finally sent message
	{
		 std::string msg;
		 int x = SSL_get_error(s,Err);
		 switch(x) //parse SSL error
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
		CRYPTO_cleanup_all_ex_data();
		ERR_free_strings();
		ERR_remove_state(0);
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Send");
		return (false);
	}
	else
	{
		_log->WriteMessage(INFO,"Response msg successfully sent with number of bytes : " + std::to_string(Err));
	}
	CRYPTO_cleanup_all_ex_data();
	ERR_free_strings();
	ERR_remove_state(0);
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
