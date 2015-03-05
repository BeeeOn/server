

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
	delete this->_L;
	delete this->_log;
}

void AdaServerSender::Start()
{
	if (!this->_L->Listen())
	this->_L->ReciveConnection();
	return;
}












