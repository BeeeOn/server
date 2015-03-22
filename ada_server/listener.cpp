/*
 * Listener.cpp
 *
 *  Created on: Feb 22, 2015
 *      Author: tuso
 */


#include "listener.h"

using namespace pugi;

int Listener::Listen ()  //funkcia na vytvorenie spojenia a komunikaciu s klientom
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::Listen");
	struct sockaddr_in sin;
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)  //vytvorime socket
	{
	  this->_log->WriteMessage(FATAL,"Creating socket failed");  //ak nastala chyba
	  this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
	  return (1);
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
		this->_log->WriteMessage(MSG,"Socket successfully binded");
	}
	if ((listen (s,10))<0) //chceme na sockete pocuvat
	{
	  this->_log->WriteMessage(FATAL,"Unable to listen");
	  this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
	  return (1);
	}
	else
	{
		this->_log->WriteMessage(MSG,"Listenin on port :" + std::to_string(_port));
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
	return (0);
}


/** Metoda pre prijatie pripojenia na sockete servra
    */
int Listener::ReciveConnection()
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
			/*char errorbuf[200];
			strerror_r(errno,errorbuf,200);*/
			std::cerr<<std::strerror(errno);
			this->_log->WriteMessage(FATAL,"Error when accepting code : " + std::to_string(errno) + " : " + std::strerror(errno) );
			this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ReciveConnection");
			this->terminated = true;
			return (1);
		}
		//sem_wait(&connectionSem);
		sem_wait((this->_semapohore));
		Worker *w  = NULL;
		while (w==NULL)
		{
			w = this->_workers->GetWorker(this->_log);
		}
		w->Unlock(com_s);

	//close (com_s);  //zavreme socket
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ReciveConnection");
	return (0);  //vratime ze je vsetko v poriadku
}

/** Konstruktor objektu vytvoreneho z triedy Listener
    */

Listener::Listener(Loger *l,int Port,sem_t *semaphore, WorkerPool *wp)
{
	l->WriteMessage(TRACE,"Entering " + this->_Name + "::Listener");
	_port = Port;
	_log = l;
	s = -1;
	this->_workers=wp;
	this->terminated = false;
	this->_semapohore = semaphore;
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listener");
}

/** Destruktor objektu vytvoreneho z triedy Listener */

Listener::~Listener()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::~Listener");
	this->UnAccept();
	while (!this->terminated)
	{
		sleep(1);
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::~Listener");
}

void Listener::UnAccept()
{
	shutdown (s,SHUT_RDWR);
	close(this->s);
}
