#include <Poco/URI.h>
#include <Poco/Net/NetException.h>
#include "service/AuthService.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

BEEEON_OBJECT(AuthService, BeeeOn::AuthService)

User AuthService::createUser(const AuthResult &result)
{
	User user;
	user.setFirstName(result.firstName());
	user.setLastName(result.lastName());

	m_userDao->create(user);
	return user;
}

ExpirableSession::Ptr AuthService::loginAsNew(const AuthResult &result)
{
	Identity identity;
	identity.setEmail(result.email());

	m_identityDao->create(identity);

	VerifiedIdentity verifiedIdentity;
	verifyIdentity(verifiedIdentity, identity, result);

	m_notificationService->notifyFirstLogin(verifiedIdentity);

	return openSession(verifiedIdentity);
}

void AuthService::createUserAndVerify(
		VerifiedIdentity &verifiedIdentity,
		Identity &identity,
		const AuthResult &result)
{
	User user(createUser(result));

	approveIdentity(verifiedIdentity, identity, user, result);
}

void AuthService::approveIdentity(
		VerifiedIdentity &verifiedIdentity,
		const Identity &identity,
		const User &user,
		const AuthResult &result)
{
	verifiedIdentity.setIdentity(identity);
	verifiedIdentity.setUser(user);
	verifiedIdentity.setProvider(result.provider());
	verifiedIdentity.setAccessToken(result.accessToken());
	verifiedIdentity.setPicture(URI(result.picture()));

	m_verifiedIdentityDao->create(verifiedIdentity);
}

void AuthService::verifyIdentity(
		VerifiedIdentity &verifiedIdentity,
		Identity &identity,
		const AuthResult &result)
{
	vector<VerifiedIdentity> identities;
	m_verifiedIdentityDao->fetchBy(identities, identity.email());

	if (identities.empty()) {
		createUserAndVerify(verifiedIdentity, identity, result);
		return;
	}

	const VerifiedIdentity &existing = identities.front();
	approveIdentity(verifiedIdentity, identity, existing.user(), result);
}

ExpirableSession::Ptr AuthService::verifyIdentityAndLogin(
		const AuthResult &result)
{
	Identity identity;

	if (!m_identityDao->fetchBy(identity, result.email()))
		return loginAsNew(result);

	VerifiedIdentity verifiedIdentity;
	if (m_verifiedIdentityDao->fetchBy(verifiedIdentity,
				result.email(),
				result.provider()))
		return openSession(verifiedIdentity);

	verifyIdentity(verifiedIdentity, identity, result);

	m_notificationService->notifyFirstLogin(verifiedIdentity);

	return openSession(verifiedIdentity);
}

ExpirableSession::Ptr AuthService::openSession(
		const VerifiedIdentity &verifiedIdentity)
{
	User user(verifiedIdentity.user());

	if (!m_userDao->fetch(user)) {
		throw NotAuthenticatedException("unknown user for identity "
				+ verifiedIdentity.id().toString());
	}

	VerifiedIdentity copy(verifiedIdentity);
	copy.setUser(user);

	return m_sessionManager->open(copy);
}

ExpirableSession::Ptr AuthService::doLoginAuthorized(const AuthResult &result)
{
	VerifiedIdentity identity;
	if (!m_verifiedIdentityDao->fetchBy(identity,
				result.email(), result.provider()))
		return verifyIdentityAndLogin(result);

	return openSession(identity);
}

const ExpirableSession::Ptr AuthService::login(const Credentials &cred)
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

	return loginAuthorized(result);
}

void AuthService::logout(const std::string &id)
{
	TRACE_METHOD();
	m_sessionManager->close(id);
}
