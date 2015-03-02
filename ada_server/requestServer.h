/*
 * requestHandler.h
 *
 *  Created on: Feb 22, 2015
 *      Author: tuso
 */

#ifndef REQUESTHANDLER_H_
#define REQUESTHANDLER_H_

#include "loger.h"
#include "sender.h"
#include "messageCreator.h"
#include "DBHandler.h"
#include <arpa/inet.h> //kniznica pre internetove operacie
#include <netdb.h> //kniznica pre sietove operacie
#include <cstdlib>  //kniznica pre standarnde funkcie a dynamicku pamat
#include <netinet/in.h>  //kniznica pre sietovy protokol
#include <sys/socket.h>  //kniznica pre sockety
#include <sys/types.h>  //kniznica pre systemove typy
#include <string.h>  //kniznica pre znakove retazce
#include <unistd.h>  //kniznica pre standardne symbolicke konstanty a typy
#include <signal.h>  //kniznica pre signaly
#include <sys/wait.h> //kniznica pre funkciu wait
#include <string>  //c++ znakove retazce
#include <semaphore.h>
#include <exception> //kniznica pre bok try/catch

class RequestServer
{
	private:
		const std::string _Name = "RequestServer";
		int com_s;
		DBHandler *database;    /**< ukazatel na objekt DBHandler*/
		std::string response;		/**< ukazatel na spravu odpovede*/
		int GetData();
		MessageCreator *MC;
		Sender *s;
		Loger *_log;
		//void StoreData();
	public:
		RequestServer(Loger *l,soci::session *SQl);
		bool HandleRequest();
		void HandleRequestCover();
		void SetSocket(int Soc);
		~RequestServer();
};


#endif /* REQUESTHANDLER_H_ */
