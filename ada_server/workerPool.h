/*
 * workerPool.h
 *
 *  Created on: Feb 16, 2015
 *      Author: tuso
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
#include "connectionServer.h"
#include "DBHandler.h"
#include "requestServer.h"

class Worker;

class WorkerPool
{
	private:
		Loger *_log,*SenderLoger,*ReceiverLoger;
		const std::string _Name="WorkerPool";
		int freeCount;
		Worker *workers[100];
		std::mutex semaphore;
		std::string _DBName;
		WorkerPool(std::string DBName, int ConnLimit, Loger *Rl, Loger *Sl);
		static WorkerPool *instance;
	public:
		int Limit();
		static WorkerPool *CreatePool(Loger *Rl, Loger *Sl, std::string DBName, int ConnLimit);
		Worker *GetWorker(Loger *l);
		void ReturnWorker(Worker *worker,Loger *l);
		~WorkerPool();
};


class Worker
{
	private :
		const std::string _Name="Worker";
		soci::session *connection;
		std::mutex *Wait;
		Loger *_log, *ReceiverLog, *SenderLog;
		int Socket;
		void SimpleMothod();
		void Work();
		in_addr IPaddress;
		bool terminate;
		std::thread *thr;
		WorkerPool *parentPool;
		ConnectionServer *_CS;
		RequestServer *_RS;
		soci::session *DB;
		int _number;
		bool Receiver;
	public :
		Worker (soci::session *s,Loger *Rl,  Loger *Sl, WorkerPool *pool, int i);
		void Unlock(int Soc, in_addr IP);
		void Unlock(int Soc);
		~Worker();
		void Start();
		void SetTermination();
};

#define WORKERPOOL_H_





#endif /* WORKERPOOL_H_ */
