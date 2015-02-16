/**
* @file mifa.cpp
*
* @Implementace modulárního prostøedí pro algoritmy
*
* @author xrasov01
* @version 1.0
*/

#include "mifa.h"

#define DEF_PORT 7082 
//Loger *Log;
ConnectionHandler *CH;


int ConnectionHandler::Listen()  //funkcia na vytvorenie spojenia a komunikaciu s klientom
{
	//Log->WriteMessage(TRACE, "Entering " + this->_Name + "::Listen");
	//signal (SIGCHLD, endchild);
	/*struct sigaction act;

	act.sa_handler = SIG_IGN;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGCHLD, &act, 0);*/
	struct sockaddr_in sin;
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)  //vytvorime socket 
	{
		//Log->WriteMessage(FATAL, "Creating socket failed");  //ak nastala chyba
		//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::Listen");
		return 1;
	}
	std::string opt;
	opt = "tap0";
	if (setsockopt(s, SOL_SOCKET, SO_BINDTODEVICE, opt.c_str(), 4) < 0)
	{
		//Log->WriteMessage(ERR, "Unable to switch socket to VPN tap0");
	}
	sin.sin_family = PF_INET;   //nastavime komunikaciu na internet
	sin.sin_port = htons(_port);  //nastavime port
	sin.sin_addr.s_addr = INADDR_ANY;  //nastavime IP adresu
	if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)  //pripojime socket na port
	{
		//Log->WriteMessage(FATAL, "Error while binding socket");  //ak je obsadeny alebo nieco ine sa nepodari vratime chybu
		//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::Listen");
		return 1;
	}
	else
	{
		//Log->WriteMessage(INFO, "Socket successfully binded");
	}
	if ((listen(s, 10))<0) //chceme na sockete pocuvat 
	{
		//Log->WriteMessage(FATAL, "Unable to listen");
		//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::Listen");
		return 1;
	}
	else
	{
		//Log->WriteMessage(INFO, "Listenin on port :" + std::to_string(_port));
	}
	//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::Listen");
	return 0;
}


/** Metoda pre prijatie pripojenia na sockete servra
*/
int ConnectionHandler::ReciveConnection()
{
	//Log->WriteMessage(TRACE, "Entering " + this->_Name + "::ReciveConnection");
	struct sockaddr_in sin;
	int com_s;  //nastavime si este jeden socket
	socklen_t s_size;
	s_size = sizeof(sin);
	while (1)
	{
		if ((com_s = accept(s, (struct sockaddr *)&sin, &s_size)) < 0)  //budeme na nom prijimat data
		{
			//Log->WriteMessage(FATAL, "Unable to accept");
			//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::ReciveConnection");
			return 1;
		}
		sem_wait(&connectionSem);
		connCount++;
		//Log->WriteMessage(INFO, "CONNECTION COUNT : " + std::to_string(connCount));
		try
		{
			ConnectionServer *CS = new ConnectionServer(com_s);
			std::thread worker(&ConnectionServer::HandleConnectionCover, CS, sin.sin_addr);
			worker.detach();
		}
		catch (std::bad_alloc &e)
		{
			std::string ErrMessage = "Allocation of ConnectionServer Failed : ";
			ErrMessage.append(e.what());
			//Log->WriteMessage(ERR, ErrMessage + "Client won't be served!");
			close(com_s);
		}
		catch (std::exception &e)
		{
			//Log->WriteMessage(ERR, "Client won't be served unable to crate thread!");
			close(com_s);
		}

		//CS->HandleConnectionCover(sin.sin_addr);
		//close (com_s);  //zavreme socket
	}
	//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::ReciveConnection");
	return 0;  //vratime ze je vsetko v poriadku
}

/** Konstruktor objektu vytvoreneho z triedy ConnectionHandler
*/

ConnectionHandler::ConnectionHandler()
{
	//Log->WriteMessage(TRACE, "Entering " + this->_Name + "::Constructor");
	//_port = c->Port();
	//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::Constructor");
}

/** Destruktor objektu vytvoreneho z triedy ConnectionHandler */

ConnectionHandler::~ConnectionHandler()
{
	//Log->WriteMessage(TRACE, "Entering " + this->_Name + "::Destructor");
	close(this->s);
	//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::Destructor");
}

