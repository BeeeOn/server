#ifndef BEEEON_SANITIZE_TEST_H
#define BEEEON_SANITIZE_TEST_H

#include <cstring>
#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Exception.h>
#include <Poco/Unicode.h>
#include <Poco/URI.h>

#include "cppunit/BetterAssert.h"
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
	CPPUNIT_TEST(testSanitizeEmail);
	CPPUNIT_TEST(testInvalidEmail);
	CPPUNIT_TEST(testXMLEntities);
	CPPUNIT_TEST(testNonAscii);
	CPPUNIT_TEST(testSanitizeCommon);
	CPPUNIT_TEST(testSanitizeStrict);
	CPPUNIT_TEST(testSanitizeUri);
	CPPUNIT_TEST(testSanitizeBase64);
	CPPUNIT_TEST(testSanitizeBase64URL);
	CPPUNIT_TEST(testSanitizeBase64Fragments);
	CPPUNIT_TEST(testSanitizeToken);
	CPPUNIT_TEST_SUITE_END();
public:
	void testSanitizeSizeLimit();
	void testSanitizeInvalidUTF8();
	void testSanitizeAttacks();
	void testNonShortestUTF8Forms();
	void testBOMIsNotRemoved();
	void testSanitizeEmail();
	void testInvalidEmail();
	void testXMLEntities();
	void testNonAscii();
	void testSanitizeCommon();
	void testSanitizeStrict();
	void testSanitizeUri();
	void testSanitizeBase64();
	void testSanitizeBase64URL();
	void testSanitizeBase64Fragments();
	void testSanitizeToken();
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

void SanitizeTest::testSanitizeEmail()
{
	CPPUNIT_ASSERT_EQUAL(string("example@example.org"),
		Sanitize::email("Example@Example.org"));

	CPPUNIT_ASSERT_EQUAL(string("example.account@gmail.com"),
		Sanitize::email("Example.Account@gmail.com"));

	CPPUNIT_ASSERT_EQUAL(string(".!#$%&'*+\\/=?^_`{|}~-@.-.ab"),
		Sanitize::email(".!#$%&'*+\\/=?^_`{|}~-@.-.ab"));
}

void SanitizeTest::testInvalidEmail()
{
	CPPUNIT_ASSERT_THROW(Sanitize::email("noemail"), InvalidAccessException);
	CPPUNIT_ASSERT_THROW(Sanitize::email("too-long-local-part-with-more-then-64-characters-well-there-is-65@"), InvalidAccessException);
	CPPUNIT_ASSERT_THROW(Sanitize::email("..@Example.org"), InvalidAccessException);
	CPPUNIT_ASSERT_THROW(Sanitize::email("<script>@Example.org"), InvalidAccessException);
	CPPUNIT_ASSERT_THROW(Sanitize::email("example@Example.org\xc0\xae"), InvalidAccessException);
	CPPUNIT_ASSERT_THROW(Sanitize::email("example@Example.org***"), InvalidAccessException);
}

void SanitizeTest::testXMLEntities()
{
	CPPUNIT_ASSERT_EQUAL(string("&#38;"), Sanitize::xml("&"));
	CPPUNIT_ASSERT_EQUAL(string("&#60;"), Sanitize::xml("<"));
	CPPUNIT_ASSERT_EQUAL(string("&#62;"), Sanitize::xml(">"));
	CPPUNIT_ASSERT_EQUAL(string("&#34;"), Sanitize::xml("\""));
	CPPUNIT_ASSERT_EQUAL(string("&#39;"), Sanitize::xml("\'"));
	CPPUNIT_ASSERT_EQUAL(string("&#47;"), Sanitize::xml("/"));

	CPPUNIT_ASSERT_EQUAL(string("&#38;amp;"), Sanitize::xml("&amp;"));
	CPPUNIT_ASSERT_EQUAL(string("&#38;lt;"), Sanitize::xml("&lt;"));
	CPPUNIT_ASSERT_EQUAL(string("&#38;gt;"), Sanitize::xml("&gt;"));
	CPPUNIT_ASSERT_EQUAL(string("&#38;quot;"), Sanitize::xml("&quot;"));

	CPPUNIT_ASSERT_EQUAL(string("&#38;#x26;"), Sanitize::xml("&#x26;"));
	CPPUNIT_ASSERT_EQUAL(string("&#38;#x3C;"), Sanitize::xml("&#x3C;"));
	CPPUNIT_ASSERT_EQUAL(string("&#38;#x3E;"), Sanitize::xml("&#x3E;"));
	CPPUNIT_ASSERT_EQUAL(string("&#38;#x27;"), Sanitize::xml("&#x27;"));
	CPPUNIT_ASSERT_EQUAL(string("&#38;#x22;"), Sanitize::xml("&#x22;"));
	CPPUNIT_ASSERT_EQUAL(string("&#38;#x2F;"), Sanitize::xml("&#x2F;"));
}

