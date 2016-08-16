#include <Poco/Net/NetException.h>
#include "service/AuthService.h"

using namespace std;
using namespace Poco::Net;
using namespace BeeeOn;

BEEEON_OBJECT(AuthService, BeeeOn::AuthService)

const string AuthService::login(const Credentials &cred)
{
	TRACE_METHOD();

	Providers::iterator it = m_providers.find(cred.provider());
	if (it == m_providers.end())
		throw NotAuthenticatedException("no such provider");

	AuthProvider *provider = it->second;
	AuthProvider::Result result;

	if (!provider->authorize(cred, result))
		throw NotAuthenticatedException("authorization failed");

	AuthProvider::Result::const_iterator resultIt = result.find("email");
	if (resultIt == result.end())
		throw NotAuthenticatedException("invalid result of authorization");

	User user;
	user.setEmail(resultIt->second);
	if (!m_userDao->fetch(user))
		throw NotAuthenticatedException("unknown user e-mail");

	return m_sessionManager->open(user);
}

void AuthService::logout(const std::string &id)
{
	TRACE_METHOD();
	m_sessionManager->close(id);
}
