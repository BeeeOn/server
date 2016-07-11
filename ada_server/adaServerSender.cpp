/**
 * @file adaServerSender.cpp
 * 
 * @brief implementation of AdaServerSender Class
 *
 * @author Matus Blaho 
 * @version 1.0
 */

#include "adaServerSender.h"

using namespace soci;
using namespace pugi;

AdaServerSender::AdaServerSender(sem_t *Sem, WorkerPool *workers, Loger *L,Config *c)
{
	L->WriteMessage(TRACE,"Entering " + this->_Name + "Constructor");
	this->_L = new Listener(L, c->SenderPort(),Sem,workers);
	this->_log = L;
	this->conf = c;
	this->workers = workers;
	_log->WriteMessage(TRACE,"Exiting " + this->_Name + "Constructor");
}

AdaServerSender::~AdaServerSender()
{
	_log->WriteMessage(TRACE,"Entering " + this->_Name + "Destructor");
	delete this->_L;
	_log->WriteMessage(TRACE,"Exiting " + this->_Name + "Destructor");
}

void AdaServerSender::Start()
{
	_log->WriteMessage(INFO, "[Main Process] Starting Sender");
	if (this->_L->Listen()==0)
	this->_L->ReciveConnection();
	return;
}












