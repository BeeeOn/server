/*
 * messageCreator.h
 *
 *  Created on: Feb 22, 2015
 *      Author: tuso
 */

#include <exception>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "structures.h"
#include "pugixml.hpp"
#include "pugiconfig.hpp"
#include "loger.h"

#ifndef MESSAGECREATOR_H_
#define MESSAGECREATOR_H_

class MessageCreator
{
	private:
		const std::string _Name = "MessageCreator";
		Loger *_log;
	public:
	std::string CreateDeleteMessage(std::string deviceID);
	std::string CreateListenMessage(std::string AdapterID);
	std::string CreateSwitchMessage(std::string Adapter,std::string deviceId, std::string value, std::string type, std::string offset);
	MessageCreator(Loger *_l);
};





#endif /* MESSAGECREATOR_H_ */
