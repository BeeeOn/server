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
	this->_log=l;
	this->container = new std::map <unsigned long long int, tadapter*>();
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Constructor");
}

SSLContainer::~SSLContainer()
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::Destructor");
	for (std::map<unsigned long long int, tadapter *>::iterator it = this->container->begin(); it != this->container->end(); ++it)
	{
		close(SSL_get_fd(it->second->connection));
		SSL_shutdown(it->second->connection);
		SSL_free(it->second->connection);
		it->second->connection = nullptr;
		delete it->second;
	}
	delete container;
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Destructor");
}

void SSLContainer::InsertSSL(unsigned long long adapterID,SSL *ssl,float cp)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::InsertSSL");
	tadapter *adapter;
	try
	{
		adapter = container->at(adapterID);
		this->_log->WriteMessage(DEBUG,"Adapter found rewriting data");
		close(SSL_get_fd(adapter->connection));
		delete adapter->connection;
		adapter->connection = ssl;
		adapter->protocol_version = cp;
	}
	catch (const std::exception &e)
	{
		this->_log->WriteMessage(DEBUG,"Inserting new adapter");
		adapter = new tadapter(ssl,cp);
		container->insert(std::pair<unsigned long long int,tadapter*>(adapterID,adapter));
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::InsertSSL");
}

tadapter* SSLContainer::GetSSL(unsigned long long adapter)
{
	this->_log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetSSL");
	try
	{
		return container->at(adapter);
	}
	catch (const std::exception &e)
	{
		this->_log->WriteMessage(WARN,"Adapter not found");
	}
	this->_log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetSSL");
	return nullptr;
}
