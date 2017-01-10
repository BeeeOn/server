#ifndef BEEEON_OAUTH2_AUTH_PROVIDER_H
#define BEEEON_OAUTH2_AUTH_PROVIDER_H

#include <Poco/SharedPtr.h>
#include <Poco/Net/Context.h>

#include "provider/AuthProvider.h"

namespace Poco {
namespace Net {

class HTTPSClientSession;

}
}

namespace BeeeOn {

class SSLClient;

class OAuth2AuthProvider : public AuthCodeAuthProvider {
public:
	OAuth2AuthProvider(const std::string &name);

	void setClientId(const std::string &clientId)
	{
		m_clientId = clientId;
	}

	void setClientSecret(const std::string &clientSecret)
	{
		m_clientSecret = clientSecret;
	}

	void setRedirectURI(const std::string &uri)
	{
		m_redirectURI = uri;
	}

	void setSSLConfig(SSLClient *config)
	{
		m_sslConfig = config;
	}

protected:
	/**
	 * Open HTTPS connection to the given host:port.
	 */
	Poco::SharedPtr<Poco::Net::HTTPSClientSession> connectSecure(
			const std::string &host,
			unsigned int port);

	/**
	 * Handle response based on given session.
	 */
	std::string handleResponse(Poco::Net::HTTPSClientSession &session);

	/**
	 * Convert istream response body to string
	 */
	std::string convertResponseToString(std::istream &rs);

private:
	/**
	 * Initialize SSL context for upcoming HTTPS requests
	 */
	void initSSL();

protected:
	std::string m_clientId;
	std::string m_clientSecret;
	std::string m_redirectURI;
	SSLClient *m_sslConfig;
};

}

#endif
