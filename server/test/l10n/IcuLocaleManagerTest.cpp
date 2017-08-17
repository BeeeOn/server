#include <cstring>

#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Exception.h>

#include <unicode/putil.h>

#include "cppunit/BetterAssert.h"
#include "l10n/IcuLocaleManager.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class IcuLocaleManagerTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(IcuLocaleManagerTest);
	CPPUNIT_TEST(testParse);
	CPPUNIT_TEST(testNonExistingDefaultLocale);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();

	void testNonExistingDefaultLocale();
	void testParse();

private:
	IcuLocaleManager m_manager;
};

CPPUNIT_TEST_SUITE_REGISTRATION(IcuLocaleManagerTest);

void IcuLocaleManagerTest::setUp()
{
	m_manager.setBundleName("test");

	if (!strcmp(u_getDataDirectory(), ""))
		m_manager.setResourceDir("test/l10n");

	m_manager.setDefaultLocale("en");
	m_manager.setup();
}

void IcuLocaleManagerTest::testNonExistingDefaultLocale()
{
	m_manager.setDefaultLocale("xxx");

	CPPUNIT_ASSERT_THROW(
		m_manager.setup(),
		IllegalStateException
	);
}

void IcuLocaleManagerTest::testParse()
{
	Locale locale;

	// exact match
	CPPUNIT_ASSERT_EQUAL("en", m_manager.parse("en").toString());
	CPPUNIT_ASSERT_EQUAL("cs", m_manager.parse("cs").toString());

	// root locale cannot be accessed directly, default is used
	CPPUNIT_ASSERT_EQUAL("en", m_manager.parse("").toString());

	// best match
	CPPUNIT_ASSERT_EQUAL("en", m_manager.parse("en_US").toString());
	CPPUNIT_ASSERT_EQUAL("en", m_manager.parse("en_GB").toString());
	CPPUNIT_ASSERT_EQUAL("cs", m_manager.parse("cs_CZ").toString());

	// default locale
	CPPUNIT_ASSERT_EQUAL("en", m_manager.parse("non-existing").toString());
}

}
