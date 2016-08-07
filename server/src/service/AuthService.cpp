#include <Poco/Net/NetException.h>
#include "service/AuthService.h"

using namespace std;
using namespace Poco::Net;
using namespace BeeeOn;

BEEEON_OBJECT(AuthService, BeeeOn::AuthService)

const string AuthService::login(const Credentials &cred)
{
	TRACE_METHOD();

	if (!cred.has("provider"))
		throw NotAuthenticatedException("no provider specified");

	const string providerName = cred.get("provider");

	Providers::iterator it = m_providers.find(providerName);
	if (it == m_providers.end())
		throw NotAuthenticatedException("no such provider");

	AuthProvider *provider = it->second;
	AuthProvider::Result result;

	if (!provider->authorize(cred, result))
		throw NotAuthenticatedException("authorization failed");

	AuthProvider::Result::const_iterator resultIt = result.find("email");
	if (resultIt == result.end())
		throw NotAuthenticatedException("invalid result of authorization");

	const string &email = resultIt->second;
	User::Ptr user = m_userDao->getByEmail(email);

	if (user.isNull())
		throw NotAuthenticatedException("unknown user e-mail");

	SessionManager::Info info;
	info.insert(make_pair(SessionManager::INFO_EMAIL, email));
	info.insert(make_pair(SessionManager::INFO_USER_ID,
				to_string(user->id())));
	return m_sessionManager->open(info);
}

void AuthService::logout(const std::string &id)
{
	TRACE_METHOD();
	m_sessionManager->close(id);
}
