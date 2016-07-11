/**
 * @file adaServerReceiver.cpp
 * 
 * @brief implementation of AdaServerReceiver Class
 *
 * @author Matus Blaho 
 * @version 1.0
 */

#include "adaServerReceiver.h" 

using namespace soci;
using namespace pugi;

AdaServerReceiver::AdaServerReceiver(sem_t *Sem, WorkerPool *workers, Loger *L,Config *c)
{
	L->WriteMessage(TRACE,"Entering " + this->_Name + "Constructor");
	this->_CH = new ConnectionHandler(L, c->ReceiverPort(),Sem,workers);
	this->_log = L;
	this->conf = c;
	this->workers = workers;
	_log->WriteMessage(TRACE,"Exiting " + this->_Name + "Constructor");
}

AdaServerReceiver::~AdaServerReceiver()
{
	delete this->_CH;
	//delete this->_log;
}

void AdaServerReceiver::Start()
{
	_log->WriteMessage(INFO, "[Main Process] Starting Receiver");
	if (this->_CH->Listen()==0)
		this->_CH->ReceiveConnection();
	return;
}



