#ifndef BEEEON_SKIPPABLE_AUTO_REGISTER_SUITE_H
#define BEEEON_SKIPPABLE_AUTO_REGISTER_SUITE_H

#include <cppunit/extensions/AutoRegisterSuite.h>

namespace BeeeOn {

typedef bool (*SkipTestFunc)();

template <typename Type>
class SkippableAutoRegisterSuite {
public:
	SkippableAutoRegisterSuite(SkipTestFunc skip)
	{
		if (!skip())
			m_suite = new CppUnit::AutoRegisterSuite<Type>();
	}

	~SkippableAutoRegisterSuite()
	{
		delete m_suite;
	}

private:
	CppUnit::AutoRegisterSuite<Type> *m_suite;
};

#define CPPUNIT_TEST_SUITE_REGISTRATION_SKIPPABLE(Type, func)    \
	static BeeeOn::SkippableAutoRegisterSuite<Type>   \
		CPPUNIT_MAKE_UNIQUE_NAME(skippableAutoRegisterRegistry__)(func)

}

#endif
