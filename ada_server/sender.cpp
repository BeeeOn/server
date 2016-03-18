/**
 * @file sender.cpp
 * 
 * @brief implementation of sender class
 *
 * @author Matus Blaho 
 * @version 1.0
 */

#include "sender.h"


int Sender::Send(std::string Message,SSL *s) //pripojenie na server a komunikacia s nim
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
		return (2);
	}
	getsockopt(soc, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);  //check socket for errors before writing to it
	if (error_code!=0)
	{
		this->_log->WriteMessage(ERR,"Unable to send message socket is closed");
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Send");
		SSL_set_fd(s,-1);
		return (2);
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
		ERR_free_strings();
		ERR_remove_state(0);
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Send");
		return (3);
	}
	else
	{
		_log->WriteMessage(INFO,"Response msg successfully sent with number of bytes : " + std::to_string(Err));
	}
	CRYPTO_cleanup_all_ex_data();
	ERR_free_strings();
	ERR_remove_state(0);
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Send");
	return (0);
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

int Sender::Receveive(SSL *s)
{
	unsigned long ssl_err;
	ssize_t DataSize=0;
	std::string data;
	data.clear();
	char buffer[1024];
	char errorBuffer[1000];
	struct timeval interval = {5,  1000};
	int socket = SSL_get_fd(s);
	setsockopt(socket , SOL_SOCKET, SO_RCVTIMEO, (char *)&interval, sizeof(struct timeval));
	while(1)
	{
		if((DataSize = SSL_read(s, buffer, 1024))>0) //read message
		{
			data.append(buffer,DataSize);
			//_log->WriteMessage(MSG,"Message :" + data);
			if ((data.find("</adapter_server>")!=std::string::npos)||((data[data.size()-2]=='/')&&(data[data.size()-2]=='>')))
			{
				break;
			}
			if(!SSL_pending(s)) //if there is nothing to read break;
			{
				break;
			}
		}
		else
		{
			ssl_err = ERR_get_error();
			std::string MSGbase = "SSL read error : ";
			ERR_error_string_n(ssl_err,errorBuffer,1000);
			_log->WriteMessage(ERR,MSGbase + errorBuffer);
			ERR_free_strings();
			ERR_remove_state(0);
			return 4;
		}
	}
	return 0;
}
