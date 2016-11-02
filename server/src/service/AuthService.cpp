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
	identity.setUser(createUser(result));

	m_identityDao->create(identity);

	VerifiedIdentity verifiedIdentity;

	if (!verifyIdentity(verifiedIdentity, identity, result))
		return NULL;

	m_notificationService->notifyFirstLogin(verifiedIdentity);

	return openSession(verifiedIdentity);
}

bool AuthService::createUserAndVerify(
		VerifiedIdentity &verifiedIdentity,
		Identity &identity,
		const AuthResult &result)
{
	if (!identity.hasUser()) {
		User user(createUser(result));
		identity.setUser(user);

		if (!m_identityDao->update(identity))
			return false;
	}

	approveIdentity(verifiedIdentity, identity, result);
	return true;
}

void AuthService::approveIdentity(
		VerifiedIdentity &verifiedIdentity,
		const Identity &identity,
		const AuthResult &result)
{
	verifiedIdentity.setIdentity(identity);
	verifiedIdentity.setProvider(result.provider());
	verifiedIdentity.setAccessToken(result.accessToken());
	verifiedIdentity.setPicture(URI(result.picture()));

	m_verifiedIdentityDao->create(verifiedIdentity);
}

bool AuthService::verifyIdentity(
		VerifiedIdentity &verifiedIdentity,
		Identity &identity,
		const AuthResult &result)
{
	vector<VerifiedIdentity> identities;
	m_verifiedIdentityDao->fetchBy(identities, identity.email());

	if (identities.empty())
		return createUserAndVerify(verifiedIdentity, identity, result);

	const VerifiedIdentity &existing = identities.front();
	approveIdentity(verifiedIdentity, existing.identity(), result);
	return true;
}

ExpirableSession::Ptr AuthService::verifyIdentityAndLogin(
		const AuthResult &result)
{
	Identity identity;

	if (!m_identityDao->fetchBy(identity, result.email()))
		return loginAsNew(result);

	VerifiedIdentity verifiedIdentity;

	if (!verifyIdentity(verifiedIdentity, identity, result))
		return NULL;

	m_notificationService->notifyFirstLogin(verifiedIdentity);

	return openSession(verifiedIdentity);
}

ExpirableSession::Ptr AuthService::openSession(
		const VerifiedIdentity &verifiedIdentity)
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

	VerifiedIdentity copy(verifiedIdentity);
	copy.setIdentity(identity);
	copy.setUser(user);

	return m_sessionManager->open(copy);
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

	VerifiedIdentity identity;
	if (!m_verifiedIdentityDao->fetchBy(identity,
				result.email(), result.provider()))
		return verifyIdentityAndLogin(result);

	return openSession(identity);
}

void AuthService::logout(const std::string &id)
{
	TRACE_METHOD();
	m_sessionManager->close(id);
}