void SanitizeTest::testNonAscii()
{
	// alpha
	CPPUNIT_ASSERT_EQUAL(string("&#945;"), Sanitize::xml("\xce\xb1"));
	// beta
	CPPUNIT_ASSERT_EQUAL(string("&#946;"), Sanitize::xml("\xce\xb2"));
	// gamma
	CPPUNIT_ASSERT_EQUAL(string("&#947;"), Sanitize::xml("\xce\xb3"));
	// all at once
	CPPUNIT_ASSERT_EQUAL(string("&#945;&#946;&#947;"), Sanitize::xml("\xce\xb1\xce\xb2\xce\xb3"));

	// euro
	CPPUNIT_ASSERT_EQUAL(string("&#8364;"), Sanitize::xml("\xe2\x82\xac"));
}

void SanitizeTest::testSanitizeCommon()
{
	CPPUNIT_ASSERT_NO_THROW(Sanitize::common("abcdefghijklmnopqrstuvwxyz"));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::common("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::common("0123456789"));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::common(".:!?()/,-#'$€¥£©®+"));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::common("My Common Place #1"));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::common("Frank O'Neil"));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::common("ěščřžýáíéůúóňďľĚŠČŘŽÝÁÍÉŮÚÓŇĎĽ"));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::common("ßàäæçèëöŰű"));

	// examples not accepted:
	CPPUNIT_ASSERT_THROW(Sanitize::common("&"),  InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::common("%"),  InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::common("@"),  InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::common("~"),  InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::common(";"),  InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::common("\\"), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::common("\""), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::common("*"),  InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::common("|"),  InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::common("\n"), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::common("\t"), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::common("\b"), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::common("\r"), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::common("¬"),  InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::common("§"),  InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::common("¶"),  InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::common("¼"),  InvalidArgumentException);
}

void SanitizeTest::testSanitizeStrict()
{
	CPPUNIT_ASSERT_NO_THROW(Sanitize::strict(" \tabcdefghijk \tlmnopqrstuvwxyz"));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::strict(" \tABCDEFGHIJK \tLMNOPQRSTUVWXYZ"));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::strict(" \t01234 \t56789- \t"));

	CPPUNIT_ASSERT_THROW(Sanitize::strict("?"), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::strict("\n"), InvalidArgumentException);
}

void SanitizeTest::testSanitizeUri()
{
	URI result;

	result = Sanitize::uri("http://www.example.org/%c0%ae");
	CPPUNIT_ASSERT_EQUAL(string("http://www.example.org/"), result.toString());

	result = Sanitize::uri("http://www.example.org/%25c0%25ae");
	CPPUNIT_ASSERT_EQUAL(string("http://www.example.org/%25c0%25ae"), result.toString());

	result = Sanitize::uri("http://www.example.org/%25");
	CPPUNIT_ASSERT_EQUAL(string("http://www.example.org/%25"), result.toString());

	result = Sanitize::uri("http://www.example.org/%ffx=6");
	CPPUNIT_ASSERT_EQUAL(string("http://www.example.org/?x=6"), result.toString());
}

