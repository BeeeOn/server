/**
 * @file connectionServer.cpp
 * 
 * @brief implementation of ConnectionServer Class
 *
 * @author Matus Blaho 
 * @version 1.0
 */

#include "connectionServer.h"

using namespace soci;
using namespace pugi;

void apps_ssl_info_callback()
{
	std::cout<<"CAllback called!"<<"\n";
}

bool ConnectionServer::LoadCertificates()
{
	_log->WriteMessage(TRACE,"Entering " + this->_Name + "::LoadCertificates");
	SSL_load_error_strings();
	SSL_library_init();
	OpenSSL_add_all_algorithms();
	sslctx = SSL_CTX_new( TLSv1_server_method());
	char errorBuffer[1000];
	std::string msgBase = "Certificate error ";
	unsigned long int err;
	  /* load CA certificate*/
	if ((err=SSL_CTX_load_verify_locations(sslctx, _config->CApath().c_str(), NULL)) != 1)
	{
		err = ERR_get_error();
		ERR_error_string_n(err,errorBuffer,1000);
		_log->WriteMessage(FATAL,msgBase + " in verify locations " + errorBuffer);
		return (false);
	}
		/* verify paths*/
	if ((err=SSL_CTX_set_default_verify_paths(sslctx)) != 1)
	{
		err = ERR_get_error();
		ERR_error_string_n(err,errorBuffer,1000);
		_log->WriteMessage(FATAL,msgBase + " in verify def paths " + errorBuffer);
				return (false);
	}
		/* set the local certificate from CertFile */
	if ((err=SSL_CTX_use_certificate_chain_file(sslctx, _config->CRTPath().c_str())) <= 0)
	{
		err = ERR_get_error();
		ERR_error_string_n(err,errorBuffer,1000);
		_log->WriteMessage(FATAL,msgBase + " use chain file " + errorBuffer);
				return (false);
	}
		/* set the private key from KeyFile (may be the same as CertFile) */
	if ((err=SSL_CTX_use_PrivateKey_file(sslctx, _config->KeyPath().c_str(), SSL_FILETYPE_PEM)) <= 0)
	{
		err = ERR_get_error();
		ERR_error_string_n(err,errorBuffer,1000);
		_log->WriteMessage(FATAL,msgBase + " in verify use private key "+ errorBuffer);
				return (false);
	}
		/* verify private key */
	if (!SSL_CTX_check_private_key(sslctx))
	{
		_log->WriteMessage(FATAL, "Private key does not match the public certificate");
		return (false);
	}
	SSL_CTX_set_verify(sslctx,SSL_VERIFY_PEER,NULL);
	//void SSL_CTX_set_info_callback((void *)sslctx,(void *)&apps_ssl_info_callback);
	_log->WriteMessage(INFO, "Certificates successfully loaded");
	cSSL = SSL_new(sslctx);
	_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::LoadCertificates");
	return (true);
}



void ConnectionServer::HandleConnection (in_addr IP)
{
	_log->WriteMessage(TRACE,"Entering " + this->_Name + "::HandleConnection");

	char buffer[1024];
	char errorBuffer[1000];
	ssize_t DataSize=0;
	std::string data;
	data.clear();
	SSL_set_fd(cSSL, com_s );
	//SSL_set_verify(cSSL,SSL_VERIFY_PEER|SSL_VERIFY_FAIL_IF_NO_PEER_CERT,NULL);
	X509 *peer = SSL_get_peer_certificate(cSSL);
	int ssl_err = SSL_accept(cSSL); //SSL handshake 
	if(ssl_err <= 0)
	{
		ssl_err = ERR_get_error();
		std::string MSGbase = "SSL accept error : ";
		ERR_error_string_n(ssl_err,errorBuffer,1000);
		_log->WriteMessage(ERR,MSGbase + errorBuffer);
	    //Error occured, log and close down ssl
		SSL_shutdown(this->cSSL);
		SSL_free(this->cSSL);
		cSSL = SSL_new(sslctx);
		close(com_s);
		return;
	}
	peer=SSL_get_peer_certificate(cSSL);
	if(peer==NULL)
	{
		_log->WriteMessage(WARN,"Peer did not provided certificate!");
	}
	else
	{
		std::string hash;
		hash+=(*(peer->sha1_hash));
		_log->WriteMessage(TRACE,"Clients hash is : " + hash);
		char issuer[100];
		X509_NAME_oneline(X509_get_issuer_name(peer), issuer, 100);
		printf("issuer= %s\n", issuer);
		std::cout<<issuer<<"\n";
		std::string iss = issuer;
		_log->WriteMessage(TRACE,"Certificate was issued by : " +  iss);
		X509_free(peer);
	}
	while(1)
	{
		if((DataSize = SSL_read(cSSL, buffer, 1024))>0) //read message
		{
			data.append(buffer,DataSize);
			//_log->WriteMessage(MSG,"Message :" + data);
			if ((data.find("</adapter_server>")!=std::string::npos)||((data[data.size()-2]=='/')&&(data[data.size()-2]=='>')))
			{
				break;
			}
			if(!SSL_pending(cSSL)) //if there is nothing to read break;
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
			close(com_s);
			 return;
		}
	}
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
	if ((CPversion == (float)0.1)||(CPversion == (float)1.0))  //according to version create parser
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
	if (!MP->ParseMessage(&adapter,FMversion,CPversion))  //parse message
	{

		_log->WriteMessage(WARN,"Wrong request format");
		delete this->MP;
		close(com_s);
		_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
		return;

	}
	parsedMessage = MP->ReturnMessage();

	if (parsedMessage->state!=REGISTER) //on registration register agent
	{
		response = MP->CreateAnswer(this->GetData());
		int Err;
		_log->WriteMessage(MSG,"Response message: \n" + response);

		// It was decided with adapter team, that at the end of each message will be \0
		response.push_back('\0');

		if((Err=SSL_write(cSSL, response.c_str(), response.size()))<=0)
		{
			_log->WriteMessage(ERR,"Writing to SSL failed :");
		}
		close(com_s);
	}
	else
	{
	    std::string resp;
	    if (!database->IsInDB("gateway","gateway_id",std::to_string(parsedMessage->adapterINTid)))
	    {
			if (database->InsertGateway(this->parsedMessage))
			{
			  resp="<server_adapter protocol_version=\"1.0\" state=\"register\" response=\"true\"/>";
			}
			else
			{
			  resp="<server_adapter protocol_version=\"1.0\" state=\"register\" response=\"false\"/>";
			}
	    }
	    else
	    {
	    	if (database->UpdateGateway(this->parsedMessage))
			{
			  	resp="<server_adapter protocol_version=\"1.0\" state=\"register\" response=\"true\"/>";
			}
			else
			{
			  	resp="<server_adapter protocol_version=\"1.0\" state=\"register\" response=\"false\"/>";
			}

	    }

	    this->_sslcont->InsertSSL(parsedMessage->adapterINTid,cSSL);
	    int Err;
		_log->WriteMessage(INFO,"Going to send response to register MSG ");
		_log->WriteMessage(INFO,"Response register MSG : " + resp);

		// It was decided with adapter team, that at the end of each message will be \0
	   	response.push_back('\0');

	    if((Err=SSL_write(cSSL, resp.c_str(), resp.size()))<=0)
		{
			_log->WriteMessage(ERR,"Sending registration answer failed :");
		}
		else
		{
			_log->WriteMessage(INFO,"Response msg successfully sent with number of bytes : " + std::to_string(Err));
		}
	    cSSL = SSL_new(sslctx);
	}
	if (parsedMessage->state!=REGISTER) //we are receiving data so save them
	{
		this->Notify(data);
		usleep(100000);
		this->StoreData();
		database->LogValue(parsedMessage);
	}
	_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
	return;
}

