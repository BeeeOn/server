#ifndef BEEEON_RANDOM_PROVIDER_H
#define BEEEON_RANDOM_PROVIDER_H

#include <Poco/Logger.h>
#include <Poco/Random.h>
#include <Poco/Timestamp.h>
#include <Poco/Mutex.h>

#include "di/AbstractInjectorTarget.h"
#include "Debug.h"

namespace BeeeOn {

/**
 * Provider of random values.
 */
class RandomProvider {
public:
	/**
	 * Return a buffer of random bytes of the given length.
	 * The quality of the randomness depends on implementation.
	 * The function is not thread-safe.
	 */
	virtual void randomBytesUnlocked(char *b, unsigned int length) = 0;

	void randomBytes(char *b, unsigned int length)
	{
		Poco::Mutex::ScopedLock guard(m_lock);
		return randomBytesUnlocked(b, length);
	}

private:
	Poco::Mutex m_lock;
};

/**
 * Provider of random values that are trusted to be secure.
 */
class SecureRandomProvider : public RandomProvider {
};

/**
 * Any random number provider can be used as a SecureRandomProvider.
 * This is intended for testing only. Never run an InsecureRandomProvider
 * in the production environment!
 */
class InsecureRandomProvider : public SecureRandomProvider,
		public AbstractInjectorTarget {
public:
	InsecureRandomProvider()
	{
		LOGGER_CLASS(this)
			.critical("AN INSECURE RANDOM PROVIDER IN USE",
					__FILE__, __LINE__);

		injector("providerImpl", &InsecureRandomProvider::setProviderImpl);
	}

	void setProviderImpl(RandomProvider *provider)
	{
		m_provider = provider;
	}

	void randomBytesUnlocked(char *b, unsigned int length)
	{
		m_provider->randomBytesUnlocked(b, length);
	}

private:
	RandomProvider *m_provider;
};

}

#endif
