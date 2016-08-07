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
	Credentials(const std::string &scheme,
			const std::string authInfo,
			const std::map<std::string, std::string> data):
		m_scheme(scheme),
		m_authInfo(authInfo),
		m_data(data)
	{
	}

	const std::string &scheme() const
	{
		return m_scheme;
	}

	const std::string &authInfo() const
	{
		return m_authInfo;
	}

	const std::string &get(const std::string &key) const
	{
		std::map<std::string, std::string>::const_iterator it;
		it = m_data.find(key);
		return it->second;
	}

	bool has(const std::string &key) const
	{
		return m_data.find(key) != m_data.end();
	}

private:
	const std::string m_scheme;
	const std::string m_authInfo;
	const std::map<std::string, std::string> m_data;
};

/**
 * An interface to an authorization provider.
 * An AuthProvider has always a name to select it from a set
 * other providers.
 */
class AuthProvider {
public:
	typedef std::map<std::string, std::string> Result;

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
		const Credentials &cred, Result &result) = 0;

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
 * A token authorization provider performs authorization
 * by using a token. The abstract class extracts a token
 * and verifies it against a 3rd party authorization service.
 */
class TokenAuthProvider : public AbstractAuthProvider {
public:
	TokenAuthProvider(const std::string &name):
		AbstractAuthProvider(name, LOGGER_CLASS(this))
	{
	}

	bool authorize(const Credentials &cred, Result &result)
	{
		_TRACE_METHOD(m_logger);

		if (!cred.has("token"))
			return false;

		const std::string &token(cred.get("token"));
		return verifyToken(token, result);
	}

protected:
	/**
	 * Verification against a 3rd party.
	 */
	virtual bool verifyToken(const std::string &token,
			Result &info) = 0;
};

}

#endif
