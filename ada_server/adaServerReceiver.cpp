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


message::message()
{
	this->values = NULL;
	this->devType = UNDEF;
	this->DeviceIDstr = "";
	this->adapterINTid = 0;
	this->socket = 0;
	this->battery = 0;
	this->cp_version = 0.0;
	this->signal_strength=0;
	this->state = UNKNOWN;
	this->values_count = 0;
	this->fm_version = 0;
	this->sensor_id = 0;
	this->timestamp = 0;

}

message::~message()
{
	delete [] values;
	values= NULL;
}

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
	if (this->_CH->Listen()==0)
		this->_CH->ReciveConnection();
	return;
}



