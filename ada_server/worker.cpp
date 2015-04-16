/*
 * worker.cpp
 *
 *  Created on: Feb 16, 2015
 *      Author: Matus Blaho
 */

#include "workerPool.h"

void Worker::SimpleMothod()
{
	sleep(3);
	this->_log->WriteMessage(INFO,"Message from worker");
}
void Worker::Work()
{
	this->SenderLog->WriteMessage(TRACE,"Entering " + this->_Name + "::Work");
	this->ReceiverLog->WriteMessage(TRACE,"Entering " + this->_Name + "::Work");
	this->SenderLog->WriteMessage(INFO,"Worker " + std::to_string(this->_number) + " started");
	this->ReceiverLog->WriteMessage(INFO,"Worker " + std::to_string(this->_number) + " started");
	while (1)
	{
		this->Wait->lock();
		if (this->terminate)
		{
			this->SenderLog->WriteMessage(INFO,"Termination received ending worker " + std::to_string(this->_number) + " body");
			this->ReceiverLog->WriteMessage(INFO,"Termination received" + std::to_string(this->_number) + " ending worker body");
			this->SenderLog->WriteMessage(TRACE,"Exiting" + this->_Name + "::Work");
			this->ReceiverLog->WriteMessage(TRACE,"Exiting" + this->_Name + "::Work");
			return;
		}
		if (this->Receiver)
		{
			this->_CS->SetSocket(this->Socket);
			this->_CS->HandleConnection(this->IPaddress);
		}
		else
		{
			this->_RS->SetSocket(this->Socket);
			this->_RS->HandleRequest();
		}
		//this->SimpleMothod();
		this->parentPool->ReturnWorker(this,this->_log);
	}
}
Worker::Worker (soci::session *s,Loger *Rl,  Loger *Sl, WorkerPool *pool, int i,SSLContainer *sslcont,Config *c)
{
	Sl->WriteMessage(TRACE,"Entering" + this->_Name + "::Constructor");
	Rl->WriteMessage(TRACE,"Entering" + this->_Name + "::Constructor");
	this->parentPool = pool;
	this->connection=s;
	this->DB = s;
	this->_log=Rl;
	this->ReceiverLog = Rl;
	this->SenderLog = Sl;
	this->Wait = new std::mutex();
	this->Socket = -1;
	this->terminate = false;
	this->_number = i;
	thr = NULL;
	this->_RS = new RequestServer(Sl,s,sslcont);
	this->_CS = new ConnectionServer(s,Rl,5,sslcont,c);
	this->_CS->LoadCertificates();
	this->Receiver = true;
	Sl->WriteMessage(TRACE,"Exiting" + this->_Name + "::Constructor");
	Rl->WriteMessage(TRACE,"Exiting" + this->_Name + "::Constructor");
}

void Worker::Unlock(int Soc, in_addr IP)
{
	this->ReceiverLog->WriteMessage(TRACE,"Entering" + this->_Name + "::Unlock");

	this->_log = this->ReceiverLog;
	this->Socket = Soc;
	this->IPaddress = IP;
	this->Receiver = true;
	this->Wait->unlock();
	this->_log->WriteMessage(TRACE,"Unlocking worker number " + std::to_string(this->_number));
	this->_log->WriteMessage(TRACE,"Entering" + this->_Name + "::Unlock");
}

void Worker::Unlock(int Soc)
{
	this->SenderLog->WriteMessage(TRACE,"Entering" + this->_Name + "::Unlock");
	this->_log = this->SenderLog;
	this->Socket = Soc;
	this->Receiver = false;
	this->Wait->unlock();
	this->_log->WriteMessage(TRACE,"Unlocking worker number" + std::to_string(this->_number));
	this->_log->WriteMessage(TRACE,"Exiting" + this->_Name + "::Unlock");
}

Worker::~Worker()
{
	this->SenderLog->WriteMessage(TRACE,"Entering " + this->_Name + "::Destructor");
	this->ReceiverLog->WriteMessage(TRACE,"Entering " + this->_Name + "::Destructor");
	delete this->_RS;
	delete this->_CS;
	delete this->Wait;
	delete this->connection;
	this->thr->join();
	delete this->thr;
	this->SenderLog->WriteMessage(TRACE,"Exiting" + this->_Name + "::Constructor");
	this->ReceiverLog->WriteMessage(TRACE,"Exiting" + this->_Name + "::Destructor");
}
void Worker::Start()
{
	this->Wait->lock();
	this->thr = new std::thread( [this] { this->Work(); } );
	//thr->detach();
}

void Worker::SetTermination()
{
	this->terminate = true;
	this->Wait->unlock();
}




