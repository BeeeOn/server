#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Exception.h>

#include "cppunit/BetterAssert.h"
#include "sqitch/SqitchRef.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class SqitchRefTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(SqitchRefTest);
	CPPUNIT_TEST(testName);
	CPPUNIT_TEST(testNameTag);
	CPPUNIT_TEST(testProjectName);
	CPPUNIT_TEST(testProjectNameTag);
	CPPUNIT_TEST(testEquals);
	CPPUNIT_TEST_SUITE_END();

public:
	void testName();
	void testNameTag();
	void testProjectName();
	void testProjectNameTag();
	void testEquals();
};

CPPUNIT_TEST_SUITE_REGISTRATION(SqitchRefTest);

void SqitchRefTest::testName()
{
	SqitchRef ref("any");

	CPPUNIT_ASSERT(ref.project().empty());
	CPPUNIT_ASSERT_EQUAL("any", ref.name());
	CPPUNIT_ASSERT(ref.tag().empty());
}

void SqitchRefTest::testNameTag()
{
	SqitchRef ref("any@foo");

	CPPUNIT_ASSERT(ref.project().empty());
	CPPUNIT_ASSERT_EQUAL("any", ref.name());
	CPPUNIT_ASSERT_EQUAL("foo", ref.tag());
}

void SqitchRefTest::testProjectName()
{
	SqitchRef ref("xxx:any");

	CPPUNIT_ASSERT_EQUAL("xxx", ref.project());
	CPPUNIT_ASSERT_EQUAL("any", ref.name());
	CPPUNIT_ASSERT(ref.tag().empty());
}

void SqitchRefTest::testProjectNameTag()
{
	SqitchRef ref("xxx:any@foo");

	CPPUNIT_ASSERT_EQUAL("xxx", ref.project());
	CPPUNIT_ASSERT_EQUAL("any", ref.name());
	CPPUNIT_ASSERT_EQUAL("foo", ref.tag());
}

void SqitchRefTest::testEquals()
{
	SqitchRef a0("a0");
	SqitchRef b0("b0");
	SqitchRef c0("a0");

	CPPUNIT_ASSERT(a0.equals(a0));
	CPPUNIT_ASSERT(!a0.equals(b0));
	CPPUNIT_ASSERT(a0.equals(c0));

	SqitchRef a1("project:a1");
	SqitchRef b1("a1");
	SqitchRef c1("xxx:a1");

	CPPUNIT_ASSERT(a1.equals(a1));
	// derive b1's project from context of a1
	CPPUNIT_ASSERT(a1.equals(b1));
	CPPUNIT_ASSERT(!a1.equals(c1));
	// derive b1's project from context of c1
	CPPUNIT_ASSERT(b1.equals(c1));

	SqitchRef a2("a2@foo");
	SqitchRef b2("a2");
	SqitchRef c2("a2@bar");

	CPPUNIT_ASSERT(a2.equals(a2));
	// there is no way how to derive the tag context
	CPPUNIT_ASSERT(!a2.equals(b2));
	CPPUNIT_ASSERT(!a2.equals(c2));
}

}
