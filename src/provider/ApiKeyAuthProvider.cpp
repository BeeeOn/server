#include <Poco/Exception.h>
#include <Poco/File.h>
#include <Poco/Logger.h>
#include <Poco/Net/NetException.h>
#include <Poco/Util/PropertyFileConfiguration.h>

#include "di/Injectable.h"
#include "provider/ApiKeyAuthProvider.h"

BEEEON_OBJECT_BEGIN(BeeeOn, ApiKeyAuthProvider)
BEEEON_OBJECT_CASTABLE(AuthProvider)
BEEEON_OBJECT_PROPERTY("keyFile", &ApiKeyAuthProvider::setKeyFile)
BEEEON_OBJECT_PROPERTY("reloadPeriod", &ApiKeyAuthProvider::setReloadPeriod)
BEEEON_OBJECT_END(BeeeOn, ApiKeyAuthProvider)

using namespace std;
using namespace Poco;
using namespace Poco::Util;
using namespace Poco::Net;
using namespace BeeeOn;

ApiKeyAuthProvider::ApiKeyAuthProvider():
	AbstractAuthProvider("apikey"),
	m_keysFile("apikeys.properties"),
	m_reloadPeriod(1 * Timespan::MINUTES),
	m_lastSize(0)
{
}

void ApiKeyAuthProvider::setKeyFile(const string &path)
{
	const File file(path);

	if (!file.exists())
		throw InvalidArgumentException("path " + path + " does not exist");
	if (!file.isFile())
		throw InvalidArgumentException("path " + path + " is not a regular file");
	if (!file.canRead())
		throw InvalidArgumentException("file " + path + " is not readable");

	m_keysFile = path;
}

void ApiKeyAuthProvider::setReloadPeriod(const Timespan &period)
{
	if (period > 0 && period < 1 * Timespan::SECONDS)
		throw InvalidArgumentException("reloadPeriod must be at least 1 s");
	else
		m_reloadPeriod = period;
}

bool ApiKeyAuthProvider::shouldReload(const File &file) const
{
	if (file.getSize() != m_lastSize)
		return true;

	if (file.getLastModified() > m_lastReload)
		return true;

	return false;
}

void ApiKeyAuthProvider::reloadKeys(bool force)
{
	const File file(m_keysFile);

	if (!force) {
		const auto period = m_reloadPeriod.totalMicroseconds();

		RWLock::ScopedReadLock guard(m_lock);

		// avoid write lock on every reloadKeys call
		if (period >= 0 && !m_lastReload.isElapsed(period))
			return;
	}

	try {
		RWLock::ScopedWriteLock guard(m_lock);

		if (!force && !shouldReload(file))
			return;

		AutoPtr<PropertyFileConfiguration> keys = new PropertyFileConfiguration;
		keys->load(file.path());

		m_keys = keys;
		m_lastReload.update();
		m_lastSize = file.getSize();

		logger().information(
			"keyFile " + file.path() + " has been reloaded",
			__FILE__, __LINE__);
	}
	BEEEON_CATCH_CHAIN(logger())
}

bool ApiKeyAuthProvider::authorize(
		const Credentials &cred,
		AuthResult &result)
{
	if (typeid(cred) != typeid(ApiKeyCredentials))
		throw NotAuthenticatedException("unrecognized credentials type");

	const ApiKeyCredentials &api =
		static_cast<const ApiKeyCredentials &>(cred);
	const auto &key = api.key();

	reloadKeys(m_keys.isNull());

	RWLock::ScopedReadLock guard(m_lock);

	if (!m_keys->has(key.toString())) {
		if (logger().debug()) {
			logger().debug("no such API key " + key.toString(),
					__FILE__, __LINE__);
		}

		return false;
	}

	const auto &email = m_keys->getString(key.toString());
	result.setEmail(email);
	result.setProvider(name());

	AutoPtr<AbstractConfiguration> user = m_keys->createView(key.toString());

	if (user->has("first_name"))
		result.setFirstName(user->getString("first_name"));

	if (user->has("last_name"))
		result.setLastName(user->getString("last_name"));

	if (user->has("picture"))
		result.setPicture(URI(user->getString("picture")));

	if (user->has("locale"))
		result.setLocale(user->getString("locale"));

	return true;
}
