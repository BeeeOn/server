/**
 * @file SSLContainer.h
 * 
 * @brief definition of SSLContainer class
 *
 * @author Matus Blaho 
 * @version 1.0
 */

#ifndef SSLCONTAINER_H_
#define SSLCONTAINER_H_
#include "loger.h"
#include "structures.h"
#include <string.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <unistd.h>
#include <map>

/** @Class SSLContainer
 *  @brief Stores SSL connections with adapters
 */

class SSLContainer
{
	private:
		long long adapters[1000]; /**< adapter ids*/
		SSL* SSLs[1000]; /**< SSLconncetions*/
		int size; /**< size of structure*/
		const std::string _Name="SSLContainer"; /**< Class name*/
		Loger *_log; /**< logger*/
		std::map<unsigned long long int, tadapter*> *container;
	public:
		/**Constructor
		 * @param l Loger used to log messages */
		SSLContainer(Loger *l);
		/** Destructor
		 */
		~SSLContainer();
		/** Method to store SSL and adapter id in container
		 * @param adapter ID
		 * @ ssl connection pointer
				 */
		void InsertSSL(unsigned long long adapterID,SSL *ssl,float cp);
		/** Method to return SSL connection
		 * @param adapter ID
				 */
		tadapter* GetSSL(unsigned long long adapter);
};



#endif /* SSLCONTAINER_H_ */
