#ifndef BEEEON_POCO_RANDOM_PROVIDER_H
#define BEEEON_POCO_RANDOM_PROVIDER_H

#include "provider/RandomProvider.h"
#include "di/InjectorTarget.h"

namespace BeeeOn {

/**
 * A nonlinear additive feedback random provider
 * using 256 bytes of state information with a period of
 * up to 2^69. The seed is obtained from RandomInputStream
 * (aka /dev/urandom).
 *
 * @see Poco::Random for details.
 */
class PocoRandomProvider : public SecureRandomProvider,
		public AbstractInjectorTarget {
public:
	PocoRandomProvider():
		m_reseed_us(0)
	{
		m_random.seed();
	}

	PocoRandomProvider(const Poco::Timestamp::TimeDiff &reseed_us):
		m_reseed_us(reseed_us)
	{
		m_random.seed();
	}

	void setReseedPeriod(int reseed)
	{
		if (reseed < 0) {
			throw Poco::InvalidArgumentException(
				"reseed timeout must not be negative");
		}

		m_reseed_us = reseed;
	}

	/**
	 * Generate a secure random string. Reseed first,
	 * if the last reseed occured in les then m_reseed_us
	 * microseconds. When m_reseed_us is zero, reseeding
	 * is performed on every call.
	 */
	std::string randomStringUnlocked(unsigned int length)
	{
		std::string value(length, '\0');

		if (m_lastSeed.isElapsed(m_reseed_us)) {
			m_random.seed();
			m_lastSeed.update();
		}

		secureRandomString(value);
		return value;
	}

private:
	void secureRandomString(std::string &s)
	{
		for (unsigned int i = 0; i < s.size(); ++i)
			s[i] = m_random.nextChar();
	}

private:
	Poco::Random m_random;
	Poco::Timestamp m_lastSeed;
	Poco::Timestamp::TimeDiff m_reseed_us;
};

}

#endif
