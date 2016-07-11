/**
 * @file loger.cpp
 * 
 * @brief implementation main
 *
 * @author Matus Blaho 
 * @version 1.0
 */

#include "loger.h"
#include "config.h"
#include "connectionHandler.h"
#include "adaServerSender.h"
#include "adaServerReceiver.h"
#include <termios.h>
#include <Poco/Exception.h>

using namespace Poco;

Config *c;

WorkerPool *wpool = NULL;
sem_t connectionSem;
std::atomic<unsigned long> connCount;
AdaServerReceiver *receiver;
AdaServerSender *sender;
std::thread *SenderThread;
Loger *SenderLog;
Loger *ReceiverLog;
bool sigint =false;
SSLContainer *sslCont;


static pthread_mutex_t *lockarray;

static void lock_callback(int mode, int type, char *file, int line)
{
	(void)file;
	(void)line;
	if (mode & CRYPTO_LOCK) {
		pthread_mutex_lock(&(lockarray[type]));
	}
	else {
		pthread_mutex_unlock(&(lockarray[type]));
	}
}

static unsigned long thread_id(void)
{
	unsigned long ret;

	ret=(unsigned long)pthread_self();
	return(ret);
}

static void init_locks(void)
{
	int i;

	lockarray=(pthread_mutex_t *)OPENSSL_malloc(CRYPTO_num_locks() *
												sizeof(pthread_mutex_t));
	for (i=0; i<CRYPTO_num_locks(); i++) {
		pthread_mutex_init(&(lockarray[i]),NULL);
	}

	CRYPTO_set_id_callback((unsigned long (*)())thread_id);
	CRYPTO_set_locking_callback((void (*)(int, int, const char*, int))lock_callback);
}

static void kill_locks(void)
{
	int i;

	CRYPTO_set_locking_callback(NULL);
	for (i=0; i<CRYPTO_num_locks(); i++)
		pthread_mutex_destroy(&(lockarray[i]));

	OPENSSL_free(lockarray);
}

void sig_handler(int signo) //on signals to turn of clear 
{
	if ((signo == SIGINT)||(signo == SIGTERM))
	{
		sigint = true;
		receiver->LogINT();
		sender->LogINT();
		delete (sslCont);
		delete (c);
		delete (wpool);
		delete (sender);
		delete (receiver);
		sem_destroy(&connectionSem);
		delete (SenderThread);
		delete (SenderLog);
		delete (ReceiverLog);
		kill_locks();
		ERR_remove_state(0);
		ERR_free_strings();
		EVP_cleanup();
		CRYPTO_cleanup_all_ex_data();
	}
	exit(EXIT_SUCCESS);
}

