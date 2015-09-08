/**
 * @file connectionServer.h
 * 
 * @brief definition of ConnectionServer Class
 *
 * @author Matus Blaho
 * @version 1.0
 */

#ifndef CONNECTIONSERVER_H_
#define CONNECTIONSERVER_H_

#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <chrono>
#include <iostream>
#include <netdb.h>
#include <cstdlib>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "../lib/pugixml.hpp"
#include "../lib/pugiconfig.hpp"
#include "config.h"
#include "DBHandler.h"
#include "loger.h"
#include "messageParsers.h"
#include "SSLContainer.h"

/** @class ConnectionServer
 *  @brief Class responsible for receiving and serving clients requests
 */
class ConnectionServer
{
	private:
		const std::string _Name="ConnectionServer"; /**< member representing class name*/
		int com_s;	/**< communication socket*/
		DBHandler *database;    /**< reference to DBHandler object*/
		MessageParser *MP;	/**< reference to MessageParser object*/
		tmessage *parsedMessage; /**< reference to tmessage object for parsed meesage*/
		std::string response;		/**< text of response*/
		 /**
		   * Obtainng wakeup time
		   * @return integer representation of next wakeup
		   */
		int GetData();
		/**
		  * Storing data logic
		  */
		void StoreData();
		/**
		* Method to send received message also to framework
		* @param MSG std::string containng message text.
		*/
		void Notify(std::string MSG);
		Loger *_log;	/**< reference to Loger object used for logging*/
		int _timeTimeOut;	/**< timeout for unresponsive client*/
		SSL_CTX *sslctx;	/**< SSL context of server*/
		SSL *cSSL;	/**< current SSL connection*/
		SSLContainer *_sslcont;  /**< reference to ssl container used for saving connections*/
		Config *_config;  /**< reference to Config object used for loading configuration*/
	public:
		ConnectionServer(soci::session *SQL, Loger *L, int timeOut,SSLContainer *sslcont,Config *c);
		/** Method for receiving message and creating response
		 * @param IP - IP address of client
		    */
		void HandleConnection(in_addr IP);
		/**
		* Method to set socket to read message from
		* @param s socket which to be set.
		*/
		void SetSocket(int s);
		/**
		* Destructor of object
		*/
		~ConnectionServer();
		/** Method to load server certificates */
		bool LoadCertificates();
};



#endif /* CONNECTIONSERVER_H_ */
