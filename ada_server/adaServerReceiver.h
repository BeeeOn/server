/**
 * @file adaServerReceiver.h
 * 
 * @brief definition of AdaServerReceiver Class
 *
 * @author Matus Blaho 
 * @version 1.0
 */

#include<string>  //c++ strings
#include<cstring> // C strings
#include <ctime>  // timestamps
#include <mutex>  // mutex library
#include <fstream> //file streams
#include <iomanip> // I/O manipulation
#include <sys/wait.h> //kniznica pre funkciu wait
#include <atomic>  // atomic library
#include <exception> //exceptions library
#include <semaphore.h>  //definitions of semaphore
#include "../lib/pugixml.hpp"
#include "../lib/pugiconfig.hpp"
#include "loger.h"
#include "config.h"
#include "connectionHandler.h"
#include "workerPool.h"

/** @Class AdaServerReceiver
 *  @brief Class representing ada_server_receiver part of aplication created on start of app
 */

class AdaServerReceiver
{
	private:
		std::string _Name = "AdaServerReceiver"; /**< Member representing name of class*/
		Loger *_log; /**< Member representing object used for logging*/
		WorkerPool *workers; /**< Member representing workerpool reference*/
		Config *conf;  /**< Member representing configuration reference*/
		ConnectionHandler *_CH; /**< Member representing connection hander reference*/

	public:
		/** Constructor of class
		 *  @param Sem - semaphore
		 *  @param workers - WorkerPool for obtaining workers
		 *  @param L - Loger object used for logging
		 *  @param c - Config object used to load configuration*/
		AdaServerReceiver(sem_t *Sem, WorkerPool *workers, Loger *L,Config *c);
		/** Method to start ada_server_receiver part of app */
		void Start();
		/** Destructor */
		~AdaServerReceiver();
		/** method to log interuption signal */
		void LogINT(){this->_log->WriteMessage(ERR,"SIGINT received stopping server!");};
};
