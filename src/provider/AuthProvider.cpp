#include <Poco/Logger.h>

#include "di/Injectable.h"
#include "provider/AuthProvider.h"
#include "provider/PermitAuthProvider.h"

BEEEON_OBJECT_BEGIN(BeeeOn, PermitAuthProvider)
BEEEON_OBJECT_CASTABLE(AuthProvider)
BEEEON_OBJECT_TEXT("resultProvider", &PermitAuthProvider::setResultProvider)
BEEEON_OBJECT_END(BeeeOn, PermitAuthProvider)

using namespace std;
using namespace BeeeOn;

Credentials::Credentials(const string &provider):
	m_provider(provider)
{
}

Credentials::~Credentials()
{
}

const string &Credentials::provider() const
{
	return m_provider;
}

AuthProvider::~AuthProvider()
{
}

AuthCodeCredentials::AuthCodeCredentials(
		const string &provider,
		const string &authCode):
	Credentials(provider),
	m_authCode(authCode)
{
}

const string &AuthCodeCredentials::authCode() const
{
	return m_authCode;
}

AuthResult::AuthResult()
{
}

void AuthResult::setEmail(const string &email)
{
	m_email = email;
}

string AuthResult::email() const
{
	return m_email;
}

void AuthResult::setProvider(const string &provider)
{
	m_provider = provider;
}

string AuthResult::provider() const
{
	return m_provider;
}

void AuthResult::setAccessToken(const string &accessToken)
{
	m_accessToken = accessToken;
}

string AuthResult::accessToken() const
{
	return m_accessToken;
}

void AuthResult::setProviderID(const string &providerID)
{
	m_providerID = providerID;
}

string AuthResult::providerID() const
{
	return m_providerID;
}

void AuthResult::setFirstName(const string &firstName)
{
	m_firstName = firstName;
}

string AuthResult::firstName() const
{
	return m_firstName;
}

void AuthResult::setLastName(const string &lastName)
{
	m_lastName = lastName;
}

string AuthResult::lastName() const
{
	return m_lastName;
}

void AuthResult::setPicture(const Poco::URI &picture)
{
	m_picture = picture;
}

const Poco::URI &AuthResult::picture() const
{
	return m_picture;
}

void AuthResult::setLocale(const string &locale)
{
	m_locale = locale;
}

string AuthResult::locale() const
{
	return m_locale;
}

AbstractAuthProvider::AbstractAuthProvider(const string &name):
	m_id(name),
	m_name(name)
{
}

void AbstractAuthProvider::setID(const string &id)
{
	m_id = id;
}

const string &AbstractAuthProvider::id() const
{
	return m_id;
}

const string &AbstractAuthProvider::name() const
{
	return m_name;
}
