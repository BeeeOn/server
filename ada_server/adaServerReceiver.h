/**
 * @file server.h
 * 
 * @brief JImplementacia servera - hlavickovy subor
 *
 * @author xblaho03 
 * @version 1.0
 */

#include<string>  //c++ znakove retazce
#include<cstring>
#include <ctime>  // kniznica nutna pre casove razitka
//kniznice na pracu s DB
#include <mutex>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <chrono>  //kniznica pre meranie casu
#include <iostream> //kniznica pre vystup na teminal
#include <signal.h>  //kniznica pre signaly
#include <sys/wait.h> //kniznica pre funkciu wait
#include <atomic>
#include <exception> //kniznica pre bok try/catch
#include <semaphore.h>
#include "pugixml.hpp"
#include "pugiconfig.hpp"
#include "loger.h"
#include "config.h"
#include "connectionHandler.h"
#include "workerPool.h"



class AdaServerReceiver
{
	private:
		std::string _Name = "AdaServerReceiver";
		Loger *_log;
		WorkerPool *workers;
		Config *conf;
		ConnectionHandler *_CH;

	public:
		AdaServerReceiver(sem_t *Sem, WorkerPool *workers, Loger *L,Config *c);
		void Start();
		~AdaServerReceiver();
		void LogINT(){this->_log->WriteMessage(ERR,"SIGINT received stopping server!");};
};
