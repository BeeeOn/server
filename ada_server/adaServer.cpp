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
#include "workerPool.h"
#include "adaServerSender.h"
#include "adaServerReceiver.h"
#include "SSLContainer.h"
#include <exception> 
#include <semaphore.h>
#include <iostream> 
#include <termios.h>
#include <unistd.h>
#include <syslog.h>

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
		CRYPTO_cleanup_all_ex_data();
		ERR_remove_state(0);
		ERR_free_strings();
		EVP_cleanup();
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


int main(int argc, char **argv)  //main body of application
{
	if (argc < 2) {
		std::cerr<<"Path to configuration file was not given."<<std::endl;
		exit(EXIT_FAILURE);
	}

	pid_t pid, sid;
	std::cout<<"Reading configuration"<<std::endl;
	c = new Config();
	if (!c->setConfig(argv[1]))
	{
		std::cerr<<"Errors during configuration exiting"<<std::endl;
		delete c;
		exit(EXIT_FAILURE);
	}
	std::string connStr = buildConnString(c->DBName(),c->User(),c->Password());
	/* Fork off the parent process */
	if (c->Mode()!=0)  //start as deamon
	{
		std::cout<<"Starting deamonization"<<std::endl;
		pid = fork();
		if (pid < 0)
		{
				std::cerr<<"Deamonzation failed! Running in teminal mode."<<std::endl;
		}
		else
		{
		/* If we got a good PID, then
		   we can exit the parent process. */
			if (pid > 0)
			{
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
			int proces_id=getpid();
			//std::cout<<"Creating stop script in current directory"<<std::endl;
			std::ofstream stopSCR;
			stopSCR.open("stop_ada_server.sh", std::ios::out|std::ios::trunc);
			stopSCR<<"kill -SIGINT "<<proces_id<<std::endl;
			stopSCR.close();
			/* Change the current working directory */
			if ((chdir("/")) < 0)
			{
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
	SenderLog->SetLogger(c->SenderVerbosity(),c->SenderMaxFiles(),c->SenderMaxLines(),c->SenderFileNaming(),c->SenderPath(),"SENDER");
	ReceiverLog->SetLogger(c->ReceiverVerbosity(),c->ReceiverMaxFiles(),c->ReceiverMaxLines(),c->ReceiverFileNaming(),c->SenderPath(),"RECEIVER");
	if (signal(SIGINT, sig_handler) == SIG_ERR)
	{
		SenderLog->WriteMessage(ERR," [Main Process] Unable to catch SIGINT");
	}
	if (signal(SIGTERM, sig_handler) == SIG_ERR)
	{
		SenderLog->WriteMessage(ERR," [Main Process] Unable to catch SIGTERM");
	}
	if (signal(SIGHUP, SIG_IGN) == SIG_ERR)
	{
		SenderLog->WriteMessage(ERR," [Main Process] Unable to mask SIGHUP");
	}
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
	{
		SenderLog->WriteMessage(ERR," [Main Process] Unable to mask SIGPIPE");
	}
	sslCont = new SSLContainer(ReceiverLog);
	wpool = WorkerPool::CreatePool(ReceiverLog,SenderLog,connStr,c,sslCont);
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
		//SenderThread->join();
		delete (SenderThread);
		delete (SenderLog);
		delete (ReceiverLog);
	}
	return (0);
}

