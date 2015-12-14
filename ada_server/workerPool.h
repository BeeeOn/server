/**
 * @file WorkerPool.h
 * 
 * @brief definition of WorkerPool and Worker classes
 *
 * @author Matus Blaho 
 * @version 1.0
 */
#ifndef WORKERPOOL_H_

#include "loger.h"
#include <mutex>
#include <soci.h>
#include <postgresql/soci-postgresql.h>
#include <mutex>
#include <soci.h>
#include <postgresql/soci-postgresql.h>
#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <semaphore.h>
#include "connectionServer.h"
#include "DBHandler.h"
#include "requestServer.h"
#include "SSLContainer.h"
#include "config.h"
#include "structures.h"

class Worker;

/** @class WorkerPool 
 *  @brief Class used to create, terminate and store all instances of Worker
 */

class WorkerPool
{
	private:
		Loger *_log,*SenderLoger,*ReceiverLoger; /**< references to loggers*/
		const std::string _Name="WorkerPool"; /**< name of class*/
		int freeCount; /**< index of last free worker*/
		Worker *workers[100]; /**< array of workers limited to 100 as max DB connections is 100*/
		std::mutex semaphore; /**< mutual exclusion for operations on array of free workers*/
		std::string _DBConnString; /**< connection string to database*/
		sem_t *Sem; /**< counter of free workers*/
		/** Private Constructor
		 * @param DBConnString std::string text representation of connection string to database
		 * @param c Config pointer to configuration
		 * @param Rl Loger pointer to receiver logger
		 * @param Sl Loger pointer to sender logger
		 * @param sslcont SSLContainer pointer to SSLContainer
				 */
		WorkerPool(std::string DBConnString, Config *c, Loger *Rl, Loger *Sl,SSLContainer *sslcont);
		static WorkerPool *instance; /**< static property caring pointer to instance*/
	public:
		/**Method to obtain count of really created connections to database
		 * @return int representing this count
				 */
		int Limit();
		/** Static method to create only one instance of this class
		 * @param Rl Loger pointer to receiver logger
		 * @param Sl Loger pointer to sender logger
		 * @param DBConnString std::string text representation of connection string to database
		 * @param c Config pointer to configuration
		 * @param sslcont SSLContainer pointer to SSLContainer
		 * @return pointer to created instance
				 */
		static WorkerPool *CreatePool(Loger *Rl, Loger *Sl, std::string DBConnString, Config *c, SSLContainer *sslcont);
		/** Method to give Worker which will serve
		 * @param l Loger pointer to loger of calling object
		 * @return pointer to Worker*/
		Worker *GetWorker(Loger *l);
		/** Method to return Worker which finish serving
		 * @param worker Worker pointer to worker which wants to return back to container
		 * @param l Loger pointer to loger of calling object
		 * */
		void ReturnWorker(Worker *worker,Loger *l);
		/** Method to set worker counter in class
		 * @param sem sem_t pointer to semaphore*/
		void SetSemaphore (sem_t *sem){this->Sem=sem;};
		/** Destructor
		 */
		~WorkerPool();
};

/** @class Worker
 * @brief Class representing one unit used for serving clients requests
 */

class Worker
{
	private :
		const std::string _Name="Worker"; /**< Name of class*/
		soci::session *connection; /**< Reference to DB connection*/
		std::mutex *Wait; /**< Mutex whitch block body of worker until there is work for him*/
		Loger *_log, *ReceiverLog, *SenderLog; /**< references to loggers*/
		int Socket; /**< Socket for servers*/
		/** Method to test corrcetion of design
		 */
		void SimpleMothod();
		/** Method called in separate thread representing worker body
		 */
		void Work();
		in_addr IPaddress; /**< IP address for ConectionServer*/
		bool terminate; /**< termination flag*/
		std::thread *thr; /**< thread which is running body*/
		WorkerPool *parentPool; /**< reference for parent pool*/
		ConnectionServer *_CS; /**< reference to adapter request server*/
		RequestServer *_RS;   /**< reference to ui server request server*/
		soci::session *DB;   /**< duplicate needs to be removed*/
		int _number; /**< worker identifier*/
		bool Receiver; /**< type of client request*/
	public :
		/** Constructor of Worker object
		 * @param S soci::session pointer to exclusive connection to DB
		 * @param Rl Loger pointer to receiver logger
		 * @param Sl Loger pointer to sender logger
		 * @param pool WorkerPool pointer to parent pool
		 * @param i int identification number of worker
		 * @param sslcont SSLContainer pointer to SSLContainer
		 * @param c Config pointer to configuration
						 */
		Worker (soci::session *s,Loger *Rl,  Loger *Sl, WorkerPool *pool, int i, SSLContainer *sslcont, Config *c);
		/** Method to unlock worker from ConncetionHandler
		 * @param Soc int socket to read message from client
		 * @param IP in_addr ip adress of client - in current implementation unused
				 */
		void Unlock(int Soc, in_addr IP);
		/** Method to unlock worker from Listener
		 * @param Soc int socket to read message from client
				 */
		void Unlock(int Soc);
		/** Destructor
				 */
		~Worker();
		/** Method to Start worker body
				 */
		void Start();
		/** Method to set termination of worker
				 */
		void SetTermination();
};

#define WORKERPOOL_H_





#endif /* WORKERPOOL_H_ */
