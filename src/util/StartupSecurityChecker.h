#pragma once

#include <list>
#include <set>
#include <string>

#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief StartupSecurityChecker checks for obvious settings
 * where it is easy to forget about security issues. Currently,
 * it takes care of:
 *
 * - current system user
 * - used gateway verifier
 * - used gateway rate limiter
 */
class StartupSecurityChecker : Loggable {
public:
	/**
	 * @brief Distinguish whether the check should
	 * fail and thus halt the application or whether
	 * it just informs (useful for debugging and corner
	 * situations).
	 */
	enum Mode {
		MODE_FAIL,   /**< if anything is wrong, fail */
		MODE_LAUD,   /**< if anything is wrong, be laud in log */
		MODE_IGNORE, /**< ignore any checks */
	};

	StartupSecurityChecker();

	void setMode(const std::string &mode);
	void setRootAllowed(bool allowed);
	void setUser(const std::string &user);
	void setSecurePeerVerifiers(const std::list<std::string> &verifiers);
	void setPeerVerifier(const std::string &verifier);
	void setSecureGatewayLimiters(const std::list<std::string> &limiters);
	void setGatewayLimiter(const std::string &limiter);

	/**
	 * @brief Perform all checks according to the mode.
	 */
	void check();

protected:
	void checkRootAllowed() const;
	void checkUser() const;
	void checkPeerVerifier() const;
	void checkGatewayLimiter() const;

private:
	Mode m_mode;
	bool m_rootAllowed;
	std::string m_user;
	std::set<std::string> m_securePeerVerifiers;
	std::string m_peerVerifier;
	std::set<std::string> m_secureGatewayLimiters;
	std::string m_gatewayLimiter;
};

}
