#ifndef BEEEON_RANDOM_PROVIDER_H
#define BEEEON_RANDOM_PROVIDER_H

#include <Poco/Random.h>
#include <Poco/Timestamp.h>
#include <Poco/Mutex.h>

namespace BeeeOn {

/**
 * Provider of random values.
 */
class RandomProvider {
public:
	/**
	 * Return a random string of the given length.
	 * The quality of the randomness depends on implementation.
	 * The function is not thread-safe.
	 */
	virtual std::string randomStringUnlocked(unsigned int length) = 0;

	std::string randomString(unsigned int length)
	{
		Poco::Mutex::ScopedLock guard(m_lock);
		return randomStringUnlocked(length);
	}

private:
	Poco::Mutex m_lock;
};

/**
 * Provider of random values that are trusted to be secure.
 */
class SecureRandomProvider : public RandomProvider {
};

}

#endif