void SanitizeTest::testSanitizeBase64()
{
	CPPUNIT_ASSERT_NO_THROW(Sanitize::base64(""));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::base64("abcdefghijklmnopqrstuvwxyz"));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::base64("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::base64("0123456789+/"));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::base64("0="));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::base64("0=="));

	CPPUNIT_ASSERT_THROW(Sanitize::base64("="), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::base64("=="), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::base64("0==1"), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::base64("0==="), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::base64(" "), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::base64("\n"), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::base64("\r"), InvalidArgumentException);

	CPPUNIT_ASSERT_EQUAL(string(""), Sanitize::base64("\x00"));
}

/**
 * The Base64 for URL variant uses '-', '_' instead of '+', '/'.
 * Allow the input to be only either the standard Base64 or Base64
 * for URL (thus without mixing the '-', '_' and '+', '/' characters).
 */
void SanitizeTest::testSanitizeBase64URL()
{
	CPPUNIT_ASSERT_NO_THROW(Sanitize::base64("+/"));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::base64("-_"));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::base64("kLYz_C_KfRynpesleipBeuYDUdQ5pv5ZeBgzsUCgyyY"));

	CPPUNIT_ASSERT_THROW(Sanitize::base64("+-"), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::base64("+_"), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::base64("/-"), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::base64("/_"), InvalidArgumentException);
}

void SanitizeTest::testSanitizeBase64Fragments()
{
	CPPUNIT_ASSERT_NO_THROW(Sanitize::base64("", ","));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::base64("abcde,fghijk,lmnopqr,stuv;wxyz", ",;"));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::base64("ABCDEF?GHI?JKLM?NOPQR?STUVWXYZ", "?"));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::base64("0123.456.789+./", "."));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::base64("0=", ","));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::base64("0==", ","));

	CPPUNIT_ASSERT_THROW(Sanitize::base64("0,=",    ","), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::base64("0,",     ","), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::base64("0,==",   ","), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::base64("0,0==1", ","), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::base64("0,0===", ","), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::base64(" ,",     ","), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::base64("\n,",    ","), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::base64("\r,",    ","), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::base64(", ",     ","), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::base64(",\n",    ","), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::base64(",\r",    ","), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::base64("0,\x00", ","), InvalidArgumentException);

	CPPUNIT_ASSERT_EQUAL(string(""), Sanitize::base64("\x00,\x00", ","));
}

void SanitizeTest::testSanitizeToken()
{
	CPPUNIT_ASSERT_NO_THROW(Sanitize::token("0123456789"));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::token("abcdefghijklmnopqrstuvwxyz"));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::token("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
	CPPUNIT_ASSERT_NO_THROW(Sanitize::token("-_.:"));

	CPPUNIT_ASSERT_THROW(Sanitize::token(","), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::token(";"), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::token("<"), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::token(">"), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::token("\n"), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::token("\r"), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::token(" "), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::token("\t"), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(Sanitize::token("\0"), InvalidArgumentException);

	// JWT HS256 example
	CPPUNIT_ASSERT_NO_THROW(Sanitize::token(
		"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9"
		"."
		"eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiYWRtaW4iOnRydWV9"
		"."
		"TJVA95OrM7E2cBab30RMHrHDcEfxjoYZgeFONFh7HgQ"
	));

	// JWT RS256 example
	CPPUNIT_ASSERT_NO_THROW(Sanitize::token(
		"eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9"
		"."
		"eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiYWRtaW4iOnRydWV9"
		"."
		"EkN-DOsnsuRjRO6BxXemmJDm3HbxrbRzXglbN2S4sOkopdU4IsDxTI8jO19W_A4K8ZPJijN"
		"Lis4EZsHeY559a4DFOd50_OqgHGuERTqYZyuhtF39yxJPAjUESwxk2J5k_4zM3O-vtd1Ghyo"
		"4IbqKKSy6J9mTniYJPenn5-HIirE"
	));
}

}

#endif
