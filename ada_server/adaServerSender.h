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
#include "pugixml.hpp"
#include "pugiconfig.hpp"
#include <soci.h>
#include <postgresql/soci-postgresql.h>
#include "config.h"
#include "loger.h"
#include "structures.h"
#include "DBHandler.h"
#include <atomic>
#include "workerPool.h"
#include "listener.h"

class AdaServerSender
{
	private:
		std::string _Name = "AdaServerReciver";
		Loger *_log;
		WorkerPool *workers;
		Config *conf;
		Listener *_L;
	public:
		AdaServerSender(sem_t *Sem, WorkerPool *workers, Loger *L, Config *c);
		~AdaServerSender();
		void Start();
		void LogINT(){this->_log->WriteMessage(ERR,"SIGINT received stopping server!");};
};








