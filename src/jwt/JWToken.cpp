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
