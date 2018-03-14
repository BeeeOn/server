#include "jwt/JWToken.h"

using namespace std;
using namespace BeeeOn;
using namespace Poco;

JWToken::JWToken()
{
}

JWToken::~JWToken()
{
}

void JWToken::setIssuer(const string &issuer)
{
	m_issuer = issuer;
}

string JWToken::issuer() const
{
	return m_issuer;
}

void JWToken::setSubject(const string &subject)
{
	m_subject = subject;
}

string JWToken::subject() const
{
	return m_subject;
}

void JWToken::setAudience(const set<string> &audience)
{
	m_audience = audience;
}

set<string> JWToken::audience() const
{
	return m_audience;
}

void JWToken::setExpirationTime(const Timestamp &time)
{
	m_expiration = time;
}

Nullable<Timestamp> JWToken::expirationTime() const
{
	return m_expiration;
}

void JWToken::setIssuedAt(const Timestamp &time)
{
	m_issuedAt = time;
}

Nullable<Timestamp> JWToken::issuedAt() const
{
	return m_issuedAt;
}

void JWToken::setNotBefore(const Timestamp &time)
{
	m_notbefore = time;
}

Nullable<Timestamp> JWToken::notBefore() const
{
	return m_notbefore;
}

void JWToken::setGivenName(const string &name)
{
	m_givenName = name;
}

Nullable<string> JWToken::givenName() const
{
	return m_givenName;
}

void JWToken::setFamilyName(const string &name)
{
	m_familyName = name;
}

Nullable<string> JWToken::familyName() const
{
	return m_familyName;
}

void JWToken::setPicture(const URI &uri)
{
	m_picture = uri;
}

Nullable<URI> JWToken::picture() const
{
	return m_picture;
}

void JWToken::setEmail(const string &email)
{
	m_email = email;
}

Nullable<string> JWToken::email() const
{
	return m_email;
}

void JWToken::setLocale(const string &locale)
{
	m_locale = locale;
}

string JWToken::locale() const
{
	return m_locale;
}

bool JWToken::hasExpired(const Timestamp &at) const
{
	return m_expiration.value() <= at;
}

bool JWToken::isAcceptable(const Timestamp &startTime) const
{
	return m_notbefore.value() <= startTime;
}

bool JWToken::isInAudience(const string &recipient) const
{
	return m_audience.find(recipient) != m_audience.end();
}
