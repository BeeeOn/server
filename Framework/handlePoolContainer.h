/**
* @file handlePoolContainer.h
*
* Hlavickovy soubor pro kontejner objektu FrameworkServerHandle
*
*/

#ifndef HADLEPOOLCONTAINER_H_INCLUDED
#define HADLEPOOLCONTAINER_H_INCLUDED

#include "loger.h"
#include "framework.h"
#include <mutex>
#include <semaphore.h>
#include <vector>


class HandlePoolContainer
{
	private:
		static HandlePoolContainer *instance;
		Loger *loger;										/** Ukazatel na objekt pomoci nehoz se provadi logovani zaznamu do logovaciho souboru. */
		const std::string _Name = "HandlePoolContainer";	/** Nazev tridy pro lepsi pouziti v implementaci tridy. */
		unsigned int handlersPoolSize;                     /** Velikost PoolContaineru. */
		unsigned int freePoolsCount;                        /** Pocet aktualne volnych poolu. Iniciuje se dle handlersPoolCount. */
		std::vector<FrameworkServerHandle *> handlersPool;
		std::mutex semaphore;
		sem_t poolLimitSemaphore;
		HandlePoolContainer(unsigned int poolSize, Loger *l);
	public:
		static HandlePoolContainer *GetHandlePoolContainer(unsigned int poolSize, Loger *l);
		~HandlePoolContainer();
		FrameworkServerHandle *GetConnectionHandler();
		void ReturnConnectionHandler(FrameworkServerHandle *conn);
		unsigned int getFreePoolsCount();
};

#endif // HADLEPOOLCONTAINER_H_INCLUDED
