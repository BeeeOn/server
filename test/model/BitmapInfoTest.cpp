#include <cppunit/extensions/HelperMacros.h>

#include "cppunit/BetterAssert.h"
#include "model/BitmapInfo.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class BitmapInfoTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(BitmapInfoTest);
	CPPUNIT_TEST(testFlagExtract);
	CPPUNIT_TEST(testGroupExtract);
	CPPUNIT_TEST(testGroupCompare);
	CPPUNIT_TEST(testBits);
	CPPUNIT_TEST_SUITE_END();

public:
	void testFlagExtract();
	void testGroupExtract();
	void testGroupCompare();
	void testBits();
};

CPPUNIT_TEST_SUITE_REGISTRATION(BitmapInfoTest);

/**
 * Test extraction of a single flag bit from the given value.
 */
void BitmapInfoTest::testFlagExtract()
{
	BitmapInfo::Flag flag;

	flag.setIndex(0);
	flag.setInversed(false);

	CPPUNIT_ASSERT_EQUAL(0, flag.extract(0));
	CPPUNIT_ASSERT_EQUAL(0, flag.extract(2));
	CPPUNIT_ASSERT_EQUAL(1, flag.extract(1));
	CPPUNIT_ASSERT_EQUAL(1, flag.extract(3));

	flag.setIndex(0);
	flag.setInversed(true);

	CPPUNIT_ASSERT_EQUAL(1, flag.extract(0));
	CPPUNIT_ASSERT_EQUAL(1, flag.extract(2));
	CPPUNIT_ASSERT_EQUAL(0, flag.extract(1));
	CPPUNIT_ASSERT_EQUAL(0, flag.extract(3));

	flag.setIndex(3);
	flag.setInversed(false);

	CPPUNIT_ASSERT_EQUAL(0, flag.extract(0));
	CPPUNIT_ASSERT_EQUAL(0, flag.extract(7));
	CPPUNIT_ASSERT_EQUAL(1, flag.extract(8));
	CPPUNIT_ASSERT_EQUAL(1, flag.extract(9));

	flag.setIndex(3);
	flag.setInversed(true);

	CPPUNIT_ASSERT_EQUAL(1, flag.extract(0));
	CPPUNIT_ASSERT_EQUAL(1, flag.extract(7));
	CPPUNIT_ASSERT_EQUAL(0, flag.extract(8));
	CPPUNIT_ASSERT_EQUAL(0, flag.extract(9));

	CPPUNIT_ASSERT_THROW(
		flag.setIndex(32),
		InvalidArgumentException);
}

/**
 * Test extraction of value specified by a group of bits.
 * The bits can be mapped non-ordered.
 */
void BitmapInfoTest::testGroupExtract()
{
	BitmapInfo::Group group;

	group.setBits({1, 3, 5});

	CPPUNIT_ASSERT_EQUAL(7, group.extract(2 + 8 + 32));
	CPPUNIT_ASSERT_EQUAL(6, group.extract(0 + 8 + 32));
	CPPUNIT_ASSERT_EQUAL(0, group.extract(0 + 0 + 0));
	CPPUNIT_ASSERT_EQUAL(1, group.extract(2 + 0 + 0));
	CPPUNIT_ASSERT_EQUAL(2, group.extract(0 + 8 + 0));
	CPPUNIT_ASSERT_EQUAL(0, group.extract(1 + 4 + 16 + 64));
	CPPUNIT_ASSERT_EQUAL(0, group.extract(0));

	// order of bits is important
	group.setBits({2, 1, 0, 4});

	CPPUNIT_ASSERT_EQUAL(0xf, group.extract(1 + 2 + 4 + 16));
	CPPUNIT_ASSERT_EQUAL(0xb, group.extract(0 + 2 + 4 + 16));
	CPPUNIT_ASSERT_EQUAL(0xd, group.extract(1 + 0 + 4 + 16));
	CPPUNIT_ASSERT_EQUAL(0xe, group.extract(1 + 2 + 0 + 16));
	CPPUNIT_ASSERT_EQUAL(0x7, group.extract(1 + 2 + 4 + 0));
	CPPUNIT_ASSERT_EQUAL(0, group.extract(0));
}