void ConnectionServer::Notify(std::string MSG)
{
	sockaddr_in SvSoc;
	_log->WriteMessage(TRACE,"Entering " + this->_Name + "::Notify");
	int s,Err;
	s = socket(AF_INET, SOCK_STREAM, 0);
	if(s < 0)
	{
		std::cerr<<"Creating socket failed\n";
		return;
	}
	SvSoc.sin_port = htons(7083);
	SvSoc.sin_family = AF_INET;
	in_addr adapterIP;
	inet_pton(AF_INET, "127.0.0.1",&adapterIP);
	SvSoc.sin_addr.s_addr = adapterIP.s_addr;
	if(connect(s,(sockaddr *) &SvSoc, sizeof(SvSoc)) < 0)
	{
		_log->WriteMessage(ERR,"Unable to connect to framework");
		close (s);
		return;
	}
	if ((Err=send(s, MSG.c_str(), MSG.size(), 0))<0)
	{
		_log->WriteMessage(ERR,"Unable to send msg to framework");
	}
	close (s);
	_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Notify");
}

ConnectionServer::ConnectionServer(soci::session *SQL, Loger *L, int timeOut,SSLContainer *sslcont,Config *c)
{
	L->WriteMessage(TRACE,"Entering " + this->_Name + "::Constructor");
	this->MP = nullptr;
	this->_sslcont = sslcont;
	this->database = new DBHandler(SQL,L);
	this->_log = L;
	this->_timeTimeOut = timeOut;
	this->parsedMessage = NULL;
	this->com_s = -1;
	this->_config = c;
	this->cSSL = NULL;
	this->sslctx = NULL;
	_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Constructor");
}

ConnectionServer::~ConnectionServer()
{
	_log->WriteMessage(TRACE,"Entering " + this->_Name + "::Destructor");
	if (cSSL != NULL)
	{
		close(SSL_get_fd(cSSL));
		SSL_shutdown(this->cSSL);
		SSL_free(this->cSSL);
	}
	if(sslctx!=NULL)
	{
		SSL_CTX_free(this->sslctx);
	}
	CRYPTO_cleanup_all_ex_data();
	ERR_remove_state(0);
	ERR_free_strings();
	EVP_cleanup();
	delete this->database;
	_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Destructor");
}

void ConnectionServer::StoreData()
{
	_log->WriteMessage(TRACE,"Entering " + this->_Name + "::StoreData");
	database->UpdateGateway(this->parsedMessage); 
	if(!database->IsInDB("device","device_euid","'" + std::to_string(this->parsedMessage->device_euid) + "'")) 
	{
		database->InsertSenAct(this->parsedMessage);
	}
	else
	{
		database->UpdateSenAct(this->parsedMessage);
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
	if(!database->IsInDB("device","device_euid","'" + std::to_string(this->parsedMessage->device_euid) + "'"))
	{
		wakeUpTime = 5;
	}
	else
	{
		wakeUpTime = database->GetWakeUpTime(std::to_string(this->parsedMessage->device_euid));
	}
	_log->WriteMessage(TRACE,"refresh:" + std::to_string(wakeUpTime));
	if(wakeUpTime> 100000)
		wakeUpTime = 10;
	_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetData");
	return (wakeUpTime);
}
