#ifndef BEEEON_FACEBOOK_AUTH_PROVIDER_H

#define BEEEON_FACEBOOK_AUTH_PROVIDER_H

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Net/HTTPSClientSession.h>

#include "provider/OAuth2AuthProvider.h"

namespace BeeeOn {

	class FacebookAuthProvider : public OAuth2AuthProvider{
	public:
		FacebookAuthProvider():
			OAuth2AuthProvider("facebook")
		{
		}

	protected:
		/**
		 * Verifies the given authCode and obtains access token and user info. The fetched accessToken is verified via
		 * the method FacebookAuthProvider::processAccessToken.
		 */
		bool verifyAuthCode(const std::string &authCode, AuthResult &info);

		/**
		 *  Verifies access token and makes calls to facebook graph api to gain user info
		 *  required to authentificate user.
		 *
		 *  On success returns true. False is returned if any of the verification methods fails.
		 */
		bool processAccessToken(const std::string &accessToken, AuthResult &info) const;

		/**
		 * Sets URL for token requests.
		 */
		void setRequestTokenUrl(const std::string &url)
		{
			m_requestTokenUrl = url;
		}

		/**
		 * Sets URL for requests for User data.
		 */
		void setUserInfoUrl(const std::string &url)
		{
			m_userInfoUrl = url;
		}

		/**
		 * Sets URL for token inspections.
		 */
		void setInspectTokenUrl(const std::string &url)
		{
			m_inspectTokenUrl = url;
		}

	private:
		/**
		 * URL for token inspection requests.
		 */
		std::string m_inspectTokenUrl = "https://graph.facebook.com/debug_token";

		/**
		 * URL for token requests.
		 */
		std::string m_requestTokenUrl = "https://graph.facebook.com/oauth/access_token";

		/**
		 * URL for requests to get user information.
		 */
		std::string m_userInfoUrl = "https://graph.facebook.com/me";

		/**
		 * Makes a call to the Facebook Graph API to obtain user data.
		 */
		std::string requestAccessToken(const std::string &authCode) const;

		/**
		 * Makes call to Facebook Graph API to verify validity of received (inserted) access token.
		 * If token is not valid, exception with details is thrown.
		 */
		void verifyAccessToken(const std::string &accessToken) const;

		/**
		 * Makes call to facebook graph api to request user data which are returned as JSON string.
		 * If the data contain an error message exception with details of error message is thrown.
		 *
		 * Note:
		 * Facebook may not return email if:
		 * 1) User has signed up with telephone number
		 * 2) User didn't gave permissions to the app (another call involving user to the graph api is required)
		 *  To request user to provide missing email make call of type POST:
		 *  - host:	https://www.facebook.com/v2.9/dialog/oauth?
		 *  - request:	client_id={app-id} &
		 *		redirect_uri={redirect-uri} &
		 *		auth_type=rerequest & (Note: rerequst, not request)
		 *		scope=email
		 *
		 * 3) User does not have confirmed his/her primary email adress yet (freshly registered user, for example)
		 * - Such user must be requested to check his/her facebook settings
		 */
		std::string fetchUserData(const std::string &accessToken) const;

		/**
		 * Parses identity of user.
		 */
		bool parseIdentity(const std::string &userInfo, const std::string &accessToken, AuthResult &result) const;

	};

}

#endif
