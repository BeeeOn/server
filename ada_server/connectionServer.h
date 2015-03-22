/*
( * connectionserver.h
 *
 *  Created on: Feb 18, 2015
 *      Author: tuso
 */

#ifndef CONNECTIONSERVER_H_
#define CONNECTIONSERVER_H_

#include<string>  //c++ znakove retazce
#include<cstring>
#include <arpa/inet.h> //kniznica pre internetove operacie
#include <sys/poll.h>
#include <chrono>  //kniznica pre meranie casu
#include <iostream> //kniznica pre vystup na teminal
#include <netdb.h> //kniznica pre sietove operacie
#include <cstdlib>  //kniznica pre standarnde funkcie a dynamicku pamat
#include <netinet/in.h>  //kniznica pre sietovy protokol
#include <sys/socket.h>  //kniznica pre sockety
#include <sys/types.h>  //kniznica pre systemove typy
#include <string.h>  //kniznica pre znakove retazce
#include <unistd.h>  //kniznica pre standardne symbolicke konstanty a typy
#include <signal.h>  //kniznica pre signaly
#include <sys/wait.h> //kniznica pre funkciu wait
#include "pugixml.hpp"
#include "pugiconfig.hpp"
#include "loger.h"
#include "messageParsers.h"
#include "DBHandler.h"
#include <errno.h>
#include <string.h>


class ConnectionServer
{
	private:
		const std::string _Name="ConnectionServer";
		int com_s;
		DBHandler *database;    /**< ukazatel na objekt DBHandler*/
		MessageParser *MP;	/**< ukazatel na objekt MessageParser*/
		tmessage *parsedMessage; /**< spracovana sprava*/
		std::string response;		/**< ukazatel na spravu odpovede*/
		int GetData();
		void StoreData();
		void Notify(std::string MSG);
		Loger *_log;
		int _timeTimeOut;
	public:
		ConnectionServer(soci::session *SQL, Loger *L, int timeOut);
		void HandleConnection(in_addr IP);
		void SetSocket(int s);
		~ConnectionServer();
};



#endif /* CONNECTIONSERVER_H_ */
