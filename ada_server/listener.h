/*
 * listener.h
 *
 *  Created on: Feb 22, 2015
 *      Author: tuso
 */

#ifndef LISTENER_H_
#define LISTENER_H_

#include <netdb.h>
#include <atomic>
#include <errno.h>
#include <cstdlib>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "loger.h"
#include "workerPool.h"

class Listener
{
	private:
    	const std::string _Name="Listener";
		int _port;		/**< port na ktorom bude server pocuvat*/
		int s;			/**< socket servra*/
		Loger *_log;
		sem_t *_semapohore;
		WorkerPool *_workers;
		std::atomic<bool> terminated;
	public:
		Listener(Loger *l,int Port,sem_t *semaphore, WorkerPool *wp);
		int Listen();
		int ReciveConnection();
		~Listener();
		void UnAccept();
};



#endif /* LISTENER_H_ */
