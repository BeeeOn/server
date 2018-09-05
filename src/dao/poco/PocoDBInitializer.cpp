#include <Poco/Data/Session.h>
#include <Poco/Data/Statement.h>

#include "dao/poco/PocoDBInitializer.h"

using namespace std;
using namespace Poco::Data;
using namespace BeeeOn;

PocoDBInitializer::PocoDBInitializer():
	m_executor(new PocoDefaultScriptExecutor)
{
}

PocoDBInitializer::~PocoDBInitializer()
{
}

void PocoDBInitializer::setExecutor(PocoScriptExecutor::Ptr executor)
{
	m_executor = executor;
}

unsigned long PocoDBInitializer::execute(
		Session &session, const string &script) const
{
	return m_executor->execute(session, script);
}
