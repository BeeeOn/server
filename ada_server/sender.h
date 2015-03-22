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
#include <errno.h>
#include "loger.h"

class Sender
{
	private:
    	const std::string _Name="Sender";
		int s;
		Loger *_log;
	public:
		bool Send(std::string Message,int soc);
		Sender(Loger *l);
		~Sender();
};


#endif /* SENDER_H_ */
