#ifndef BEEEON_POCO_RANDOM_PROVIDER_H
#define BEEEON_POCO_RANDOM_PROVIDER_H

#include "provider/RandomProvider.h"

namespace BeeeOn {

/**
 * A nonlinear additive feedback random provider
 * using 256 bytes of state information with a period of
 * up to 2^69. The seed is obtained from RandomInputStream
 * (aka /dev/urandom).
 *
 * @see Poco::Random for details.
 */
class PocoRandomProvider : public SecureRandomProvider {
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
	 * Generate a secure random byte array. Reseed first,
	 * if the last reseed occured in less then m_reseed_us
	 * microseconds. When m_reseed_us is zero, reseeding
	 * is performed on every call.
	 */
	void randomBytesUnlocked(char *b, unsigned int length)
	{
		if (m_lastSeed.isElapsed(m_reseed_us)) {
			m_random.seed();
			m_lastSeed.update();
		}

		secureRandomBytes(b, length);
	}

private:
	void secureRandomBytes(char *b, unsigned int length)
	{
		for (unsigned int i = 0; i < length; ++i)
			b[i] = m_random.nextChar();
	}

private:
	Poco::Random m_random;
	Poco::Timestamp m_lastSeed;
	Poco::Timestamp::TimeDiff m_reseed_us;
};

}

#endif
