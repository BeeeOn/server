/*
 * adaServer.h
 *
 *  Created on: Feb 22, 2015
 *      Author: tuso
 */

#ifndef ADASERVER_H_
#define ADASERVER_H_

#include<string>  //c++ znakove retazce
#include<cstring>
#include <ctime>  // kniznica nutna pre casove razitka
//kniznice na pracu s DB
#include <ctime>
#include <chrono>  //kniznica pre meranie casu
#include <iostream> //kniznica pre vystup na teminal
#include <signal.h>  //kniznica pre signaly
#include <sys/wait.h> //kniznica pre funkciu wait
#include <atomic>
#include <exception> //kniznica pre bok try/catch
#include <semaphore.h>
#include "loger.h"
#include "config.h"
#include "connectionHandler.h"
#include "workerPool.h"
#include "adaServerSender.h"
#include "adaServerReceiver.h"



#endif /* ADASERVER_H_ */
