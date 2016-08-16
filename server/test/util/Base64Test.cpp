#ifndef BEEEON_BASE64_TEST_H
#define BEEEON_BASE64_TEST_H

#include <cstring>
#include <cppunit/extensions/HelperMacros.h>

#include "util/Base64.h"

using namespace std;

namespace BeeeOn {

class Base64Test : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(Base64Test);
	CPPUNIT_TEST(testEncode);
	CPPUNIT_TEST(testDecode);
	CPPUNIT_TEST_SUITE_END();
public:
	void testEncode();
	void testDecode();
};

CPPUNIT_TEST_SUITE_REGISTRATION(Base64Test);

void Base64Test::testEncode()
{
	char buf[32] = {
		'\x05', '\x16', '\xab', '\xff', '\x12', '\x95', '\x49', '\x90',
		'\x34', '\x44', '\x17', '\x47', '\x77', '\x78', '\x77', '\x10',
		'\x00', '\xed', '\xcc', '\x10', '\x89', '\xa8', '\xaa', '\xa8',
		'\x01', '\xdc', '\xc2', '\x47', '\x8b', '\x78', '\x77', '\x10',
	};

	const string expect =
		"XHgwNVx4MTZceGFiXHhmZlx4MTJceDk"
		"1XHg0OVx4OTBceDM0XHg0NFx4MTdceD"
		"Q3XHg3N1x4NzhceDc3XHgxMFx4MDBce"
		"GVkXHhjY1x4MTBceDg5XHhhOFx4YWFc"
		"eGE4XHgwMVx4ZGNceGMyXHg0N1x4OGJ"
		"ceDc4XHg3N1x4MTAK";

	const string &result = Base64::encode(buf, sizeof(buf));
	CPPUNIT_ASSERT(result.compare(expect));
}

void Base64Test::testDecode()
{
	const string data = "YW55IGFyYml0cmFyeSB0ZXN0aW5nIHRleHQK";
	char buf[32];
	size_t len;

	const char expect[] = {
		'a', 'n', 'y', ' ', 'a', 'r', 'b', 'i',
		't', 'r', 'a', 'r', 'y', ' ', 't', 'e',
		's', 't', 'i', 'n', 'g', ' ', 't', 'e', 'x', 't', '\n'};

	len = Base64::decode(data, buf, sizeof(buf));
	CPPUNIT_ASSERT_EQUAL(sizeof(expect), len);
	CPPUNIT_ASSERT(std::memcmp(buf, expect, sizeof(expect)) == 0);
}

}

#endif
