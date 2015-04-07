#include "SSLContainer.h"


SSLContainer::SSLContainer(Loger *l)
{
	l->WriteMessage(TRACE,"Entering " + this->_Name + "::Constructor");
	this->size=0;
	this->_log=l;
	//this->SSLs = new SSL*[1000];
	//this->adapters = new int[1000];
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Constructor");
}

SSLContainer::~SSLContainer()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::Destructor");
	for (int i=0;i<size;i++)
	{
		close(SSL_get_fd(SSLs[i]));
		SSL_shutdown(SSLs[i]);
		SSL_free(SSLs[i]);
	}
	/*delete[] SSLs;
	delete[] adapters;*/
	size = 0;
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Destructor");
}

void SSLContainer::InsertSSL(long long adapter,SSL *ssl)
{
	for (int i=0;i<size;i++)
	{
		if (adapters[i]==adapter)
		{
			close(SSL_get_fd(SSLs[i]));
			SSL_shutdown(SSLs[i]);
			SSL_free(SSLs[i]);
			SSLs[i] = ssl;
			return;
		}
	}
	adapters[size]=adapter;
	SSLs[size]=ssl;
	size++;
}

SSL* SSLContainer::GetSSL(long long adapter)
{
	for(int i=0; i<size;i++)
	{
		if (adapters[i]==adapter)
			return(SSLs[i]);
	}
	return (NULL);
}
