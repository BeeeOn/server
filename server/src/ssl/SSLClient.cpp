#include <Poco/String.h>
#include <Poco/StringTokenizer.h>
#include <Poco/NumberParser.h>
#include <Poco/Net/Utility.h>
#include <Poco/Net/SSLManager.h>

#include "di/Injectable.h"
#include "ssl/SSLClient.h"
#include "ssl/RejectCertificateHandler.h"

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

BEEEON_OBJECT_BEGIN(BeeeOn, SSLClient)
BEEEON_OBJECT_TEXT("caLocation", &SSLFacility::setCALocation)
BEEEON_OBJECT_TEXT("loadDefaultCA", &SSLFacility::setLoadDefaultCA)
BEEEON_OBJECT_TEXT("privateKey", &SSLFacility::setPrivateKey)
BEEEON_OBJECT_TEXT("passphrase", &SSLFacility::setPassphrase)
BEEEON_OBJECT_TEXT("certificate", &SSLFacility::setCertificate)
BEEEON_OBJECT_TEXT("verificationMode", &SSLFacility::setVerificationMode)
BEEEON_OBJECT_NUMBER("verificationDepth", &SSLFacility::setVerificationDepth)
BEEEON_OBJECT_TEXT("cipherList", &SSLFacility::setCipherList)
BEEEON_OBJECT_TEXT("sessionCache", &SSLFacility::setSessionCache)
BEEEON_OBJECT_TEXT("disabledProtocols", &SSLFacility::setDisabledProtocols)
BEEEON_OBJECT_HOOK("done", &SSLFacility::initContext)
BEEEON_OBJECT_END(BeeeOn, SSLClient)
