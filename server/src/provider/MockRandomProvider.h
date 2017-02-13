#ifndef BEEEON_MOCK_RANDOM_PROVIDER_H
#define BEEEON_MOCK_RANDOM_PROVIDER_H

#include <Poco/Exception.h>
#include "provider/RandomProvider.h"

namespace BeeeOn {

class MockRandomProvider : public RandomProvider,
		public AbstractInjectorTarget {
public:
	MockRandomProvider()
	{
		textInjector("nextRandom", &MockRandomProvider::setNextRandom);
	}

	void setNextRandom(const std::string &next)
	{
		m_nextRandom = next;
	}

	void randomBytesUnlocked(char *b, unsigned int length)
	{
		if (length != m_nextRandom.size()) {
			throw Poco::AssertionViolationException(
				"request length does not match the next random string length");
		}

		std::string::const_iterator it = m_nextRandom.begin();
		for (int i = 0; it != m_nextRandom.end(); ++it, ++i)
			b[i] = *it;
	}

private:
	std::string m_nextRandom;
};

}

#endif
