#ifndef BEEEON_AUTH_PROVIDER_H
#define BEEEON_AUTH_PROVIDER_H

#include <map>
#include <Poco/Logger.h>
#include "di/InjectorTarget.h"
#include "Debug.h"

namespace BeeeOn {

/**
 * Representation of credentials to the application.
 * It is generic enough to work with many authorization
 * schemes.
 */
class Credentials {
public:
	/**
	 * Create Credentials from the scheme, authInfo and
	 * a map of other data specific to the scheme.
	 * The scheme and authInfo are usually extracted
	 * from a HTTP Request header Authorization.
	 */
	Credentials(const std::string &provider):
		m_provider(provider)
	{
	}

	const std::string &provider() const
	{
		return m_provider;
	}

private:
	const std::string m_provider;
};

class AuthCodeCredentials : public Credentials {
public:
	AuthCodeCredentials(const std::string &provider,
			const std::string &authCode):
		Credentials(provider),
		m_authCode(authCode)
	{
	}

	const std::string &authCode() const
	{
		return m_authCode;
	}

private:
	const std::string m_authCode;
};

class AuthResult {
public:
	AuthResult()
	{
	}

	void setEmail(const std::string &email)
	{
		m_email = email;
	}

	std::string email() const
	{
		return m_email;
	}

	void setProvider(const std::string &provider)
	{
		m_provider = provider;
	}

	std::string provider() const
	{
		return m_provider;
	}

	void setAccessToken(const std::string &accessToken)
	{
		m_accessToken = accessToken;
	}

	std::string accessToken() const
	{
		return m_accessToken;
	}

	void setProviderID(const std::string &providerID)
	{
		m_providerID = providerID;
	}

	std::string providerID() const
	{
		return m_providerID;
	}

	void setFirstName(const std::string &firstName)
	{
		m_firstName = firstName;
	}

	std::string firstName() const
	{
		return m_firstName;
	}

	void setLastName(const std::string &lastName)
	{
		m_lastName = lastName;
	}

	std::string lastName() const
	{
		return m_lastName;
	}

	void setPicture(const std::string &picture)
	{
		m_picture = picture;
	}

	std::string picture() const
	{
		return m_picture;
	}

private:
	std::string m_email;
	std::string m_provider;
	std::string m_accessToken;
	std::string m_providerID;
	std::string m_firstName;
	std::string m_lastName;
	std::string m_picture;
};

/**
 * An interface to an authorization provider.
 * An AuthProvider has always a name to select it from a set
 * other providers.
 */
class AuthProvider {
public:
	virtual ~AuthProvider() {}

	/**
	 * Return name of the provider.
	 */
	virtual const std::string &name() const = 0;

	/**
	 * Return false is not successful.
	 * If true, a user details are returned in the result.
	 * An exception may be thrown in case of a failure
	 * (cannot perform the authorization operation itself).
	 */
	virtual bool authorize(
		const Credentials &cred, AuthResult &result) = 0;

};

class AbstractAuthProvider : public virtual AuthProvider,
		public AbstractInjectorTarget {
public:
	AbstractAuthProvider(const std::string &name,
			Poco::Logger &logger):
		m_name(name),
		m_logger(logger)
	{
	}

	const std::string &name() const
	{
		return m_name;
	}

protected:
	const std::string m_name;
	Poco::Logger &m_logger;
};

/**
 * An auth code authorization provider performs authorization
 * by using an auth code. The abstract class extracts the auth
 * code and verifies it against a 3rd party authorization service.
 */
class AuthCodeAuthProvider : public AbstractAuthProvider {
public:
	AuthCodeAuthProvider(const std::string &name):
		AbstractAuthProvider(name, LOGGER_CLASS(this))
	{
	}

	bool authorize(const Credentials &cred, AuthResult &result)
	{
		_TRACE_METHOD(m_logger);

		const AuthCodeCredentials &authCodeCredentials =
			reinterpret_cast<const AuthCodeCredentials &>(cred);

		return verifyAuthCode(authCodeCredentials.authCode(), result);
	}

protected:
	/**
	 * Verification against a 3rd party.
	 */
	virtual bool verifyAuthCode(const std::string &authCode,
			AuthResult &info) = 0;
};

}

#endif
