#include <cmath>
#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Exception.h>
#include <Poco/SharedPtr.h>

#include "cppunit/BetterAssert.h"
#include "util/UnknownEvaluator.h"

using namespace std;

namespace BeeeOn {

class UnknownEvaluatorTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(UnknownEvaluatorTest);
	CPPUNIT_TEST(testFromRaw);
	CPPUNIT_TEST(testFromRawNotNormal);
	CPPUNIT_TEST(testToRaw);
	CPPUNIT_TEST(testToRawNotNormal);
	CPPUNIT_TEST_SUITE_END();
public:
	void testFromRaw();
	void testFromRawNotNormal();
	void testToRaw();
	void testToRawNotNormal();

private:
	UnknownEvaluator m_evaluator;
};

CPPUNIT_TEST_SUITE_REGISTRATION(UnknownEvaluatorTest);

void UnknownEvaluatorTest::testFromRaw()
{
	ModuleInfo bypass;
	bypass.setFromUnknown("${raw}");

	CPPUNIT_ASSERT_EQUAL(10, m_evaluator.fromRaw(bypass, 10));
	CPPUNIT_ASSERT_EQUAL(678.123, m_evaluator.fromRaw(bypass, 678.123));

	ModuleInfo someops;
	someops.setFromUnknown("3 * ${raw} + 129.6 / 2");

	CPPUNIT_ASSERT_EQUAL(66.3, m_evaluator.fromRaw(someops, 1));
	CPPUNIT_ASSERT_EQUAL(64.8, m_evaluator.fromRaw(someops, 0));

	ModuleInfo power;
	power.setFromUnknown("${raw} * ${raw}");

	CPPUNIT_ASSERT_EQUAL(9, m_evaluator.fromRaw(power, 3));
	CPPUNIT_ASSERT_EQUAL(0, m_evaluator.fromRaw(power, 0));
	CPPUNIT_ASSERT_EQUAL(4, m_evaluator.fromRaw(power, -2));
}

void UnknownEvaluatorTest::testFromRawNotNormal()
{
	ModuleInfo bypass;
	bypass.setFromUnknown("${raw}");

	CPPUNIT_ASSERT(std::isnan(m_evaluator.fromRaw(bypass, NAN)));
	CPPUNIT_ASSERT(std::isnan(m_evaluator.fromRaw(bypass, INFINITY)));

	ModuleInfo someops;
	someops.setFromUnknown("3 * ${raw} + 129.6 / 2");

	CPPUNIT_ASSERT(std::isnan(m_evaluator.fromRaw(someops, NAN)));
	CPPUNIT_ASSERT(std::isnan(m_evaluator.fromRaw(someops, INFINITY)));
}

void UnknownEvaluatorTest::testToRaw()
{
	ModuleInfo bypass;
	bypass.setToUnknown("${value}");

	CPPUNIT_ASSERT_EQUAL(10, m_evaluator.toRaw(bypass, 10));
	CPPUNIT_ASSERT_EQUAL(678.123, m_evaluator.toRaw(bypass, 678.123));

	ModuleInfo someops;
	someops.setToUnknown("3 * ${value} + 129.6 / 2");

	CPPUNIT_ASSERT_EQUAL(66.3, m_evaluator.toRaw(someops, 1));
	CPPUNIT_ASSERT_EQUAL(64.8, m_evaluator.toRaw(someops, 0));
}

void UnknownEvaluatorTest::testToRawNotNormal()
{
	ModuleInfo bypass;
	bypass.setToUnknown("${value}");

	CPPUNIT_ASSERT(std::isnan(m_evaluator.toRaw(bypass, NAN)));
	CPPUNIT_ASSERT(std::isnan(m_evaluator.toRaw(bypass, INFINITY)));

	ModuleInfo someops;
	someops.setToUnknown("3 * ${value} + 129.6 / 2");

	CPPUNIT_ASSERT(std::isnan(m_evaluator.toRaw(someops, NAN)));
	CPPUNIT_ASSERT(std::isnan(m_evaluator.toRaw(someops, INFINITY)));

	ModuleInfo power;
	power.setToUnknown("${value} * ${value}");

	CPPUNIT_ASSERT(std::isnan(m_evaluator.toRaw(power, NAN)));
	CPPUNIT_ASSERT(std::isnan(m_evaluator.toRaw(power, INFINITY)));

}

}
