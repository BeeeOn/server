#include <cstring>

#include <cppunit/extensions/HelperMacros.h>

#include "cppunit/BetterAssert.h"
#include "l10n/IcuLocaleImpl.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class IcuLocaleImplTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(IcuLocaleImplTest);
	CPPUNIT_TEST(testCreate);
	CPPUNIT_TEST(testParse);
	CPPUNIT_TEST(testDisplayName);
	CPPUNIT_TEST(testAvailable);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();

	void testCreate();
	void testParse();
	void testDisplayName();
	void testAvailable();
};

CPPUNIT_TEST_SUITE_REGISTRATION(IcuLocaleImplTest);

void IcuLocaleImplTest::setUp()
{
	// Set environment variable ICU_DATA or assume we are
	// running tests from the root of the project.
	//
	// The res_index.res is expected to be located at
	//
	//   test/l10n/test/res_index.res
	//
	if (!strcmp(u_getDataDirectory(), ""))
		u_setDataDirectory("test/l10n");
}

void IcuLocaleImplTest::testCreate()
{
	IcuLocaleImpl root;
	CPPUNIT_ASSERT_EQUAL("", root.language());
	CPPUNIT_ASSERT_EQUAL("", root.country());
	CPPUNIT_ASSERT_EQUAL("", root.toString());

	IcuLocaleImpl en_US("en", "US");
	CPPUNIT_ASSERT_EQUAL("en", en_US.language());
	CPPUNIT_ASSERT_EQUAL("US", en_US.country());
	CPPUNIT_ASSERT_EQUAL("en_US", en_US.toString());

	IcuLocaleImpl en_GB("en", "GB");
	CPPUNIT_ASSERT_EQUAL("en", en_GB.language());
	CPPUNIT_ASSERT_EQUAL("GB", en_GB.country());
	CPPUNIT_ASSERT_EQUAL("en_GB", en_GB.toString());

	IcuLocaleImpl cs_CZ("cs", "CZ");
	CPPUNIT_ASSERT_EQUAL("cs", cs_CZ.language());
	CPPUNIT_ASSERT_EQUAL("CZ", cs_CZ.country());
	CPPUNIT_ASSERT_EQUAL("cs_CZ", cs_CZ.toString());
}

void IcuLocaleImplTest::testParse()
{
	CPPUNIT_ASSERT_EQUAL("en_US", IcuLocaleImpl::parse("en_US").toString());
	CPPUNIT_ASSERT_EQUAL("en", IcuLocaleImpl::parse("en").toString());
	CPPUNIT_ASSERT_EQUAL("cs", IcuLocaleImpl::parse("cs").toString());
	CPPUNIT_ASSERT_EQUAL("_CZ", IcuLocaleImpl::parse("_CZ").toString());
	CPPUNIT_ASSERT_EQUAL("czech", IcuLocaleImpl::parse("czech").toString());
	CPPUNIT_ASSERT_EQUAL("garbage", IcuLocaleImpl::parse("garbage").toString());
}

void IcuLocaleImplTest::testDisplayName()
{
	CPPUNIT_ASSERT_EQUAL("English (United States)",
			IcuLocaleImpl::parse("en_US").displayName());
	CPPUNIT_ASSERT_EQUAL("English (United Kingdom)",
			IcuLocaleImpl::parse("en_GB").displayName());

	const string cr = IcuLocaleImpl::parse("cs_CZ").displayName();
	CPPUNIT_ASSERT(cr == "čeština (Česko)" || cr == "čeština (Česká republika)");

	CPPUNIT_ASSERT_EQUAL("garbage",
			IcuLocaleImpl::parse("garbage").displayName());
}

void IcuLocaleImplTest::testAvailable()
{
	list<IcuLocaleImpl> locales;

	// no non-existing/res_index.res resource found
	CPPUNIT_ASSERT_THROW(
		IcuLocaleImpl::available("non-existing", locales),
		NotFoundException
	);

	IcuLocaleImpl::available("test", locales);
	// en, cs (root is avoided)
	CPPUNIT_ASSERT_EQUAL(2, locales.size());
}

}
