#ifndef BEEEON_SANITIZE_TEST_H
#define BEEEON_SANITIZE_TEST_H

#include <cstring>
#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Exception.h>
#include <Poco/Unicode.h>

#include "util/Sanitize.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class SanitizeTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(SanitizeTest);
	CPPUNIT_TEST(testSanitizeSizeLimit);
	CPPUNIT_TEST(testSanitizeInvalidUTF8);
	CPPUNIT_TEST(testSanitizeAttacks);
	CPPUNIT_TEST(testNonShortestUTF8Forms);
	CPPUNIT_TEST(testBOMIsNotRemoved);
	CPPUNIT_TEST_SUITE_END();
public:
	void testSanitizeSizeLimit();
	void testSanitizeInvalidUTF8();
	void testSanitizeAttacks();
	void testNonShortestUTF8Forms();
	void testBOMIsNotRemoved();
};

CPPUNIT_TEST_SUITE_REGISTRATION(SanitizeTest);

void SanitizeTest::testSanitizeSizeLimit()
{
	CPPUNIT_ASSERT_EQUAL(string("test"), Sanitize::encoding("test", 4));
	CPPUNIT_ASSERT_THROW(Sanitize::encoding("test", 3), InvalidArgumentException);

	CPPUNIT_ASSERT_EQUAL(string("\xe0\xa5\x90"), Sanitize::encoding("\xe0\xa5\x90", 3));
	CPPUNIT_ASSERT_THROW(Sanitize::encoding("om\xe0\xa5\x90", 3), InvalidArgumentException);
}

/**
 * Well-formed UTF-8 overview can be found here:
 * http://www.unicode.org/versions/Unicode6.2.0/ch03.pdf (p. 95)
 */
void SanitizeTest::testSanitizeInvalidUTF8()
{
	CPPUNIT_ASSERT_EQUAL(string("????????????????"),
		Sanitize::encoding("\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f"));
	CPPUNIT_ASSERT_EQUAL(string("????????????????"),
		Sanitize::encoding("\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f"));
	CPPUNIT_ASSERT_EQUAL(string("????????????????"),
		Sanitize::encoding("\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf"));
	CPPUNIT_ASSERT_EQUAL(string("????????????????"),
		Sanitize::encoding("\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf"));

	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xf5\xf6\xf7\xf8"));
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xf6"));
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xf7"));
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xf8"));
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xf9"));
	CPPUNIT_ASSERT_EQUAL(string("???"),
		Sanitize::encoding("\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe"));
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xfa"));
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xfb"));
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xfc"));
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xfd"));
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xfe"));
	CPPUNIT_ASSERT_EQUAL(string("????"),
		Sanitize::encoding("\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff"));
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xff"));

	CPPUNIT_ASSERT_EQUAL(string("AÀA"),
		Sanitize::encoding("\x41\xc3\x80\x41"));
	CPPUNIT_ASSERT_EQUAL(string("A??A"),
		Sanitize::encoding("\x41\xc2\xc3\x80\x41"));
}

void SanitizeTest::testSanitizeAttacks()
{
	// http://www.perlmonks.org/?node_id=644786
	// \xc9\x3b is invalid, \x3b is semicolon
	CPPUNIT_ASSERT_EQUAL(string("foo?id"),
		Sanitize::encoding("\x66\x6f\x6f\xc9\x3b\x69\x64"));

	// might be hidden '/'
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xc0\x2f"));
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xc0\xaf"));

	// 0x3c is <
	CPPUNIT_ASSERT_EQUAL(string("?\x3c"),
		Sanitize::encoding("\xc2\x22\x3c"));

	// this is preserved, somebody however might later misinterpret the character
	// (U+180E) as a space
	CPPUNIT_ASSERT_EQUAL(string("<a href=#\xe1\xa0\x8eonclick=alert()>"),
		Sanitize::encoding("<a href=#\xe1\xa0\x8eonclick=alert()>"));
	CPPUNIT_ASSERT(!Unicode::isSpace(0x180e));
}

void SanitizeTest::testBOMIsNotRemoved()
{
	// however, better is to replace with '?'
	CPPUNIT_ASSERT_EQUAL(string("<scr\xef\xbb\xbfipt>"),
		Sanitize::encoding("<scr\xef\xbb\xbfipt>"));

}

// http://websec.github.io/unicode-security-guide/character-transformations/
void SanitizeTest::testNonShortestUTF8Forms()
{
	// A
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xc1\x81"));
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xe0\x81\x81"));
	// "
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xc0\xa2"));
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xe0\x80\xa2"));
	// '
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xc0\xa7"));
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xe0\x80\xa7"));
	// <
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xc0\xbc"));
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xe0\x80\xbc"));
	// >
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xc0\xbe"));
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xe0\x80\xbe"));
	// .
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xc0\xae"));
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xe0\x80\xae"));
	// /
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xc0\xaf"));
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xe0\x80\xaf"));
	/* \ */
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xc1\x9c"));
	CPPUNIT_ASSERT_EQUAL(string("?"),
		Sanitize::encoding("\xe0\x81\x9c"));
}

}

#endif
