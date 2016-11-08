#include <Poco/String.h>
#include <Poco/StringTokenizer.h>
#include <Poco/NumberParser.h>
#include <Poco/Net/Utility.h>
#include <Poco/Net/SSLManager.h>

#include "util/SSLClient.h"
#include "util/RejectCertificateHandler.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

SSLClient::SSLClient():
	m_certHandler(new BetterRejectCertificateHandler(false))
{
}

SSLClient::~SSLClient()
{
}

Context::Ptr SSLClient::createContext()
{
	Context::Ptr context = new Context(
		Context::CLIENT_USE,
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

BEEEON_OBJECT(SSLClient, BeeeOn::SSLClient)
