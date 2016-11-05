#include <Poco/String.h>
#include <Poco/Delegate.h>
#include <Poco/StringTokenizer.h>
#include <Poco/NumberParser.h>
#include <Poco/Net/Utility.h>
#include <Poco/Net/SSLManager.h>

#include "util/SSLFacility.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

Mutex PrivateKeyPassphraseProvider::defaultLock;

PrivateKeyPassphraseProvider::PrivateKeyPassphraseProvider(
		const std::string &passphrase):
	m_passphrase(passphrase),
	m_lock(PrivateKeyPassphraseProvider::defaultLock)
{
	init();
}

PrivateKeyPassphraseProvider::PrivateKeyPassphraseProvider(
		const std::string &passphrase, Mutex &lock):
	m_passphrase(passphrase),
	m_lock(lock)
{
	init();
}

void PrivateKeyPassphraseProvider::init()
{
	m_lock.lock();

	SSLManager &manager = SSLManager::instance();
	manager.PrivateKeyPassphraseRequired +=
		Delegate<PrivateKeyPassphraseProvider, string>(
			this,
			&PrivateKeyPassphraseProvider::onRequest
		);
}

PrivateKeyPassphraseProvider::~PrivateKeyPassphraseProvider()
{
	SSLManager &manager = SSLManager::instance();
	manager.PrivateKeyPassphraseRequired -=
		Delegate<PrivateKeyPassphraseProvider, string>(
			this,
			&PrivateKeyPassphraseProvider::onRequest
		);

	m_lock.unlock();
}

void PrivateKeyPassphraseProvider::onRequest(
		const void *sender, string &passphrase)
{
	passphrase = m_passphrase;
}

SSLFacility::SSLFacility():
	m_loadDefaultCA(false),
	m_verificationMode(VerificationMode::VERIFY_RELAXED),
	m_verificationDepth(9),
	m_cipherList("ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"),
	m_sessionCache(false)
{
	initializeSSL();

	textInjector("caLocation", (TextSetter)
			&SSLFacility::setCALocation);
	textInjector("loadDefaultCA", (TextSetter)
			&SSLFacility::setLoadDefaultCA);
	textInjector("privateKey", (TextSetter)
			&SSLFacility::setPrivateKey);
	textInjector("passphrase", (TextSetter)
			&SSLFacility::setPassphrase);
	textInjector("certificate", (TextSetter)
			&SSLFacility::setCertificate);
	textInjector("verificationMode", (TextSetter)
			&SSLFacility::setVerificationMode);
	numberInjector("verificationDepth", (NumberSetter)
			&SSLFacility::setVerificationDepth);
	textInjector("cipherList", (TextSetter)
			&SSLFacility::setCipherList);
	textInjector("sessionCache", (TextSetter)
			&SSLFacility::setSessionCache);
}

SSLFacility::~SSLFacility()
{
	uninitializeSSL();
}

void SSLFacility::initContext()
{
	Mutex::ScopedLock guard(m_lock);

	if (!m_context.isNull())
		return;

	if (!m_passphrase.empty()) {
		PrivateKeyPassphraseProvider provider(m_passphrase);
		m_context = createContext();
	} else {
		m_context = createContext();
	}
}

Context::Ptr SSLFacility::context()
{
	initContext();
	return m_context;
}

void SSLFacility::injectionDone()
{
	initContext();
}

void SSLFacility::setSessionCache(const std::string &enable)
{
	m_sessionCache = NumberParser::parseBool(enable);
}

void SSLFacility::setCALocation(const string &location)
{
	m_caLocation = location;
}

void SSLFacility::setLoadDefaultCA(const string &enable)
{
	NumberParser::parseBool(enable);
}

void SSLFacility::setPrivateKey(const string &file)
{
	m_privateKey = file;
}

void SSLFacility::setPassphrase(const std::string &passphrase)
{
	m_passphrase = passphrase;
}

void SSLFacility::setCertificate(const string &file)
{
	m_certificate = file;
}

void SSLFacility::setVerificationMode(const string &mode)
{
	m_verificationMode = Utility::convertVerificationMode(mode);
}

void SSLFacility::setVerificationDepth(int depth)
{
	m_verificationDepth = depth;
}

void SSLFacility::setCipherList(const string &list)
{
	m_cipherList = list;
}
