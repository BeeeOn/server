#ifndef BEEEON_GOOGLE_AUTH_PROVIDER_H
#define BEEEON_GOOGLE_AUTH_PROVIDER_H

#include <Poco/Timespan.h>
#include <Poco/Net/HTTPSClientSession.h>

#include "provider/OAuth2AuthProvider.h"

namespace BeeeOn {

/**
 * Verify the user access token against Google servers and obtain
 * information about her.
 */
class GoogleAuthProvider : public OAuth2AuthProvider {
public:
	GoogleAuthProvider():
		OAuth2AuthProvider("google")
	{
	}

protected:
	bool verifyAuthCode(
		const AuthCodeCredentials &credentials,
		AuthResult &info);

private:
	struct GoogleTokens {
		std::string accessToken;
		std::string refreshToken;
		std::string idToken;
		std::string tokenType;
		Poco::Timespan expiresIn;
	};

	/**
	 * Get token info a.k.a. user info from Google API
	 */
	std::string fetchUserInfo(const GoogleTokens &tokens);

	/**
	 * Get access token from Google APIs
	 */
	GoogleTokens requestTokens(const std::string &authCode);

	/**
	 * Parse userInfo data and fill the AuthResult instance.
	 */
	bool parseIdentity(const std::string &userInfo,
			const GoogleTokens &tokens, AuthResult &result);

	// URL to get access and ID token from Google API
	const std::string m_tokenUrl = "https://www.googleapis.com/oauth2/v4/token";
	// URL to get user info using ID token
	const std::string m_tokenInfoUrl =
		"https://www.googleapis.com/oauth2/v3/tokeninfo";
};

}

#endif
