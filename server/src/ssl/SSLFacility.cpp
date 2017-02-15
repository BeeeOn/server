#include <Poco/Version.h>
#include <Poco/String.h>
#include <Poco/Logger.h>
#include <Poco/Delegate.h>
#include <Poco/StringTokenizer.h>
#include <Poco/NumberParser.h>
#include <Poco/NumberFormatter.h>
#include <Poco/Net/Utility.h>
#include <Poco/Net/SSLManager.h>

#include "ssl/SSLFacility.h"
#include "ssl/RejectCertificateHandler.h"

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
	m_sessionCache(false),
	m_disabledProtocols(-1)
{
	initializeSSL();

#if POCO_VERSION >= 0x01070000
	m_disabledProtocols = Context::PROTO_SSLV2 | Context::PROTO_SSLV3;
#endif
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

#if POCO_VERSION >= 0x01070000
	m_context->disableProtocols(m_disabledProtocols);
#else
	if (m_disabledProtocols)
		logger().warning("protocols are not disabled, linked against too old Poco library: "
				+ NumberFormatter::formatHex(POCO_VERSION, 10, true),
				__FILE__, __LINE__);
#endif

	logger().information("SSL context initialized",
				__FILE__, __LINE__);

	logger().debug("caLocation: " + m_caLocation);
	logger().debug("loadDefaultCA: " + to_string(m_loadDefaultCA));
	logger().debug("privateKey: " + m_privateKey);
	logger().debug("passphrase: " + to_string(!m_passphrase.empty()));
	logger().debug("certificate: " + m_certificate);
	logger().debug("verificationMode: " + to_string(m_verificationMode));
	logger().debug("verificationDepth: " + to_string(m_verificationDepth));
	logger().debug("cihperList: " + m_cipherList);
	logger().debug("sessionCache: " + to_string(m_sessionCache));
	logger().debug("disableProtocols: " + NumberFormatter::formatHex(m_disabledProtocols, 10, true));
}

Context::Ptr SSLFacility::context()
{
	initContext();
	return m_context;
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

static unsigned int parseProtocol(const string &protocol)
{
#if POCO_VERSION >= 0x01070000
	if (!icompare(protocol, "sslv2"))
		return Context::PROTO_SSLV2;
	else if (!icompare(protocol, "sslv3"))
		return Context::PROTO_SSLV3;
	else if (!icompare(protocol, "tlsv1"))
		return Context::PROTO_TLSV1;
	else if (!icompare(protocol, "tlsv1_1"))
		return Context::PROTO_TLSV1_1;
	else if (!icompare(protocol, "tlsv1_2"))
		return Context::PROTO_TLSV1_2;
	else
#endif
		return 0;
}

void SSLFacility::setDisabledProtocols(const string &protocols)
{
	StringTokenizer tokenizer(protocols, ",;",
		StringTokenizer::TOK_IGNORE_EMPTY | StringTokenizer::TOK_TRIM);

	unsigned int disable = 0;

	for (auto tok : tokenizer) {
		logger().debug("parsing SSL protocol: " + tok,
				__FILE__, __LINE__);

		unsigned int ret = parseProtocol(tok);
		if (ret == 0) {
			logger().warning("failed to recognize SSL protocol: " + tok,
					__FILE__, __LINE__);
		}

		disable |= ret;
	}

	m_disabledProtocols = disable;
}
