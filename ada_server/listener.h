/**
 * @file listener.h
 * 
 * @brief definition of Listener class
 *
 * @author Matus Blaho 
 * @version 1.0
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
#include <string.h>
#include <locale.h>
#include <cstring>


/** @class Listener
 *  @brief Class responsible for receiving connections on ada_server_sender side
 */

class Listener
{
	private:
    	const std::string _Name="Listener"; /**< property storing class name*/
		int _port;		/**< port to listen on*/
		int s;			/**< server socket*/
		Loger *_log;	/**< reference to loger*/
		sem_t *_semapohore;	/**< semaphore for free workers*/
		WorkerPool *_workers; /**< reference to WorkerPool to obtain workers*/
		std::atomic<bool> _terminated; /**< member to store termination info*/
		std::atomic<bool> _toBeTerminated;
	public:
		/**Constructor
		 * @param l to set logger of class
		 * @param Port int representing number of port to listen
		 * @param semaphore representing free workers counter
		 * @param wp representing WorkerPool to obtain workers
		 */
		Listener(Loger *l,int Port,sem_t *semaphore, WorkerPool *wp);
		/**Method to set up socket and bind it to port
		 * @return int representing success/failure
		 */
		int Listen();
		/**Method to receive connections from clients
		 * @return int representing success/failure
		 */
		int ReceiveConnection();
		/**Destructor
		 */
		~Listener();
		/**Method to destroy socket and stop receiving connection
		 */
		void UnAccept();
	private:
		int handleAcceptFailed(int &failcounter);
		void acceptClient(int sock);
};



#endif /* LISTENER_H_ */
