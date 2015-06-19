/**
 * @file WorkerPool.cpp
 * 
 * @brief implementation of WorkerPool class
 *
 * @author Matus Blaho 
 * @version 1.0
 */

#include "workerPool.h"

/**********************************************Start of DatabaseConnectionContainer section******************************************************/

using namespace soci;

WorkerPool::WorkerPool(std::string DBConnString, Config *c, Loger *Rl, Loger *Sl,SSLContainer *sslcont)
{
	this->ReceiverLoger = Rl;
	this->SenderLoger = Sl;
	this->_log = Rl;
	this->freeCount =0;
	Rl->WriteMessage(TRACE,"Entering " + this->_Name + "::Constructor");
	Sl->WriteMessage(TRACE,"Entering " + this->_Name + "::Constructor");
	_DBConnString = DBConnString;
	Rl->WriteMessage(INFO,"Creating connections to DB");
	Sl->WriteMessage(INFO,"Creating connections to DB");
	soci::session *connections[100];
	for (int i = 0; i < c->ConnLimit(); i++)
	{
		try  //try to connect to database
		{
			session *SQL = new session(postgresql, this->_DBConnString);
			connections[i] = SQL;
			this->freeCount = i+1;
		}
		catch(std::exception const &e) //if there is error 
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
		try  //try to create workers
		{
			Worker *w= new Worker(connections[i],this->ReceiverLoger,this->SenderLoger, this, i,sslcont,c);
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
		catch(CertificateLoadException &e)
		{
			SenderLoger->WriteMessage(FATAL, "Error during certificates loading occurred! Terminating!");
			ReceiverLoger->WriteMessage(FATAL, "Error during certificates loading occurred! Terminating!");
			for (int y=i; y<freeCount; y++)
			{
				delete connections[y];
			}
			for (int y=0;y<i;y++)
			{
				workers[y]->SetTermination();
			}
			this->semaphore.lock(); //mututal exclusion zone
			for (int y = 0; y< i; y++)
			{
				delete(workers[y]); //delete workers
				this->SenderLoger->WriteMessage(TRACE,"Deleted connection : " + std::to_string(i) + " from : " + std::to_string(this->freeCount));
				this->ReceiverLoger->WriteMessage(TRACE,"Deleted connection : " + std::to_string(i) + " from : " + std::to_string(this->freeCount));
			}
			this->semaphore.unlock();
			throw(WorkerPoolCreateException());
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
		workers[i]->SetTermination(); //terminate all workers
	}
	sleep(6);
	this->semaphore.lock(); //mututal exclusion zone
	for (int i = 0; i< this->freeCount; i++)
	{
		delete(workers[i]); //delete workers
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
	this->workers[this->freeCount] = worker; //give worker back to array
	this->freeCount++;
	this->semaphore.unlock();
	sem_post((this->Sem));
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

WorkerPool *WorkerPool::CreatePool(Loger *Rl, Loger *Sl, std::string DBConnString, Config *c,SSLContainer *sslcont)
{
	Rl->WriteMessage(TRACE,"Entering WorkerPool::CreatePool");
	Sl->WriteMessage(TRACE,"Entering WorkerPool::CreatePool");
	if (!instance)  //if there is no instance create it
	{
		try
		{
			instance=new WorkerPool(DBConnString, c, Rl, Sl,sslcont);
		}
		catch(WorkerPoolCreateException &e)
		{
			instance = NULL;
			Sl->WriteMessage(TRACE,"WorkerPool creation failed!");
			Rl->WriteMessage(TRACE,"WorkerPool creation failed!");
		}
	}
	Sl->WriteMessage(TRACE,"Exiting WorkerPool::CreatePool");
	Rl->WriteMessage(TRACE,"Exiting WorkerPool::CreatePool");
	return (instance);

}

	/**********************************************End of DatabaseConnectionContainer section******************************************************/



