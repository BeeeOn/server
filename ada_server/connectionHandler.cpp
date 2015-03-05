/*
 * connectionHandler.cpp
 *
 *  Created on: Feb 18, 2015
 *      Author: tuso
 */

#include "connectionHandler.h"

/** Metoda pre nastavenie serveroveho socketu a pocuvanie na nom
    */
int ConnectionHandler::Listen ()  //funkcia na vytvorenie spojenia a komunikaciu s klientom
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::Listen");
	struct sockaddr_in sin;
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)  //vytvorime socket
	{
	  this->_log->WriteMessage(FATAL,"Creating socket failed");  //ak nastala chyba
	  this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
	  return (1);
	}
	std::string opt;
	opt = "tap0";
	if (setsockopt(s, SOL_SOCKET, SO_BINDTODEVICE, opt.c_str(), 4) < 0)
	{
		this->_log->WriteMessage(ERR,"Unable to switch socket to VPN tap0");
	}
	sin.sin_family = PF_INET;   //nastavime komunikaciu na internet
	sin.sin_port = htons (_port);  //nastavime port
	sin.sin_addr.s_addr = INADDR_ANY;  //nastavime IP adresu
	if (bind(s,(struct sockaddr *)&sin , sizeof(sin)) < 0)  //pripojime socket na port
	{
	  this->_log->WriteMessage(FATAL,"Error while binding socket");  //ak je obsadeny alebo nieco ine sa nepodari vratime chybu
	  this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
	  return (1);
	}
	else
	{
		this->_log->WriteMessage(INFO,"Socket successfully binded");
	}
	if ((listen (s,10))<0) //chceme na sockete pocuvat
	{
	  this->_log->WriteMessage(FATAL,"Unable to listen");
	  this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
	  return (1);
	}
	else
	{
		this->_log->WriteMessage(INFO,"Listenin on port :" + std::to_string(_port));
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
	return (0);
}


/** Metoda pre prijatie pripojenia na sockete servra
    */
int ConnectionHandler::ReciveConnection()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::ReciveConnection");
	struct sockaddr_in sin;
	int com_s;  //nastavime si este jeden socket
	socklen_t s_size;
	s_size=sizeof(sin);
	while (1)
	{
		if ((com_s=accept(s,(struct sockaddr *)&sin ,&s_size )) < 0)  //budeme na nom prijimat data
		{
			this->_log->WriteMessage(FATAL,"Unable to accept");
			this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ReciveConnection");
			return (1);
		}
		sem_wait((this->_semaphore));
		Worker *w  = NULL;
		while (w==NULL)
		{
			w = this->_workers->GetWorker(this->_log);
		}
		w->Unlock(com_s,sin.sin_addr);
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ReciveConnection");
	return (0);  //vratime ze je vsetko v poriadku
}

/** Konstruktor objektu vytvoreneho z triedy ConnectionHandler
    */

ConnectionHandler::ConnectionHandler(Loger *L, int port,sem_t *sem, WorkerPool *wp)
{
	L->WriteMessage(TRACE,"Entering " + this->_Name + "::Constructor");
	_port = port;
	_log = L;
	s = -1;
	this->_semaphore = sem;
	this->_workers = wp;
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Constructor");
}

/** Destruktor objektu vytvoreneho z triedy ConnectionHandler */

ConnectionHandler::~ConnectionHandler()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::Destructor");

	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Destructor");
}

void ConnectionHandler::UnbindSocket()
{
	shutdown (s,SHUT_RDWR);
	close(this->s);
}

	/**********************************************End of ConnectionHandler section******************************************************/


