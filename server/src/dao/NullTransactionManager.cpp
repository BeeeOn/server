#include <Poco/SingletonHolder.h>
#include <Poco/Exception.h>

#include "dao/NullTransactionManager.h"

using namespace Poco;
using namespace BeeeOn;

NullTransactionManager::NullTransactionManager()
{
}

NullTransactionManager::~NullTransactionManager()
{
}

TransactionManager &NullTransactionManager::instance()
{
	static SingletonHolder<NullTransactionManager> singleton;
	return *singleton.get();
}

Transaction *NullTransactionManager::start()
{
	throw NullPointerException("missing transaction manager when calling start()");
}

Transaction *NullTransactionManager::current()
{
	throw NullPointerException("missing transaction manager when calling current()");
}