/**********************************************End of ConnectionHandler section******************************************************/

/**********************************************Start of ConnectionServer section******************************************************/

void ConnectionServer::HandleConnectionCover(in_addr IP)
{
	//Log->WriteMessage(TRACE, "Entering " + this->_Name + "::HandleConnectionCover");
	this->start = std::chrono::system_clock::now();
	//Log->WriteMessage(TRACE, "Process created!");
	session *Conn = NULL;
	while (Conn == NULL)
	{
		Conn = cont->GetConnection();
		if (Conn == NULL)
		{
			Log->WriteMessage(INFO, "ALL CONNECTIONS ARE USED!");
		}
	}
	try
	{
		this->database = new DBHandler(Conn, Log);
	}
	catch (std::exception &e)
	{
		//Log->WriteMessage(ERR, "Client won't be served unable to create DBHandler!");
		close(com_s);
		//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::HandleConnectionCover");
		delete (this);
		return;
	}
	this->HandleConnection(IP);
	cont->ReturnConnection(database->ReturnConnection());
	sem_post(&connectionSem);
	auto time = (end - start);
	//Log->WriteMessage(TRACE, "Time to answer : " + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(time).count()) + " microseconds");
	//Log->WriteMessage(TRACE, "Process end!");
	connCount--;
	//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::HandleConnectionCover");
	delete (this);
}
void ConnectionServer::HandleConnection(in_addr IP)
{
	//Log->WriteMessage(TRACE, "Entering " + this->_Name + "::HandleConnection");
	char buffer[1024];  //natavime buffer
	int pollRes;
	ssize_t DataSize = 0;
	std::string data;
	data.clear();
	struct pollfd ufds;
	ufds.fd = com_s;
	ufds.events = POLLIN;
	while (1)
	{
		pollRes = poll(&ufds, 1, c->TimeOut() * 1000);
		if (pollRes == -1)
		{
			//Log->WriteMessage(WARN, "Reading data from client failed!");
			//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::HandleConnection");
			return;
		}
		if (pollRes == 0)
		{
			//Log->WriteMessage(WARN, "Connection to client timed out!");
			//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::HandleConnection");
			return;
		}
		else
		{
			if ((DataSize = read(this->com_s, &buffer, 1024)) < 0) //prijmeme dlzku prichadzajucich dat (2 byty kratkeho integera bez znamienka)
			{
				//Log->WriteMessage(WARN, "Reading data from client failed");
				//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::HandleConnection");
				return;
			}
			if (DataSize == 0)  //ak nic neprislo ukoncime proces
			{
				//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::HandleConnection");
				return;
			}
			data.append(buffer, DataSize);
			if ((data.find("</adapter_server>") != std::string::npos) || ((data[data.size() - 2] == '/') && (data[data.size() - 2] == '>')))
			{
				break;
			}
		}
	}
	Log->WriteMessage(MSG, "Message :" + data);
	xml_document doc;
	xml_parse_result result = doc.load_buffer(data.data(), data.size());
	if (result.status != status_ok)
	{
		std::string tmp = "Wrong message XML format : ";
		tmp.append(result.description());
		//Log->WriteMessage(WARN, tmp);
		//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::HandleConnection");
		return;
	}
	xml_node adapter = doc.child("adapter_server");
	float CPversion = adapter.attribute("protocol_version").as_float();
	int FMversion = adapter.attribute("fw_version").as_int();
	if ((CPversion == (float)0.1) || (CPversion == (float)1.0))
	{
		if (FMversion == 0)
		{
			MP = new ProtocolV1MessageParser();
		}
		else
		{
			if (FMversion == 1)
			{
				try
				{
					MP = new ProtocolV1MessageParser();
				}
				catch (std::exception &e)
				{
					//Log->WriteMessage(ERR, "Unable to create space for Protocol parser exiting client won't be server!");
					//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::HandleConnection");
					return;
				}
			}
			else
			{
				//Log->WriteMessage(WARN, "Unsupported protocol version");
				//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::HandleConnection");
				return;
			}
		}
	}
	else
	{
		//Log->WriteMessage(WARN, "Unsupported protocol version");
		//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::HandleConnection");
		return;
	}
	if (!MP->parseMessage(&adapter, FMversion, CPversion))  //pomocou parsera spracujeme spravu
	{

		//Log->WriteMessage(WARN, "Wrong request format");
		//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::HandleConnection");
		return;
	}
	MP->setAdapterIP(IP);  //ulozime IP adresu adapteru do spravy
	parsedMessage = MP->ReturnMessage();  //spracovanu spravu si ulozime lokalne

	if (parsedMessage->state != 0)
	{
		this->Notify();
	}
	//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::HandleConnection");
	return;
}

