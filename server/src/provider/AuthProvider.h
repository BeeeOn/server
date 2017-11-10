#ifndef BEEEON_AUTH_PROVIDER_H
#define BEEEON_AUTH_PROVIDER_H

#include <map>
#include <Poco/URI.h>

#include "util/Loggable.h"

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
	Credentials(const std::string &provider);

	const std::string &provider() const;

private:
	const std::string m_provider;
};

class AuthCodeCredentials : public Credentials {
public:
	AuthCodeCredentials(const std::string &provider,
			const std::string &authCode);

	const std::string &authCode() const;

private:
	const std::string m_authCode;
};

class AuthResult {
public:
	AuthResult();

	void setEmail(const std::string &email);
	std::string email() const;

	void setProvider(const std::string &provider);
	std::string provider() const;

	void setAccessToken(const std::string &accessToken);
	std::string accessToken() const;

	void setProviderID(const std::string &providerID);
	std::string providerID() const;

	void setFirstName(const std::string &firstName);
	std::string firstName() const;

	void setLastName(const std::string &lastName);
	std::string lastName() const;

	void setPicture(const Poco::URI &picture);
	const Poco::URI &picture() const;

	void setLocale(const std::string &locale);
	std::string locale() const;

private:
	std::string m_email;
	std::string m_provider;
	std::string m_accessToken;
	std::string m_providerID;
	std::string m_firstName;
	std::string m_lastName;
	Poco::URI   m_picture;
	std::string m_locale;
};

/**
 * An interface to an authorization provider.
 * An AuthProvider has always a name to select it from a set
 * other providers.
 */
class AuthProvider {
public:
	virtual ~AuthProvider();

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
		public Loggable {
public:
	AbstractAuthProvider(const std::string &name);

	const std::string &name() const;

protected:
	const std::string m_name;
};

/**
 * An auth code authorization provider performs authorization
 * by using an auth code. The abstract class extracts the auth
 * code and verifies it against a 3rd party authorization service.
 */
class AuthCodeAuthProvider : public AbstractAuthProvider {
public:
	AuthCodeAuthProvider(const std::string &name);

	bool authorize(const Credentials &cred, AuthResult &result);

protected:
	/**
	 * Verification against a 3rd party.
	 */
	virtual bool verifyAuthCode(const std::string &authCode,
			AuthResult &info) = 0;
};

}

#endif
