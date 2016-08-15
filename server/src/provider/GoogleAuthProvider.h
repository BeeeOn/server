#ifndef BEEEON_GOOGLE_AUTH_PROVIDER_H
#define BEEEON_GOOGLE_AUTH_PROVIDER_H

#include <Poco/Net/HTTPSClientSession.h>

#include "provider/AuthProvider.h"

namespace BeeeOn {

/**
 * Verify the user access token against Google servers and obtain
 * information about her.
 */
class GoogleAuthProvider : public AuthCodeAuthProvider {
public:
	GoogleAuthProvider():
		AuthCodeAuthProvider("google")
	{
		textInjector("client_id",
				(TextSetter) &GoogleAuthProvider::setClientId);
		textInjector("client_secret",
				(TextSetter) &GoogleAuthProvider::setClientSecret);
	}

	void setClientId(const std::string &clientId)
	{
		m_clientId = clientId;
	}

	void setClientSecret(const std::string &clientSecret)
	{
		m_clientSecret = clientSecret;
	}

protected:
	bool verifyAuthCode(const std::string &authCode, Result &info);

private:
	/**
	 * Get token info a.k.a. user info from Google API
	 */
	std::string fetchUserInfo(const std::string &token);

	/**
	 * Get access token from Google APIs
	 */
	std::string requestIdToken(const std::string &token);

	/**
	 * Handle response based on given session
	 */
	std::string handleResponse(Poco::Net::HTTPSClientSession &session);

	/**
	 * Initialize SSL context for upcoming HTTPS requests
	 */
	void initSSL();

	/**
	 * Convert istream response body to string
	 */
	std::string convertResponseToString(std::istream &rs);

	std::string m_clientId;
	std::string m_clientSecret;

	// URL to get access and ID token from Google API
	const std::string m_tokenUrl = "https://www.googleapis.com/oauth2/v4/token";
	// URL to get user info using ID token
	const std::string m_tokenInfoUrl =
		"https://www.googleapis.com/oauth2/v3/tokeninfo?id_token=";
};

}

#endif
