#ifndef BEEEON_SSL_FACILITY_H
#define BEEEON_SSL_FACILITY_H

#include <string>
#include <Poco/Mutex.h>
#include <Poco/Net/Context.h>

#include "di/InjectorTarget.h"

namespace BeeeOn {

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
	void setCertificate(const std::string &file);
	void setVerificationMode(const std::string &mode);
	void setVerificationDepth(int depth);
	void setCipherList(const std::string &list);
	void setSessionCache(const std::string &enable);

	Poco::Net::Context::Ptr context();

protected:
	void initContext();
	virtual Poco::Net::Context::Ptr createContext() = 0;
	virtual void injectionDone() override;

protected:
	using VerificationMode =
		typename Poco::Net::Context::VerificationMode;

	Poco::Mutex m_lock;
	Poco::Net::Context::Ptr m_context;
	std::string m_caLocation;
	bool m_loadDefaultCA;
	std::string m_privateKey;
	std::string m_certificate;
	VerificationMode m_verificationMode;
	int m_verificationDepth;
	std::string m_cipherList;
	bool m_sessionCache;
};

}

#endif
