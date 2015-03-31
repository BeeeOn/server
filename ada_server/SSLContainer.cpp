#include "SSLContainer.h"


SSLContainer::SSLContainer()
{
	this->size=0;
	//this->SSLs = new SSL*[1000];
	//this->adapters = new int[1000];
}

SSLContainer::~SSLContainer()
{
	for (int i=0;i<size;i++)
	{
		SSL_shutdown(SSLs[i]);
		SSL_free(SSLs[i]);
	}
	/*delete[] SSLs;
	delete[] adapters;*/
	size = 0;
}

void SSLContainer::InsertSSL(long long adapter,SSL *ssl)
{
	for (int i=0;i<size;i++)
	{
		if (adapters[i]==adapter)
		{
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
