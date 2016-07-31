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
		textInjector("nextRandom", (TextSetter)
				&MockRandomProvider::setNextRandom);
	}

	void setNextRandom(const std::string &next)
	{
		m_nextRandom = next;
	}

	std::string randomStringUnlocked(unsigned int length)
	{
		if (length != m_nextRandom.size()) {
			throw Poco::AssertionViolationException(
				"request length does not match the next random string length");
		}

		return m_nextRandom;
	}

private:
	std::string m_nextRandom;
};

}

#endif
