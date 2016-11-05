#include <Poco/String.h>
#include <Poco/StringTokenizer.h>
#include <Poco/NumberParser.h>
#include <Poco/Net/Utility.h>
#include <Poco/Net/SSLManager.h>

#include "util/SSLServer.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

SSLServer::SSLServer()
{
}

SSLServer::~SSLServer()
{
}

Context::Ptr SSLServer::createContext()
{
	Context::Ptr context = new Context(
		Context::SERVER_USE,
		m_privateKey,
		m_certificate,
		m_caLocation,
		m_verificationMode,
		m_verificationDepth,
		m_loadDefaultCA,
		m_cipherList
	);

	context->enableSessionCache(m_sessionCache);
	return context;
}

BEEEON_OBJECT(SSLServer, BeeeOn::SSLServer)
