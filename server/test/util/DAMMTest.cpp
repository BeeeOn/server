#include <cppunit/extensions/HelperMacros.h>

#include <map>
#include "util/DAMM.h"

using namespace std;

namespace BeeeOn {

class DAMMTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(DAMMTest);
	CPPUNIT_TEST(testSingle);
	CPPUNIT_TEST(testCompute);
	CPPUNIT_TEST_SUITE_END();

public:
	DAMMTest()
	{
	}

	void setUp();
	void tearDown();
	void testSingle();
	void testCompute();

private:
	map<string, int> m_expect;
};

CPPUNIT_TEST_SUITE_REGISTRATION(DAMMTest);

void DAMMTest::setUp()
{
	// from adapter factory code
	m_expect["161762224815122"] = 3;
	m_expect["181462068183316"] = 8;
	m_expect["140548523101825"] = 4;
	m_expect["144426044538750"] = 4;
	m_expect["111007672057235"] = 7;
	m_expect["116084064167771"] = 9;
	m_expect["118453478507226"] = 1;
	// en.wikibooks.org/wiki/Algorithm_Implementation/Checksums/Damm_Algorithm
	m_expect["43881234567"] = 9;
	m_expect["572"] = 4;
}

void DAMMTest::tearDown()
{
}

/**
 * Test against a single direct value.
 */
void DAMMTest::testSingle()
{
	int result = DAMM::compute("113664050215770");
	CPPUNIT_ASSERT_EQUAL(3, result);
}

/**
 * More complex test over a set of precomputed values.
 */
void DAMMTest::testCompute()
{
	map<string, int>::const_iterator it;

	for (it = m_expect.begin(); it != m_expect.end(); ++it)
		CPPUNIT_ASSERT_EQUAL(it->second, DAMM::compute(it->first));
}

}