void ConnectionServer::Notify()
{
	//Log->WriteMessage(TRACE, "Entering " + this->_Name + "::Notify");
	for (int i = 0; i<this->parsedMessage->values_count; i++)
	{
		if (this->parsedMessage->values[i].type != TEMP)
		{
			continue;
		}
		else
		{
			if (this->parsedMessage->values[i].fval<30.0)
			{
				continue;
			}
			else
			{
				float lastVal = this->database->GetLastTemp(this->parsedMessage->DeviceIDstr, std::to_string(this->parsedMessage->values[i].intType));
				//Log->WriteMessage(TRACE, "Last val : " + std::to_string(lastVal));
				if (lastVal>30.0)
				{
					continue;
				}
				else
				{
					std::vector<std::string> *emails;
					emails = database->GetEmails(std::to_string(this->parsedMessage->adapterINTid));
					if (emails == nullptr)
					{
						continue;
					}
					else
					{
						for (int i = 0; i<emails->size(); i++)
						{
							if (emails->at(i).empty())
							{
								break;
							}
							std::vector<std::string> *IDs;
							IDs = database->GetNotifString(emails->at(i));
							if (IDs == nullptr)
							{
								Log->WriteMessage(INFO, "Email " + emails->at(i) + " has no notification string");
								continue;
							}
							//Log->WriteMessage(INFO, "Notifying email " + emails->at(i));
							Notification *notif = new LimitExceededNotification(emails->at(i), counter++, *IDs, this->parsedMessage->timestamp, "Temperature exceeded 30 degrees on adapter :" + std::to_string(this->parsedMessage->adapterINTid) + " on device " + std::to_string(this->parsedMessage->sensor_id), 123, this->parsedMessage->DeviceIDstr, 1, 2);
							Notificator::sendNotification(*notif);
							delete IDs;
							delete notif;
						}
					}
				}
			}
		}
	}
	//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::Notify");
}

tmessage* MessageParser::ReturnMessage()
{
	//Log->WriteMessage(TRACE, "Entering " + this->_Name + "::ReturnMessage");
	//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::ReturnMessage");
	return _message;

}


ConnectionServer::ConnectionServer(int s)
{
	//Log->WriteMessage(TRACE, "Entering " + this->_Name + "::Constructor");
	this->com_s = s;
	this->MP = NULL;
	this->database = NULL;
	//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::Constructor");
}

ConnectionServer::~ConnectionServer()
{
	//Log->WriteMessage(TRACE, "Entering " + this->_Name + "::Destructor");
	close(this->com_s);
	delete (this->MP);
	delete this->database;
	//Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::Destructor");
}


/** Metoda pre prijatie spravy a jej spracovanie a odpovedanie na nu
* @param IP - ip adresa zariadenia s ktorym sa komunikuje
*/
int main()
{
	/*
	try
	{
		Log = new Loger();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << "Unable to create memory space for logging exiting!" << std::endl;
		exit(EXIT_FAILURE);
	}
	*/
	int semVal = cont->Limit();
	//Log->WriteMessage(FATAL, "Maximal conn count : " + std::to_string(semVal));
	sem_init(&connectionSem, 0, semVal);
	try
	{
		CH = new ConnectionHandler();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << "Unable to create memory space for ConnectionHandler exiting!" << std::endl;
		exit(EXIT_FAILURE);
	}
	CH->Listen();  //zacneme pocuvat na porte
	CH->ReciveConnection();  //zacneme prijimat a obsluhovat pripojenia
	//delete (CH);
	delete(cont);
	delete (CH);
	delete (Log);

	return 0;
}

