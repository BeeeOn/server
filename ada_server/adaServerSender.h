/**
 * @file adaServerSender.h
 * 
 * @brief definition of AdaServerSender Class
 *
 * @author Matus Blaho 
 * @version 1.0
 */

#include <string>
#include <iostream>
#include <netdb.h>
#include <errno.h>
#include <cstdlib>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sstream>
#include <locale.h>
#include <fstream>
#include <signal.h>
#include <sys/wait.h>
#include <vector>
#include <pwd.h>
#include <thread>
#include <arpa/inet.h>
#include <exception>
#include <soci.h>
#include <postgresql/soci-postgresql.h>
#include "config.h"
#include "loger.h"
#include "structures.h"
#include "DBHandler.h"
#include <atomic>
#include "workerPool.h"
#include "listener.h"


/** @Class AdaServerSender
 *  @brief Class representing ada_server_sender part of aplication created on start of app
 */

class AdaServerSender
{
	private:
		std::string _Name = "AdaServerReciver"; /**< Member representing name of class*/
		Loger *_log; /**< Reference to logger used for logging*/
		WorkerPool *workers; /**< reference to workerpool to obtain workers*/
		Config *conf; /**< Reference to Config used to load configuration*/
		Listener *_L; /**< Reference to Listener object*/
	public:
		/**Constructor of object
		 *  @param Sem - semaphore
		 *  @param workers - WorkerPool for obtaining workers
		 *  @param L - Loger object used for logging
		 *  @param c - Config object used to load configuration*/
		AdaServerSender(sem_t *Sem, WorkerPool *workers, Loger *L, Config *c);
		/** Destructor */
		~AdaServerSender();
		/** Method to start ada_server_sender part of app */
		void Start();
		/** method to log interuption signal */
		void LogINT(){this->_log->WriteMessage(ERR,"SIGINT received stopping server!");};
};








