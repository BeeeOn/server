/**
 * @file conncetionHandler.h
 * 
 * @brief definition of ConncetionHandler Class
 *
 * @author Matus Blaho 
 * @version 1.0
 */

#ifndef CONNECTIONHANDLER_H_
#define CONNECTIONHANDLER_H_

#include "loger.h"  //definitions of Loger class
#include "workerPool.h" //definitions of WorkerPool class and Worker class
#include <arpa/inet.h> //library for network operations
#include <netdb.h> //library for network operations
#include <cstdlib>  //standard functions library and dynamic allocation
#include <netinet/in.h>  //network protocol library
#include <sys/socket.h>  //sockets library
#include <sys/types.h>  //system types library
#include <string.h>  //classic C strings library
#include <unistd.h>  //library for standard types and constants
#include <signal.h>  //signals library
#include <sys/wait.h> //wait library
#include <string>  //c++ strings
#include <semaphore.h> //semaphore library
#include <exception> //try/catch exception library
#include <cstring> //c standard library for strings

/** @Class ConncetionHandler
 *  @brief Class responsible for handling connections in ada_server_receiver part
 */

class ConnectionHandler
{
	private:
		const std::string _Name="ConnectionHandler"; /**< Member representing name of class*/
		int _port;		/**< Member representing port to listen on*/
		int s;			/**< socket of server*/
		Loger *_log;	/**< reference to logger used for logging*/
		sem_t *_semaphore; /**< semaphore used for connection counter*/
		WorkerPool *_workers; /**< reference to worker pool for obtaining workers*/
	public:
		/** Constructor of Connectionhandler class
		 *  @param L an pointer to  Loger object to use for logging
		 *  @param port an integer value of port to listen on
		 *  @param sem pointer to  semaphore to use as connection counter
		 *  @param wp pointer to Workerpool object for obtaining workers
		    */
		ConnectionHandler(Loger *L, int port,sem_t *sem, WorkerPool *wp);
		/** Method setting server socket and starts listening on it
		    */
		int Listen();
		/** Method to receive connections from adapters
		    */
		int ReciveConnection();
		/** Destructor
		    */
		~ConnectionHandler();
		/** Method to shut down blocked socket
		    */
		void UnbindSocket();
};


#endif /* CONNECTIONHANDLER_H_ */
