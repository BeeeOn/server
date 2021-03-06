#pragma once

#include <Poco/Net/NetException.h>

#include "server/Session.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * Abstract class providing access to sessions.
 */
class SessionVerifier : public Loggable {
public:
	SessionVerifier(const std::string &scheme = "Bearer");
	virtual ~SessionVerifier();

	void setScheme(const std::string &scheme);
	std::string scheme() const;

	/**
	 * Verify the session exists and return it.
	 */
	template <typename Request>
	Session::Ptr verifyAuthorized(const Request &request)
	{
		if (!request.hasCredentials()) {
			throw Poco::Net::NotAuthenticatedException(
					"missing credentials");
		}

		std::string scheme;
		std::string authInfo;
		request.getCredentials(scheme, authInfo);

		return verifyAuthorized(scheme, authInfo);
	}

	Session::Ptr verifyAuthorized(
			const std::string &scheme,
			const std::string &authInfo);

protected:
	virtual Session::Ptr doVerifyAuthorized(
			const std::string &authInfo) = 0;

private:
	std::string m_scheme;
};

}
