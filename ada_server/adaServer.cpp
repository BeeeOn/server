/*
 * adaServer.cpp
 *
 *  Created on: Feb 22, 2015
 *      Author: tuso
 */




#include "loger.h"
#include "config.h"
#include "connectionHandler.h"
#include "workerPool.h"
#include "adaServerSender.h"
#include "adaServerReceiver.h"
#include "SSLContainer.h"
#include <exception> //kniznica pre bok try/catch
#include <semaphore.h>
#include <iostream> //kniznica pre vystup na teminal

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

void sig_handler(int signo)
{
	if (signo == SIGINT)
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
	}
	exit(EXIT_SUCCESS);
}

int main()  //hlavne telo programu
{
	SenderLog = new Loger();
	ReceiverLog = new Loger();
	c = new Config();
	if (!c->setConfig("ada_server.config.xml"))
	{
		delete SenderLog;
		delete ReceiverLog;
		delete c;
		return (1);
	}
	else
	{
		SenderLog->SetLogger(c->SenderVerbosity(),c->SenderMaxFiles(),c->SenderMaxLines(),c->SenderFileNaming(),"SENDER");
		ReceiverLog->SetLogger(c->ReceiverVerbosity(),c->ReceiverMaxFiles(),c->ReceiverMaxLines(),c->ReceiverFileNaming(),"RECEIVER");
	}
	if (signal(SIGINT, sig_handler) == SIG_ERR)
	{
		SenderLog->WriteMessage(ERR," [Main Process] Unable to catch SIGINT");
	}
	sslCont = new SSLContainer();
	wpool = WorkerPool::CreatePool(ReceiverLog,SenderLog,c->DBName(),c->ConnLimit(),sslCont);
	if (wpool->Limit()<=0)
	{
		SenderLog->WriteMessage(FATAL," [Main Process] 0 connections to DB unable to server terminating!");
		delete (c);
		delete wpool;
		delete SenderLog;
		delete ReceiverLog;
		return (1);
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
	}
	return (0);
}

