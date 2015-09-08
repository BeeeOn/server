/**
 * @file messageCreator.h
 * 
 * @brief definition of MessageCreator class
 *
 * @author Matus Blaho 
 * @version 1.0
 */
#include <exception>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "structures.h"
#include "../lib/pugixml.hpp"
#include "../lib/pugiconfig.hpp"
#include "loger.h"
#include "structures.h"

#ifndef MESSAGECREATOR_H_
#define MESSAGECREATOR_H_


/** @Class MessageCreator
 *  @brief Creates messages for adapter
 */
class MessageCreator
{
	private:
		const std::string _Name = "MessageCreator"; /**< Name of class*/
		Loger *_log;  /**< reference to logger*/
	public:
	/**Method to create deletion message for adapter
	 * @param device ID std::string caring ID of device to be deleted
	 */
	std::string CreateDeleteMessage(std::string deviceID);
	/**Method to create Listen message for adapter
	 * @param std::string caring ID of adapter	 */
	std::string CreateListenMessage(std::string AdapterID);
	/**Method to create Switch message
		 * @param tmessage caring details	 */
	std::string CreateSwitchMessage(tmessage *Message);
	/**Constructor
		 * @param l Loger used for logging	 */
	MessageCreator(Loger *_l);
};





#endif /* MESSAGECREATOR_H_ */
