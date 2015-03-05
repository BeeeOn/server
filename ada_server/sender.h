/*
 * sender.h
 *
 *  Created on: Feb 22, 2015
 *      Author: tuso
 */

#ifndef SENDER_H_
#define SENDER_H_

#include <netdb.h>
#include <errno.h>
#include <cstdlib>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "loger.h"

class Sender
{
	private:
    	const std::string _Name="Sender";
		int _port;
		int s;
		Loger *_log;
	public:
		bool Connect(std::string Message,std::string IP);
		Sender(Loger *l,int p);
		~Sender();
};


#endif /* SENDER_H_ */
