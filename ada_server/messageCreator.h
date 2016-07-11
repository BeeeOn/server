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
#include "loger.h"

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
	/**Method to create Search message for adapter
	 * @param std::string caring ID of adapter	 
	 * @param std::string caring IP of device	to search */
	std::string CreateSearchMessage(std::string AdapterID, std::string deviceIp, std::string deviceId);
	/**Method to create Switch message
		 * @param tmessageV1_0 caring details	 */
	std::string CreateSwitchMessage(tmessageV1_0 *Message);
	/**Constructor
		 * @param l Loger used for logging	 */
	std::string CreatePingMessage(tadapter *adapter);
	MessageCreator(Loger *_l);
};





#endif /* MESSAGECREATOR_H_ */
