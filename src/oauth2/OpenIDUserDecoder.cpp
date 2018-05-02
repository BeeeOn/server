#include <Poco/Logger.h>
#include <Poco/Timestamp.h>
#include <Poco/Net/NetException.h>

#include "di/Injectable.h"
#include "oauth2/OpenIDUserDecoder.h"
#include "util/Sanitize.h"

BEEEON_OBJECT_BEGIN(BeeeOn, OpenIDUserDecoder)
BEEEON_OBJECT_CASTABLE(OAuth2UserFetcher)
BEEEON_OBJECT_PROPERTY("decoder", &OpenIDUserDecoder::setDecoder)
BEEEON_OBJECT_PROPERTY("clientId", &OpenIDUserDecoder::setClientId)
BEEEON_OBJECT_END(BeeeOn, OpenIDUserDecoder)

using namespace std;
using namespace BeeeOn;
using namespace Poco;
using namespace Poco::Net;

OpenIDUserDecoder::OpenIDUserDecoder()
{
}

void OpenIDUserDecoder::setDecoder(JWTDecoder::Ptr decoder)
{
	m_decoder = decoder;
}

void OpenIDUserDecoder::setClientId(const string &clientId)
{
	m_clientId = clientId;
}

string OpenIDUserDecoder::clientId() const
{
	return m_clientId;
}

void OpenIDUserDecoder::fetch(
		const OAuth2CodeExchanger::Tokens &tokens,
		AuthResult &result,
		const Timestamp &now)
{
	if (tokens.idToken.empty())
		throw NotAuthenticatedException("missing ID token");

	const auto &jwt = m_decoder->decode(tokens.idToken);

	if (logger().trace()) {
		logger().trace(
			"ID token decoded: " + jwt.toJSONString(),
			__FILE__, __LINE__);
	}
	else if (logger().debug()) {
		logger().debug(
			"ID token decoded as JWT successfully",
			__FILE__, __LINE__);
	}

	if (!jwt.isAcceptable(now))
		throw NotAuthenticatedException("user info is not valid yet (nbf)");

	if (jwt.hasExpired(now))
		throw NotAuthenticatedException("user info has expired (exp)");

	if (!m_clientId.empty()) {
		if (!jwt.isInAudience(m_clientId))
			throw NotAuthenticatedException("clientId does not match (aud)");
	}

	result.setProviderID(jwt.subject());

	if (!jwt.email().isNull())
		result.setEmail(Sanitize::email(jwt.email()));

	if (!jwt.givenName().isNull())
		result.setFirstName(Sanitize::common(jwt.givenName()));

	if (!jwt.familyName().isNull())
		result.setLastName(Sanitize::common(jwt.familyName()));

	if (!jwt.locale().empty())
		result.setLocale(Sanitize::locale(jwt.locale()));
}
