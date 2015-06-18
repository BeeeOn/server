/**
 * @file requestServer.h
 * 
 * @brief definition of RequestServer class
 *
 * @author Matus Blaho 
 * @version 1.0
 */

#ifndef REQUESTHANDLER_H_
#define REQUESTHANDLER_H_

#include "loger.h"
#include "sender.h"
#include "messageCreator.h"
#include "DBHandler.h"
#include <arpa/inet.h> 
#include <netdb.h> 
#include <cstdlib>  
#include <netinet/in.h>  
#include <sys/socket.h>  
#include <sys/types.h>  
#include <string.h>  
#include <unistd.h>  
#include <signal.h>  
#include <sys/wait.h> 
#include <string>  
#include <semaphore.h>
#include <exception> 
#include <bitset>
#include <errno.h>
#include <string.h>
#include "SSLContainer.h"
#include "messageParsers.h"

/**
 * @Class RequestServer
 * @brief Class to receive and parse message from the ui_server and sent it to adapter 
 */

class RequestServer
{
	private:
		const std::string _Name = "RequestServer"; /**< Name of class*/
		int com_s;  /**< communication socket*/
		DBHandler *database;    /**< database interface*/
		std::string response;		/**< response text*/
		/**Method to to obtain data from database
		 * @return int representing result
						 */
		int GetData();
		MessageCreator *MC; /**< reference to message creator*/
		Sender *s;  /**< reference to sender*/
 		Loger *_log; /**< reference to loger used for logging*/
		SSLContainer *_sslcont;  /**< reference to SSLContainer to pick SSL connection*/
		UIServerMessageParser *UIp; /**< reference to UIServerMessageParser*/
	public:
		/**Constructor
		 * @param l Loger used to log messages
		 * @param SQL soci::session pointer to database connection
		 * @param sslcont SSLContainer pointer to ssl container with active ssl connection
				 */
		RequestServer(Loger *l,soci::session *SQl, SSLContainer *sslcont);
		/**Method to handle request
		 * @return bool true/false on success/failure
				 */
		bool HandleRequest();
		/**Method to set clients socket
		 * @param Soc int representation of socket descriptor
				 */
		void SetSocket(int Soc);
		/**Descrutor
				 */
		~RequestServer();
};


#endif /* REQUESTHANDLER_H_ */
