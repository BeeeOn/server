/**
 * @file SSLContainer.cpp
 * 
 * @brief implementation of SSLContainer class
 *
 * @author Matus Blaho 
 * @version 1.0
 */
#include "SSLContainer.h"


SSLContainer::SSLContainer(Loger *l)
{
	l->WriteMessage(TRACE,"Entering " + this->_Name + "::Constructor");
	this->size=0;
	this->_log=l;
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Constructor");
}

SSLContainer::~SSLContainer()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::Destructor");
	for (int i=0;i<size;i++) //free all SSLs
	{
		close(SSL_get_fd(SSLs[i]));
		SSL_shutdown(SSLs[i]);
		SSL_free(SSLs[i]);
	}
	size = 0;
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Destructor");
}

void SSLContainer::InsertSSL(long long adapter,SSL *ssl)
{
	for (int i=0;i<size;i++) //go thorugh all SSLs
	{
		if (adapters[i]==adapter) //if you find it
		{
			close(SSL_get_fd(SSLs[i])); //delete old stuff
			SSL_shutdown(SSLs[i]);
			SSL_free(SSLs[i]);
			SSLs[i] = ssl; //replace it with new one
			return;
		}
	}
	adapters[size]=adapter;
	SSLs[size]=ssl;
	size++;
	this->_log->WriteMessage(INFO,"saved connections cout: " +std::to_string(size));
}

SSL* SSLContainer::GetSSL(long long adapter)
{
	for(int i=0; i<size;i++) //go through all ssls
	{
		if (adapters[i]==adapter) //if there is return pointer to it
			return(SSLs[i]);
	}
	return (NULL); //if there isn't return NULL
}
