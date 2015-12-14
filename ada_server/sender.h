/**
 * @file sender.h
 * 
 * @brief definition of sender class
 *
 * @author Matus Blaho 
 * @version 1.0
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
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

/** @Class Sender
 *  @brief Class to send message to adapter
 */

class Sender
{
	private:
    	const std::string _Name="Sender"; /**< class name*/
		int s; /**< socket*/
		Loger *_log; /**< reference to loger*/
		SSL_CTX *sslctx; /**< SSL context*/
		SSL *cSSL; /**< ssl connection*/
	public:
		/**Method to send message
		 * @param Message std::string text of message
		 * @param s pointer to SSL connection to use for sending
				 */
		bool Send(std::string Message,SSL *s);
		/**Constructor
		 * @param l Loger pointer to loger used for logging
				 */
		Sender(Loger *l);
		/**Destructor
				 */
		~Sender();
};


#endif /* SENDER_H_ */
