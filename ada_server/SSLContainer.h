/*
 * SSLContainer.h
 *
 *  Created on: Mar 24, 2015
 *      Author: tuso
 */

#ifndef SSLCONTAINER_H_
#define SSLCONTAINER_H_

#include <string.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <unistd.h>


class SSLContainer
{
	private:
		long long adapters[1000];
		SSL* SSLs[1000];
		int size;
	public:
		SSLContainer();
		~SSLContainer();
		void InsertSSL(long long adapter,SSL *ssl);
		SSL* GetSSL(long long adapter);
};



#endif /* SSLCONTAINER_H_ */
