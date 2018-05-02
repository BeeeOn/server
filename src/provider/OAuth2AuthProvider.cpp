#include <Poco/Exception.h>
#include <Poco/Net/NetException.h>

#include "di/Injectable.h"
#include "provider/OAuth2AuthProvider.h"

using namespace std;
using namespace BeeeOn;
using namespace Poco;
using namespace Poco::Net;

BEEEON_OBJECT_BEGIN(BeeeOn, OAuth2AuthProvider)
BEEEON_OBJECT_CASTABLE(AuthProvider)
BEEEON_OBJECT_PROPERTY("id", &OAuth2AuthProvider::setId)
BEEEON_OBJECT_PROPERTY("name", &OAuth2AuthProvider::setName)
BEEEON_OBJECT_PROPERTY("codeExchanger", &OAuth2AuthProvider::setCodeExchanger)
BEEEON_OBJECT_PROPERTY("userFetcher", &OAuth2AuthProvider::setUserFetcher)
BEEEON_OBJECT_PROPERTY("tokenType", &OAuth2AuthProvider::setTokenType)
BEEEON_OBJECT_END(BeeeOn, OAuth2AuthProvider)

OAuth2AuthProvider::OAuth2AuthProvider():
	m_tokenType("Bearer")
{
}

void OAuth2AuthProvider::setId(const string &id)
{
	m_id = id;
}

const string &OAuth2AuthProvider::id() const
{
	return m_id;
}

void OAuth2AuthProvider::setName(const string &name)
{
	m_name = name;
}

const string &OAuth2AuthProvider::name() const
{
	return m_name;
}

string OAuth2AuthProvider::clientId() const
{
	return m_fetcher->clientId();
}

string OAuth2AuthProvider::redirectURI() const
{
	if (m_exchanger.isNull())
		return "";

	return m_exchanger->redirectUri();
}

void OAuth2AuthProvider::setCodeExchanger(OAuth2CodeExchanger::Ptr exchanger)
{
	m_exchanger = exchanger;
}

void OAuth2AuthProvider::setUserFetcher(OAuth2UserFetcher::Ptr fetcher)
{
	m_fetcher = fetcher;
}

void OAuth2AuthProvider::setTokenType(const string &type)
{
	m_tokenType = type;
}

bool OAuth2AuthProvider::authorize(const Credentials &cred, AuthResult &result)
{
	OAuth2CodeExchanger::Tokens tokens;

	if (typeid(cred) == typeid(AuthCodeCredentials)) {
		if (m_exchanger.isNull())
			throw NotAuthenticatedException("AuthCodeCredentials not configured for " + id());

		const AuthCodeCredentials &authCodeCredentials =
			static_cast<const AuthCodeCredentials &>(cred);

		tokens = m_exchanger->exchange(authCodeCredentials);
		m_fetcher->fetch(tokens, result);
		result.setProvider(name());
		return true;
	}
	if (typeid(cred) == typeid(AccessTokenCredentials)) {
		const AccessTokenCredentials &accessTokenCredentials =
			dynamic_cast<const AccessTokenCredentials &>(cred);

		tokens.accessToken = accessTokenCredentials.accessToken().toString();
		tokens.tokenType = m_tokenType;

		/*
		 * We have no idea about the real expire time unless the access token
		 * is decoded (which is not generally possible unless fetch is called).
		 * Thus, behave like the the access token seems to be valid.
		 */
		tokens.expiresIn = 1;

		m_fetcher->fetch(tokens, result);
		result.setProvider(name());
		return true;
	}

	throw NotAuthenticatedException("unrecognized credentials type");
}
