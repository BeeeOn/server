#include <Poco/Net/NetException.h>
#include "service/AuthService.h"

using namespace std;
using namespace Poco::Net;
using namespace BeeeOn;

BEEEON_OBJECT(AuthService, BeeeOn::AuthService)

string AuthService::openSession(const VerifiedIdentity &verifiedIdentity)
{
	Identity identity(verifiedIdentity.identity().id());

	if (!m_identityDao->fetch(identity)) {
		throw NotAuthenticatedException(
			"failed to find identity "
				+ identity.id().toString()
				+ " for "
				+ verifiedIdentity.id().toString());
	}

	User user(identity.user());

	if (!m_userDao->fetch(user)) {
		throw NotAuthenticatedException("unknown user for identity "
				+ identity.id().toString());
	}

	return m_sessionManager->open(user);
}

const string AuthService::login(const Credentials &cred)
{
	TRACE_METHOD();

	Providers::iterator it = m_providers.find(cred.provider());
	if (it == m_providers.end())
		throw NotAuthenticatedException("no such provider");

	AuthProvider *provider = it->second;
	AuthResult result;

	if (!provider->authorize(cred, result))
		throw NotAuthenticatedException("authorization failed");

	if (result.email().empty())
		throw NotAuthenticatedException("invalid result of authorization");

	VerifiedIdentity identity;
	if (!m_verifiedIdentityDao->fetchBy(identity,
				result.email(), result.provider()))
		return "";

	return openSession(identity);
}

void AuthService::logout(const std::string &id)
{
	TRACE_METHOD();
	m_sessionManager->close(id);
}
