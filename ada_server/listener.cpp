/**
 * @file listener.cpp
 * 
 * @brief implementation of Listener class
 *
 * @author Matus Blaho 
 * @version 1.0
 */



#include "util.h"
#include "listener.h"

int Listener::Listen ()  
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::Listen");
	struct sockaddr_in sin;
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)  
	{
	  this->_log->WriteMessage(FATAL,"Creating socket failed");  
	  this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
	  return (1);
	}
	sin.sin_family = PF_INET;   //set communicaton to internet
	sin.sin_port = htons (_port);  //set port
	sin.sin_addr.s_addr = INADDR_ANY;  //set IP address
	if (bind(s,(struct sockaddr *)&sin , sizeof(sin)) < 0)  //bind socket
	{
	  this->_log->WriteMessage(FATAL,"Error while binding socket");  //on error
	  this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
		this->_terminated = true;
	  return (1);
	}
	else
	{
		this->_log->WriteMessage(MSG,"Socket successfully binded");
	}

	if (makeSocketReusable(s) < 0)
		this->_log->WriteMessage(ERR, "Failed to set socket reusable: " + std::to_string(errno));

	if ((listen (s,10))<0) //start listening on socket
	{
		this->_log->WriteMessage(FATAL,"Unable to listen");
		this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
		this->_terminated = true;
		return (1);
	}
	else
	{
		this->_log->WriteMessage(MSG,"Listenin on port :" + std::to_string(_port));
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
	return (0);
}

int Listener::handleAcceptFailed(int &failcounter)
{
	this->_log->WriteMessage(FATAL,"Error when accepting code : " + std::to_string(errno) + " : " + std::strerror(errno) );
	if (!this->_toBeTerminated)
	{
		failcounter++;
		if (failcounter<=5)
		{
			usleep (5000);
			this->_log->WriteMessage(FATAL,"Trying to retrieve from accept error try number :" + std::to_string(failcounter) + " of 5");
			return 0;
		}
		else
		{
			this->_log->WriteMessage(FATAL,"Unable to accept max count of consecutive failed retrievigs reached");
			this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::ReceiveConnection");
			this->_terminated = true;
			return (1);
		}
	}
	else
	{
		this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::ReceiveConnection");
		this->_terminated = true;
		return (1);
	}
}

void Listener::acceptClient(int sock)
{
	sem_wait((this->_semapohore)); //decrease count of workers or wait for free worker if there is none

	Worker *w  = NULL;
	while (w==NULL)  //just for sure try to get worker until it is not NULL
	{
		w = this->_workers->GetWorker(this->_log); //pick worker
	}

	w->Unlock(sock);
}

/** Metoda pre prijatie pripojenia na sockete servra
    */
int Listener::ReceiveConnection()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::ReceiveConnection");
	struct sockaddr_in sin;
	int com_s;  //set communincation socket
	socklen_t s_size;
	s_size=sizeof(sin);
	int failcounter = 0;
	while (1)
	{
		if ((com_s=accept(s,(struct sockaddr *)&sin ,&s_size )) < 0)  //wait for client to connect
		{
			if (handleAcceptFailed(failcounter) == 1)
				return 1;
		}

		acceptClient(com_s);
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ReceiveConnection");
	return (0); 
}

/** Konstruktor objektu vytvoreneho z triedy Listener
    */

Listener::Listener(Loger *l,int Port,sem_t *semaphore, WorkerPool *wp)
{
	l->WriteMessage(TRACE,"Entering " + this->_Name + "::Listener");
	_port = Port;
	_log = l;
	s = -1;
	this->_toBeTerminated=false;
	this->_workers=wp;
	this->_terminated = false;
	this->_semapohore = semaphore;
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listener");
}

/** Destruktor objektu vytvoreneho z triedy Listener */

Listener::~Listener()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::~Listener");
	this->_toBeTerminated = true;
	this->UnAccept();
	while (!this->_terminated)
	{
		sleep(1);
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::~Listener");
}

void Listener::UnAccept()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::UnAccept");
	shutdown (s,SHUT_RDWR);
	close(this->s);
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::UnAccept");
}
