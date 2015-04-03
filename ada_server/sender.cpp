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

bool Sender::LoadCertificates()
{
	_log->WriteMessage(TRACE,"Entering " + this->_Name + "::LoadCertificates");
	SSL_load_error_strings();
	SSL_library_init();
	OpenSSL_add_all_algorithms();
	sslctx = SSL_CTX_new( TLSv1_server_method());
	/*SSL_CTX_set_options(sslctx, SSL_OP_SINGLE_DH_USE);
	int use_cert = SSL_CTX_use_certificate_file(sslctx, "/etc/openvpn/server.crt" , SSL_FILETYPE_PEM);*/
	char errorBuffer[1000];
	std::string msgBase = "Certificate error ";
	unsigned long int err;
	if ((err=SSL_CTX_load_verify_locations(sslctx, "ca.crt", NULL)) != 1)
	{
		ERR_error_string_n(err,errorBuffer,1000);
		_log->WriteMessage(FATAL,msgBase + " in verify locations" + errorBuffer);
		return (false);
	}
	if ((err=SSL_CTX_set_default_verify_paths(sslctx)) != 1)
	{
		ERR_error_string_n(err,errorBuffer,1000);
		_log->WriteMessage(FATAL,msgBase + " in verify def paths" + errorBuffer);
				return (false);
	}
		/* set the local certificate from CertFile */
	if ((err=SSL_CTX_use_certificate_chain_file(sslctx, "/etc/openvpn/server.crt")) <= 0)
	{
		ERR_error_string_n(err,errorBuffer,1000);
		_log->WriteMessage(FATAL,msgBase + " use chain file" + errorBuffer);
				return (false);
	}
		/* set the private key from KeyFile (may be the same as CertFile) */
	if ((err=SSL_CTX_use_PrivateKey_file(sslctx, "/etc/openvpn/server.key", SSL_FILETYPE_PEM)) <= 0)
	{
		ERR_error_string_n(err,errorBuffer,1000);
		_log->WriteMessage(FATAL,msgBase + " in verify use private key"+ errorBuffer);
				return (false);
	}
		/* verify private key */
	if (!SSL_CTX_check_private_key(sslctx))
	{
		_log->WriteMessage(FATAL, "Private key does not match the public certificate");
		return (false);
	}
	_log->WriteMessage(INFO, "Certificates successfully loaded");
	cSSL = SSL_new(sslctx);
	_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::LoadCertificates");
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
	SSL_shutdown(this->cSSL);
	SSL_free(this->cSSL);
}