/**
 * Test that group of bits can be compared based on the group
 * bits indexes.
 */
void BitmapInfoTest::testGroupCompare()
{
	BitmapInfo::Group a;
	BitmapInfo::Group b;
	BitmapInfo::Group c;
	BitmapInfo::Group d;

	a.setBits({0, 1, 2});
	b.setBits({2, 3, 4});
	c.setBits({2, 3, 4});
	d.setBits({0, 1});

	CPPUNIT_ASSERT(a < b);
	CPPUNIT_ASSERT(b > a);
	CPPUNIT_ASSERT(!(a == b));
	CPPUNIT_ASSERT(a != b);

	CPPUNIT_ASSERT(!(b < c));
	CPPUNIT_ASSERT(!(c > b));
	CPPUNIT_ASSERT(b == c);
	CPPUNIT_ASSERT(!(b != c));

	CPPUNIT_ASSERT(d < a);
	CPPUNIT_ASSERT(a > d);
	CPPUNIT_ASSERT(!(a == d));
	CPPUNIT_ASSERT(a != d);
}

/**
 * Test we can reliably read all bits specifications of a BitmapInfo
 * instance. A single BitmapInfo can hold either flags or groups of bits.
 * All of them can overlap.
 */
void BitmapInfoTest::testBits()
{
	BitmapInfo info;
	using Flag = BitmapInfo::Flag;
	using Group = BitmapInfo::Group;

	info.setFlags({
		{Flag(0).index(), Flag(0)},
		{Flag(2).index(), Flag(2, true)},
		{Flag(3).index(), Flag(3, true)},
		{Flag(8).index(), Flag(8)},
	});

	info.setGroups({
		Group({0, 5, 6}),
		Group({1, 2, 3}),
		Group({0, 2, 7}),
	});

	CPPUNIT_ASSERT_EQUAL(7, info.bits().size());

	auto bits = info.bits();
	auto it = bits.begin();

	CPPUNIT_ASSERT(it != bits.end());
	CPPUNIT_ASSERT(typeid(it->get()) == typeid(Flag));
	CPPUNIT_ASSERT_EQUAL(0, dynamic_cast<const Flag &>(it->get()).index());

	++it;
	CPPUNIT_ASSERT(it != bits.end());
	CPPUNIT_ASSERT(typeid(it->get()) == typeid(Flag));
	CPPUNIT_ASSERT_EQUAL(2, dynamic_cast<const Flag &>(it->get()).index());

	++it;
	CPPUNIT_ASSERT(it != bits.end());
	CPPUNIT_ASSERT(typeid(it->get()) == typeid(Flag));
	CPPUNIT_ASSERT_EQUAL(3, dynamic_cast<const Flag &>(it->get()).index());

	++it;
	CPPUNIT_ASSERT(it != bits.end());
	CPPUNIT_ASSERT(typeid(it->get()) == typeid(Flag));
	CPPUNIT_ASSERT_EQUAL(8, dynamic_cast<const Flag &>(it->get()).index());

	++it;
	CPPUNIT_ASSERT(it != bits.end());
	CPPUNIT_ASSERT(typeid(it->get()) == typeid(Group));
	const vector<uint8_t> a{0, 2, 7};
	CPPUNIT_ASSERT(a == dynamic_cast<const Group &>(it->get()).bits());

	++it;
	CPPUNIT_ASSERT(it != bits.end());
	CPPUNIT_ASSERT(typeid(it->get()) == typeid(Group));
	const vector<uint8_t> b{0, 5, 6};
	CPPUNIT_ASSERT(b == dynamic_cast<const Group &>(it->get()).bits());

	++it;
	CPPUNIT_ASSERT(it != bits.end());
	CPPUNIT_ASSERT(typeid(it->get()) == typeid(Group));
	const vector<uint8_t> c{1, 2, 3};
	CPPUNIT_ASSERT(c == dynamic_cast<const Group &>(it->get()).bits());

	++it;
	CPPUNIT_ASSERT(it == bits.end());
}

}
