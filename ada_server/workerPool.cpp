/*
 * workerPool.cpp
 *
 *  Created on: Feb 16, 2015
 *      Author: tuso
 */

#include "workerPool.h"

/**********************************************Start of DatabaseConnectionContainer section******************************************************/

using namespace soci;

WorkerPool::WorkerPool(std::string DBName, int ConnLimit, Loger *Rl, Loger *Sl)
{
	this->ReceiverLoger = Rl;
	this->SenderLoger = Sl;
	this->_log = Rl;
	this->freeCount =0;
	Rl->WriteMessage(TRACE,"Entering " + this->_Name + "::Constructor");
	Sl->WriteMessage(TRACE,"Entering " + this->_Name + "::Constructor");
	_DBName = DBName;
	Rl->WriteMessage(INFO,"Creating connections to DB");
	Sl->WriteMessage(INFO,"Creating connections to DB");
	soci::session *connections[100];
	for (int i = 0; i < ConnLimit; i++)
	{
		try  //pokusime  sa o vytvorenie spojenia s databazou
		{
			session *SQL = new session(postgresql, "dbname="+this->_DBName);
			connections[i] = SQL;
			this->freeCount = i+1;
		}
		catch(std::exception const &e) //ak nastala chyba zachytime ju
		{
			std::string ErrorMessage = "Creating DB connection number : " + std::to_string(i) + "failed reason :";
			ErrorMessage.append (e.what());
			this->ReceiverLoger->WriteMessage(ERR,ErrorMessage );
			this->SenderLoger->WriteMessage(ERR,ErrorMessage );

		}
	}
	this->ReceiverLoger->WriteMessage(INFO,"Connections to DB created");
	this->ReceiverLoger->WriteMessage(INFO,"Creating workers");
	this->SenderLoger->WriteMessage(INFO,"Connections to DB created");
	this->SenderLoger->WriteMessage(INFO,"Creating workers");
	for (int i = 0; i < this->freeCount; i++)
	{
		try  //pokusime  sa o vytvorenie vlakna
		{
			Worker *w= new Worker(connections[i],this->ReceiverLoger,this->SenderLoger, this, i);
			this->workers[i] = w;
			this->workers[i]->Start();
		}
		catch (std::bad_alloc &e)
		{
			delete connections[this->freeCount];
			this->freeCount--;
			std::string ErrMessage = "Allocation of worker " + std::to_string(i) + " Failed : ";
			ErrMessage.append(e.what());
			SenderLoger->WriteMessage(ERR, ErrMessage);
			ReceiverLoger->WriteMessage(ERR, ErrMessage);
		}

	}
	this->ReceiverLoger->WriteMessage(INFO,"Workers were created");
	this->ReceiverLoger->WriteMessage(TRACE,"Exiting " + this->_Name + "::DatabaseConnectionContainer");
	this->SenderLoger->WriteMessage(INFO,"Workers were created");
	this->SenderLoger->WriteMessage(TRACE,"Exiting " + this->_Name + "::DatabaseConnectionContainer");
}

WorkerPool::~WorkerPool()
{
	this->SenderLoger->WriteMessage(TRACE,"Entering " + this->_Name + "::Destructor");
	this->SenderLoger->WriteMessage(INFO,"Closing connections to DB");
	this->ReceiverLoger->WriteMessage(TRACE,"Entering " + this->_Name + "::Destructor");
	this->ReceiverLoger->WriteMessage(INFO,"Closing connections to DB");
	for (int i = 0; i< this->freeCount; i++)
	{
		workers[i]->SetTermination();
	}
	sleep(6);
	this->semaphore.lock();
	for (int i = 0; i< this->freeCount; i++)
	{
		delete(workers[i]);
		this->SenderLoger->WriteMessage(TRACE,"Deleted connection : " + std::to_string(i) + " from : " + std::to_string(this->freeCount));
		this->ReceiverLoger->WriteMessage(TRACE,"Deleted connection : " + std::to_string(i) + " from : " + std::to_string(this->freeCount));
	}
	this->semaphore.unlock();
	this->ReceiverLoger->WriteMessage(INFO,"Connections to DB closed");
	this->ReceiverLoger->WriteMessage(TRACE,"Exiting " + this->_Name + "::Destructor");
	this->SenderLoger->WriteMessage(INFO,"Connections to DB closed");
	this->SenderLoger->WriteMessage(TRACE,"Exiting " + this->_Name + "::Destructor");
}

void WorkerPool::ReturnWorker(Worker *worker, Loger *l)
{
	l->WriteMessage(TRACE,"Entering " + this->_Name + "::ReturnConnection");
	this->semaphore.lock();
	this->workers[this->freeCount] = worker;
	this->freeCount++;
	this->semaphore.unlock();
	l->WriteMessage(TRACE,"Exiting " + this->_Name + "::ReturnConnection");
}

int WorkerPool::Limit()
{
	return (freeCount);
}

Worker *WorkerPool::GetWorker(Loger *l)
{
	l->WriteMessage(TRACE,"Entering "+ this->_Name + "::GetWorker");
	Worker *retWorker = NULL;
	this->semaphore.lock();
	if (this->freeCount>0)
	{
		retWorker = this->workers[this->freeCount-1];
		this->workers[this->freeCount-1] = NULL;
		this->freeCount--;
		l->WriteMessage(INFO,"Free connections : " + std::to_string(this->freeCount));
	}
	this->semaphore.unlock();
	l->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetWorker");
	return (retWorker);
}

WorkerPool *WorkerPool::instance = NULL;

WorkerPool *WorkerPool::CreatePool(Loger *Rl, Loger *Sl, std::string DBName, int ConnLimit)
{
	Rl->WriteMessage(TRACE,"Entering WorkerPool::CreatePool");
	Sl->WriteMessage(TRACE,"Entering WorkerPool::CreatePool");
	if (!instance)
		instance=new WorkerPool(DBName, ConnLimit, Rl, Sl);
	Sl->WriteMessage(TRACE,"Exiting WorkerPool::CreatePool");
	Rl->WriteMessage(TRACE,"Exiting WorkerPool::CreatePool");
	return (instance);

}

	/**********************************************End of DatabaseConnectionContainer section******************************************************/