std::string buildConnString(std::string DBName, std::string User, std::string Password) //building connection string to database
{
	std::string result;
	result.clear();
	if(DBName.empty())
	{
		return ("");
	}
	if (User.empty())
	{
		return ("dbname=" + DBName);
	}
	if (Password.empty())
	{
		std::cout<<"Enter password for user :"<<User<<std::endl;
		struct termios tty;
		tcgetattr(STDIN_FILENO, &tty);
		tty.c_lflag &= ~ECHO;
		(void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
		std::cin>>Password;
		tcgetattr(STDIN_FILENO, &tty);
		tty.c_lflag |= ECHO;
		(void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
		std::cout<<"Password readed"<<std::endl;
		result = "dbname="+ DBName + " user=" + User + " password=" + Password;
		return (result);
	}
	result = "dbname="+ DBName + " user=" + User + " password=" + Password;
	return(result);
}

static void startDaemon(void)
{
	pid_t pid, sid;

	std::cout<<"Starting deamonization"<<std::endl;
	pid = fork();
	if (pid < 0) {
		std::cerr<<"Deamonzation failed! Running in teminal mode."<<std::endl;
	} else {
		/* If we got a good PID, then
		   we can exit the parent process. */
		if (pid > 0) {
			delete(c);
			exit(EXIT_SUCCESS);
		}

		/* Change the file mode mask */
		umask(0);

		/* Open any logs here */

		/* Create a new SID for the child process */
		sid = setsid();
		if (sid < 0) {
			/* Log the failure */
			exit(EXIT_FAILURE);
		}

		int proces_id = getpid();
		//std::cout<<"Creating stop script in current directory"<<std::endl;
		std::ofstream stopSCR;
		stopSCR.open("stop_ada_server.sh", std::ios::out|std::ios::trunc);
		stopSCR<<"kill -SIGINT "<<proces_id<<std::endl;
		stopSCR.close();

		/* Change the current working directory */
		if ((chdir("/")) < 0) {
			/* Log the failure */
			exit(EXIT_FAILURE);
		}
		//std::cout<<"Finishing deamonization"<<std::endl;
		/* Close out the standard file descriptors */
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
	}
}

static void trapSignals(void)
{
	if (signal(SIGINT, sig_handler) == SIG_ERR)
		SenderLog->WriteMessage(ERR," [Main Process] Unable to catch SIGINT");

	if (signal(SIGTERM, sig_handler) == SIG_ERR)
		SenderLog->WriteMessage(ERR," [Main Process] Unable to catch SIGTERM");

	if (signal(SIGHUP, SIG_IGN) == SIG_ERR)
		SenderLog->WriteMessage(ERR," [Main Process] Unable to mask SIGHUP");

	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		SenderLog->WriteMessage(ERR," [Main Process] Unable to mask SIGPIPE");
}

int main(int argc, char **argv)  //main body of application
{
	if (argc < 2) {
		std::cerr<<"Path to configuration file was not given."<<std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout<<"Reading configuration"<<std::endl;
	c = new Config();

	try {
		c->setConfig(argv[1]);
	} catch(Exception &e) {
		std::cerr << e.displayText() << std::endl;
		std::cerr << "Failed to parse configuration" << std::endl;
		delete c;
		return EXIT_FAILURE;
	}
	std::string connStr = buildConnString(c->DBName(),c->User(),c->Password());
	/* Fork off the parent process */
	if (c->Mode()!=0)  //start as deamon
	{
		startDaemon();
	}
	else
	{
		std::cout<<"Server started in terminal(debug) mode"<<std::endl;
		int proces_id=getpid();
		//std::cout<<"Creating stop script in current directory"<<std::endl;
		std::ofstream stopSCR;
		stopSCR.open("stop_ada_server.sh", std::ios::out|std::ios::trunc);
		stopSCR<<"kill -SIGINT "<<proces_id<<std::endl;
		stopSCR.close();
	}

	SenderLog = new Loger();
	ReceiverLog = new Loger();
	SenderLog->SetLogger(c->SenderVerbosity(),c->SenderMaxFiles(),c->SenderMaxLines(),c->SenderFileNaming(),c->SenderPath(),"SENDER",c->SenderToSTD());
	ReceiverLog->SetLogger(c->ReceiverVerbosity(),c->ReceiverMaxFiles(),c->ReceiverMaxLines(),c->ReceiverFileNaming(),c->SenderPath(),"RECEIVER",c->ReceiverToSTD());

	trapSignals();

	SSL_load_error_strings();
	SSL_library_init();
	OpenSSL_add_all_algorithms();

	sslCont = new SSLContainer(ReceiverLog);
	wpool = WorkerPool::CreatePool(ReceiverLog,SenderLog,connStr,c,sslCont);
	init_locks();
	if ((wpool==NULL)||(wpool->Limit()<=0))
	{
		SenderLog->WriteMessage(FATAL," [Main Process] 0 connections to DB unable to serve, terminating!");
		delete (c);
		delete wpool;
		delete SenderLog;
		delete ReceiverLog;
		exit(EXIT_FAILURE);
	}
	else
	{
		int semVal = wpool->Limit();
		SenderLog->WriteMessage(INFO,"[Main Process] Maximal connection count : " + std::to_string(semVal));
		ReceiverLog->WriteMessage(INFO,"[Main Process] Maximal connection count : " + std::to_string(semVal));
		sem_init(&connectionSem,0,semVal);
		wpool->SetSemaphore(&connectionSem);
		SenderLog->WriteMessage(TRACE,"[Main Process] Creating Sender");
		ReceiverLog->WriteMessage(TRACE,"[Main Process] Creating Sender");
		sender = new AdaServerSender(&connectionSem,wpool,SenderLog,c);
		SenderLog->WriteMessage(TRACE,"[Main Process] Creating Receiver");
		ReceiverLog->WriteMessage(TRACE,"[Main Process] Creating Receiver");
		receiver = new AdaServerReceiver(&connectionSem,wpool,ReceiverLog,c);
		SenderLog->WriteMessage(INFO,"[Main Process] Starting Sender");
		ReceiverLog->WriteMessage(INFO,"[Main Process] Starting Sender");
		SenderThread = new std::thread( [ ] { sender->Start(); });
		SenderThread->detach();
		ReceiverLog->WriteMessage(INFO,"[Main Process] Starting Receiver");
		SenderLog->WriteMessage(INFO,"[Main Process] Starting Receiver");
		receiver->Start();
	}
	if (!sigint)
	{
		delete (c);
		delete (wpool);
		delete (sender);
		delete (receiver);
		CRYPTO_cleanup_all_ex_data();
		kill_locks();
		ERR_remove_state(0);
		ERR_free_strings();
		EVP_cleanup();
		sem_destroy(&connectionSem);
		delete (SenderThread);
		delete (SenderLog);
		delete (ReceiverLog);
		exit(EXIT_FAILURE);
	}
	return (0);
}

