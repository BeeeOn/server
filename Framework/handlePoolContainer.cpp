/**
* @file handlePoolContainer.cpp
*
* Implementace kontrejneru poolu pro Prichozi spojeni
*
*/

#include "handlePoolContainer.h"

using namespace std;

#define INIT_NON_EXIST_PORT -1
#define INIT_NON_EXIST_SOCKET -1
HandlePoolContainer *HandlePoolContainer::instance = NULL;

HandlePoolContainer::HandlePoolContainer(unsigned int poolSize, Loger *l)
{
	this->loger = l;
	this->freePoolsCount = 0;

	loger->WriteMessage(TRACE,"Entering " + this->_Name + "::HandlePoolContainer");
    //FrameworkServerHandle **poolContainer = malloc(sizeof(FrameworkServerHandle)*poolSize);
	for (int i = 0; i < poolSize; i++)
	{
	    try
		{
            FrameworkServerHandle * tmpNewConnectionHandler = new FrameworkServerHandle(INIT_NON_EXIST_SOCKET, INIT_NON_EXIST_PORT);
            handlersPool.push_back(tmpNewConnectionHandler);
            this->freePoolsCount++;
		}
		catch(std::exception const &e)
		{
		    std::string ErrorMessage = this->_Name + "::HandlePoolContainer Not able to create HandlePoolContainer number : " + std::to_string(i) + "failed reason :";
			ErrorMessage.append(e.what());
			this->loger->WriteMessage(ERR,ErrorMessage);
		}
	}
	sem_init(&this->poolLimitSemaphore, 0, this->freePoolsCount);
	this->loger->WriteMessage(INFO,"HandlePoolContainer created");
	this->loger->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandlePoolContainer");
}

/**
* Destruktor kontejneru pro praci s databazi
*
*/
HandlePoolContainer::~HandlePoolContainer()
{
	this->loger->WriteMessage(TRACE,"Entering " + this->_Name + "::Destructor");

    this->semaphore.lock();
	for (int i = 0; i< this->freePoolsCount; i++)
	{
	    FrameworkServerHandle * tmpHandleToRemove = this->handlersPool.back();
	    this->handlersPool.pop_back();
		delete(tmpHandleToRemove);
		this->loger->WriteMessage(INFO,"Deleted FrameworkServerHandle from Pool of Connection Handlers");
	}
	instance = NULL;
    this->semaphore.unlock();

	this->loger->WriteMessage(TRACE,"Exiting " + this->_Name + "::Destructor");
}

FrameworkServerHandle *HandlePoolContainer::GetConnectionHandler()
{
	this->loger->WriteMessage(TRACE,"Entering HandlePoolContainer::GetConnectionHandler");
	FrameworkServerHandle *retConnectionHandler = NULL;
	sem_wait(&this->poolLimitSemaphore);
	this->semaphore.lock();
	if (this->freePoolsCount>0)
	{
	    retConnectionHandler = this->handlersPool.back();
	    this->handlersPool.pop_back();
		this->freePoolsCount--;
		this->loger->WriteMessage(INFO,"Free connection pools : " + std::to_string(this->freePoolsCount));
	}
	this->semaphore.unlock();
	this->loger->WriteMessage(TRACE,"Exiting HandlePoolContainer::GetConnectionHandler");
	return retConnectionHandler;
}

void HandlePoolContainer::ReturnConnectionHandler(FrameworkServerHandle *connectionHandler)
{
	this->loger->WriteMessage(TRACE,"Entering " + this->_Name + "::ReturnConnection");
	this->semaphore.lock();
	this->handlersPool.push_back(connectionHandler);
	this->freePoolsCount++;
	this->semaphore.unlock();
	sem_post(&this->poolLimitSemaphore);
	this->loger->WriteMessage(TRACE,"Exiting " + this->_Name + "::ReturnConnection");
}

unsigned int HandlePoolContainer::getFreePoolsCount()
{
	this->loger->WriteMessage(TRACE,"Entering " + this->_Name + "::getFreePoolsCount");
	this->semaphore.lock();
	unsigned int tmpFreePoolsCount = this->freePoolsCount;
    this->semaphore.unlock();
	this->loger->WriteMessage(TRACE,"Exiting " + this->_Name + "::getFreePoolsCount");
	return tmpFreePoolsCount;
}

HandlePoolContainer *HandlePoolContainer::GetHandlePoolContainer(unsigned int poolSize, Loger *l)
{
	l->WriteMessage(TRACE,"Entering  HandlePoolContainer::GetHandlePoolContainer");
	if (!instance){
        instance = new HandlePoolContainer(poolSize,l);
	}
	l->WriteMessage(TRACE,"Exiting  HandlePoolContainer::GetHandlePoolContainer");
	return instance;
}
