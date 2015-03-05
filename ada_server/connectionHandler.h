/*
 * connectionHandler.h
 *
 *  Created on: Feb 18, 2015
 *      Author: tuso
 */

#ifndef CONNECTIONHANDLER_H_
#define CONNECTIONHANDLER_H_

#include "loger.h"
#include "workerPool.h"
#include <arpa/inet.h> //kniznica pre internetove operacie
#include <netdb.h> //kniznica pre sietove operacie
#include <cstdlib>  //kniznica pre standarnde funkcie a dynamicku pamat
#include <netinet/in.h>  //kniznica pre sietovy protokol
#include <sys/socket.h>  //kniznica pre sockety
#include <sys/types.h>  //kniznica pre systemove typy
#include <string.h>  //kniznica pre znakove retazce
#include <unistd.h>  //kniznica pre standardne symbolicke konstanty a typy
#include <signal.h>  //kniznica pre signaly
#include <sys/wait.h> //kniznica pre funkciu wait
#include <string>  //c++ znakove retazce
#include <semaphore.h>
#include <exception> //kniznica pre bok try/catch

class ConnectionHandler
{
	private:
		const std::string _Name="ConnectionHandler";
		int _port;		/**< port na ktorom bude server pocuvat*/
		int s;			/**< socket servra*/
		Loger *_log;
		sem_t *_semaphore;
		WorkerPool *_workers;
	public:
		ConnectionHandler(Loger *L, int port,sem_t *sem, WorkerPool *wp);
		int Listen();
		int ReciveConnection();
		~ConnectionHandler();
		void UnbindSocket();
};


#endif /* CONNECTIONHANDLER_H_ */
