#include <unistd.h>

#include <Poco/Environment.h>
#include <Poco/Exception.h>
#include <Poco/Logger.h>

#include "di/Injectable.h"
#include "util/MultiException.h"
#include "util/StartupSecurityChecker.h"

BEEEON_OBJECT_BEGIN(BeeeOn, StartupSecurityChecker)
BEEEON_OBJECT_PROPERTY("mode", &StartupSecurityChecker::setMode)
BEEEON_OBJECT_PROPERTY("rootAllowed", &StartupSecurityChecker::setRootAllowed)
BEEEON_OBJECT_PROPERTY("user", &StartupSecurityChecker::setUser)
BEEEON_OBJECT_PROPERTY("securePeerVerifiers", &StartupSecurityChecker::setSecurePeerVerifiers)
BEEEON_OBJECT_PROPERTY("peerVerifier", &StartupSecurityChecker::setPeerVerifier)
BEEEON_OBJECT_PROPERTY("secureGatewayLimiters", &StartupSecurityChecker::setSecureGatewayLimiters)
BEEEON_OBJECT_PROPERTY("gatewayLimiter", &StartupSecurityChecker::setGatewayLimiter)
BEEEON_OBJECT_HOOK("done", &StartupSecurityChecker::check)
BEEEON_OBJECT_END(BeeeOn, StartupSecurityChecker)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

StartupSecurityChecker::StartupSecurityChecker():
	m_mode(MODE_FAIL),
	m_rootAllowed(false)
{
}

void StartupSecurityChecker::setMode(const string &mode)
{
	if (mode == "fail")
		m_mode = MODE_FAIL;
	else if (mode == "laud")
		m_mode = MODE_LAUD;
	else if (mode == "ignore")
		m_mode = MODE_IGNORE;
	else
		throw InvalidArgumentException("unrecognized mode: " + mode);
}

void StartupSecurityChecker::setRootAllowed(bool allowed)
{
	m_rootAllowed = allowed;
}

void StartupSecurityChecker::setUser(const string &user)
{
	m_user = user;
}

void StartupSecurityChecker::setSecurePeerVerifiers(const list<string> &verifiers)
{
	for (const auto &name : verifiers)
		m_securePeerVerifiers.emplace(name);
}

void StartupSecurityChecker::setPeerVerifier(const string &verifier)
{
	m_peerVerifier = verifier;
}

void StartupSecurityChecker::setSecureGatewayLimiters(const list<string> &limiters)
{
	for (const auto &name : limiters)
		m_secureGatewayLimiters.emplace(name);
}

void StartupSecurityChecker::setGatewayLimiter(const string &limiter)
{
	m_gatewayLimiter = limiter;
}

void StartupSecurityChecker::checkRootAllowed() const
{
	if (m_rootAllowed)
		return;

	const auto uid = ::getuid();
	if (uid == 0) {
		throw IllegalStateException(
			"UID 0 (root) is insecure");
	}

	const auto euid = ::geteuid();
	if (euid == 0) {
		throw IllegalStateException(
			"EUID 0 (root) is insecure");
	}

	logger().information(
		"UID " + to_string(uid) + ", EUID " + to_string(euid),
		__FILE__, __LINE__);
}

void StartupSecurityChecker::checkUser() const
{
	if (m_user.empty())
		return;

	const auto &current = Environment::get("USER", "");
	if (current != m_user) {
		throw IllegalStateException(
			"expected user '" + m_user
			+ "', but got '" + current + "'");
	}
}

void StartupSecurityChecker::checkPeerVerifier() const
{
	if (m_peerVerifier.empty())
		return;

	const auto it = m_securePeerVerifiers.find(m_peerVerifier);
	if (it == m_securePeerVerifiers.end()) {
		string secure;

		for (const auto &one : m_securePeerVerifiers) {
			if (secure.empty())
				secure = "'" + one + "'";
			else
				secure += ", '" + one + "'";
		}

		throw IllegalStateException(
			"peer verifier '" + m_peerVerifier
			+ "' is considered to be insecure"
			+ " try any of: " + secure);
	}
}

void StartupSecurityChecker::checkGatewayLimiter() const
{
	if (m_gatewayLimiter.empty())
		return;

	const auto it = m_secureGatewayLimiters.find(m_gatewayLimiter);
	if (it == m_secureGatewayLimiters.end()) {
		string secure;

		for (const auto &one : m_secureGatewayLimiters) {
			if (secure.empty())
				secure = "'" + one + "'";
			else
				secure += ", '" + one + "'";
		}

		throw IllegalStateException(
			"gateway limiter '" + m_gatewayLimiter
			+ "' is considered to be insecure"
			+ " try any of: " + secure);
	}
}

void StartupSecurityChecker::check()
{
	if (m_mode == MODE_IGNORE)
		return;

	MultiException issues;

	try {
		checkRootAllowed();
	}
	catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
		issues.caught(e);
	}

	try {
		checkUser();
	}
	catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
		issues.caught(e);
	}

	try {
		checkPeerVerifier();
	}
	catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
		issues.caught(e);
	}

	try {
		checkGatewayLimiter();
	}
	catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
		issues.caught(e);
	}

	if (m_mode == MODE_LAUD)
		logger().log(issues, __FILE__, __LINE__);
	else if (m_mode == MODE_FAIL)
		issues.rethrow();
}
