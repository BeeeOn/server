/*
 * conncetionServer.cpp
 *
 *  Created on: Feb 18, 2015
 *      Author: tuso
 */

#include "connectionServer.h"

using namespace soci;
using namespace pugi;

bool ConnectionServer::LoadCertificates()
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



/** Metoda pre prijatie spravy a jej spracovanie a odpovedanie na nu
 * @param IP - ip adresa zariadenia s ktorym sa komunikuje
    */
void ConnectionServer::HandleConnection (in_addr IP)
{
	_log->WriteMessage(TRACE,"Entering " + this->_Name + "::HandleConnection");
	char buffer[1024];// = new char[1024];  //natavime buffer
	//int pollRes;

	/*int use_prv = SSL_CTX_use_PrivateKey_file(sslctx, "/etc/openvpn/server.key", SSL_FILETYPE_PEM);*/
	/*if (use_prv!=1)
	{
		_log->WriteMessage(ERR,"Private key loading error");
	}*/
	ssize_t DataSize=0;
	std::string data;
	data.clear();
	SSL_set_fd(cSSL, com_s );
	int ssl_err = SSL_accept(cSSL);
	if(ssl_err <= 0)
	{
		_log->WriteMessage(ERR,"SSL accept error");
	    //Error occured, log and close down ssl
		SSL_shutdown(this->cSSL);
		SSL_free(this->cSSL);
		cSSL = SSL_new(sslctx);
		 close(com_s);
		 return;
	}
	while(1)
	{
		if((DataSize = SSL_read(cSSL, buffer, 1024))>0)
		{
			data.append(buffer,DataSize);
			if ((data.find("</adapter_server>")!=std::string::npos)||((data[data.size()-2]=='/')&&(data[data.size()-2]=='>')))
			{
				break;
			}
			if(!SSL_pending(cSSL))
			{
				break;
			}
		}
		else
		{
			_log->WriteMessage(ERR,"SSL read error");
			close(com_s);
			 return;
		}
	}
	/*struct pollfd ufds;
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
	}*/
	_log->WriteMessage(MSG,"Message :" + data);
	xml_document doc;
	xml_parse_result result = doc.load_buffer(data.data(), data.size());
	if (result.status!=status_ok)
	{
		std::string tmp = "Wrong message XML format : ";
		tmp.append(result.description());
		_log->WriteMessage(WARN,tmp);
		_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
		close(com_s);
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
					close(com_s);
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
					close(com_s);
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
		close(com_s);
		return;
	}
	if (!MP->ParseMessage(&adapter,FMversion,CPversion))  //pomocou parsera spracujeme spravu
	{

		_log->WriteMessage(WARN,"Wrong request format");
		delete this->MP;
		close(com_s);
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
		/*if ((Err=send(this->com_s, response.c_str(), response.size() , 0))<0)  //odoslanie odpovede klientovi
		{
			_log->WriteMessage(WARN,"Sending answer failed with code : " + std::to_string(errno) + " : " + std::strerror(errno));
		}*/
		if((Err=SSL_write(cSSL, response.c_str(), response.size()))<=0)
		{
			_log->WriteMessage(ERR,"Writing to SSL failed :");
		}
		close(com_s);
	}
	else
	{
	    std::string resp;
	    parsedMessage->socket = 1000;
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
	    	//close (oldSocket);
	    	if (database->UpdateAdapter(this->parsedMessage))
			{
			  resp="<server_adapter protocol_version=\"0.1\" state=\"register\" response=\"true\"/>";
			}
			else
			{
			  resp="<server_adapter protocol_version=\"0.1\" state=\"register\" response=\"false\"/>";
			}

	    }
	    this->_sslcont->InsertSSL(parsedMessage->adapterINTid,cSSL);
	    int Err;
	  /*  if ((Err=send(this->com_s, resp.c_str(), resp.size() , 0))<0)  //odoslanie odpovede klientovi
	    {
			_log->WriteMessage(WARN,"Sending registration answer failed with code : " + std::to_string(errno) + " : " + std::strerror(errno));
	    }*/
		_log->WriteMessage(INFO,"Going to send response to register MSG ");
		_log->WriteMessage(INFO,"Response register MSG : " + resp);
	    if((Err=SSL_write(cSSL, resp.c_str(), resp.size()))<=0)
		{
			_log->WriteMessage(ERR,"Sending registration answer failed :");
			//close(com_s);
		}
		else
		{
			_log->WriteMessage(INFO,"Response msg successfully sent with number of bytes : " + std::to_string(Err));
		}
	    cSSL = SSL_new(sslctx);
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
		_log->WriteMessage(ERR,"Unable to connect to framework");
		close (s);
		return;  //nepodarilo sa pripojit na server
	}
	if ((Err=send(s, MSG.c_str(), MSG.size(), 0))<0)  //odoslanie poziadavky na server
	{
		_log->WriteMessage(ERR,"Unable to send msg to framework");
	}
	close (s);
	_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Notify");
}

ConnectionServer::ConnectionServer(soci::session *SQL, Loger *L, int timeOut,SSLContainer *sslcont)
{
	L->WriteMessage(TRACE,"Entering " + this->_Name + "::Constructor");
	this->MP = nullptr;
	this->_sslcont = sslcont;
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
	close(SSL_get_fd(cSSL));
	SSL_shutdown(this->cSSL);
	SSL_free(this->cSSL);
	ERR_free_strings();
	EVP_cleanup();
	CRYPTO_cleanup_all_ex_data();
	SSL_CTX_free(this->sslctx);
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
