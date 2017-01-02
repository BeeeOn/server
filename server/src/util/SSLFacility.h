#ifndef BEEEON_SSL_FACILITY_H
#define BEEEON_SSL_FACILITY_H

#include <string>
#include <Poco/Mutex.h>
#include <Poco/Net/Context.h>

#include "di/InjectorTarget.h"

namespace Poco {

class Logger;

}

namespace BeeeOn {

/**
 * Passphrase provider intended for the next private key being load.
 * If multiple private keys share the same passphrase (not recommended)
 * then it is possible to reuse the created instance. It avoids collisions
 * of passwords when multiple private keys are loaded by independent
 * threads. Also it ensures that the given password is used for the
 * private key until the provider instance is destroyed.
 *
 * Typical usage:
 *
 *   {
 *       PrivateKeyPassphraseProvider provider("secret passphrase");
 *       Context::Ptr c = new Context(...);
 *   }
 *
 * If a private key is being load by the Context it the provider
 * provides the preset password. It is required that every Context
 * creation is done by using the PrivateKeyPassphraseProvider or
 * all such places share the same Mutex.
 */
class PrivateKeyPassphraseProvider {
public:
	PrivateKeyPassphraseProvider(const std::string &passphrase);
	PrivateKeyPassphraseProvider(const std::string &passphrase,
				Poco::Mutex &lock);
	~PrivateKeyPassphraseProvider();

	void onRequest(const void *sender, std::string &passphrase);

private:
	void init();

private:
	const std::string m_passphrase;
	Poco::Mutex &m_lock;
	static Poco::Mutex defaultLock;
};

/**
 * Abstract class for configuration of SSL context. It allows
 * to set the most common settings of the Context. Properties
 * specific to client or server context are to be added by
 * the inhertited classes.
 *
 * The SSLFacility provides the following properties for
 * dependency-injection setup:
 *
 * - caLocation: string (empty)
 * - loadDefaultCA: string ("false")
 * - privateKey - string (empty)
 * - passphrase - string (empty)
 * - certificate - string (empty)
 * - verificationMode - string ("relaxed")
 * - verificationDepth - number (9)
 * - cipherList - string ("ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH")
 * - sessionCache - string ("false")
 */
class SSLFacility : public AbstractInjectorTarget {
public:
	SSLFacility();
	virtual ~SSLFacility();

	void setCALocation(const std::string &caLocation);
	void setLoadDefaultCA(const std::string &enable);
	void setPrivateKey(const std::string &file);
	void setPassphrase(const std::string &passphrase);
	void setCertificate(const std::string &file);
	void setVerificationMode(const std::string &mode);
	void setVerificationDepth(int depth);
	void setCipherList(const std::string &list);
	void setSessionCache(const std::string &enable);
	void setDisabledProtocols(const std::string &protocols);

	Poco::Net::Context::Ptr context();

protected:
	void initContext();
	virtual Poco::Net::Context::Ptr createContext() = 0;
	virtual void injectionDone() override;

protected:
	using VerificationMode =
		typename Poco::Net::Context::VerificationMode;

	Poco::Logger &m_logger;
	Poco::Mutex m_lock;
	Poco::Net::Context::Ptr m_context;
	std::string m_caLocation;
	bool m_loadDefaultCA;
	std::string m_privateKey;
	std::string m_passphrase;
	std::string m_certificate;
	VerificationMode m_verificationMode;
	int m_verificationDepth;
	std::string m_cipherList;
	bool m_sessionCache;
	unsigned int m_disabledProtocols;
};

}

#endif
