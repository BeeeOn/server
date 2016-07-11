/**
 * @file conncetionHandler.cpp
 * 
 * @brief implementation of ConncetionHandler Class
 *
 * @author Matus Blaho 
 * @version 1.0
 */

#include "connectionHandler.h"


int ConnectionHandler::Listen ()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::Listen");
	struct sockaddr_in sin;
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
	  this->_log->WriteMessage(FATAL,"Creating socket failed");
	  this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
	  return (1);
	}
	sin.sin_family = PF_INET;
	sin.sin_port = htons (_port);
	sin.sin_addr.s_addr = INADDR_ANY;
	if (bind(s,(struct sockaddr *)&sin , sizeof(sin)) < 0)
	{
	  this->_log->WriteMessage(FATAL,"Error while binding socket with code : " + std::to_string(errno) + " : " + std::strerror(errno));
	  this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
	  return (1);
	}
	else
	{
		this->_log->WriteMessage(INFO,"Socket successfully binded");
	}
	if ((listen (s,100))<0)
	{
	  this->_log->WriteMessage(FATAL,"Error to listen with code : " + std::to_string(errno) + " : " + std::strerror(errno));
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



int ConnectionHandler::ReceiveConnection()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::ReceiveConnection");
	struct sockaddr_in sin;
	int com_s;
	socklen_t s_size;
	s_size=sizeof(sin);
	int fail_count = 0;
	while (1)
	{
		if ((com_s=accept(s,(struct sockaddr *)&sin ,&s_size )) < 0)
		{
			this->_log->WriteMessage(FATAL,"Unable to accept with code : " + std::to_string(errno) + " : " + std::strerror(errno));
            fail_count++;
            if (fail_count<=5)
            {
                usleep (5000);
                this->_log->WriteMessage(FATAL,"Trying to retrieve from accept error try number :" + std::to_string(fail_count) + " of 5");
            }
            else
            {
                this->_log->WriteMessage(FATAL,"Unable to accept max count of consecutive failed retrievigs reached");
                this->_log->WriteMessage(TRACE, "Exiting " + this->_Name + "::ReceiveConnection");
                return (1);
            }
		}
		else
		{
            fail_count = 0;
            sem_wait((this->_semaphore));
            Worker *w  = NULL;
            while (w==NULL)
            {
                w = this->_workers->GetWorker(this->_log);
            }
            w->Unlock(com_s,sin.sin_addr);
		}
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ReceiveConnection");
	return (0);
}



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



