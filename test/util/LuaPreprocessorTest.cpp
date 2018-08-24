#include <cppunit/extensions/HelperMacros.h>

#include <iostream>
#include <Poco/Exception.h>

#include "cppunit/BetterAssert.h"
#include "util/LuaPreprocessor.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class LuaPreprocessorTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(LuaPreprocessorTest);
	CPPUNIT_TEST(testNoInput);
	CPPUNIT_TEST(testTextOnlyChunk);
	CPPUNIT_TEST(testCodeOnlyChunk);
	CPPUNIT_TEST(testMultipleChunks);
	CPPUNIT_TEST(testForLoop);
	CPPUNIT_TEST(testMissingClose);
	CPPUNIT_TEST(testNonMatchingClose);
	CPPUNIT_TEST_SUITE_END();

public:
	void testNoInput();
	void testTextOnlyChunk();
	void testCodeOnlyChunk();
	void testMultipleChunks();
	void testForLoop();
	void testMissingClose();
	void testNonMatchingClose();
};

CPPUNIT_TEST_SUITE_REGISTRATION(LuaPreprocessorTest);

void LuaPreprocessorTest::testNoInput()
{
	LuaPreprocessor preprocessor;

	CPPUNIT_ASSERT_EQUAL("", preprocessor.process(""));
}

void LuaPreprocessorTest::testTextOnlyChunk()
{
	LuaPreprocessor preprocessor;

	CPPUNIT_ASSERT_EQUAL(
		"some text\n text text\t\n text",
		preprocessor.process("some text\n text text\t\n text"));
}

void LuaPreprocessorTest::testCodeOnlyChunk()
{
	LuaPreprocessor preprocessor;

	CPPUNIT_ASSERT_EQUAL(
		"some text",
		preprocessor.process("<? echo(\"some text\"); ?>"));
}

void LuaPreprocessorTest::testMultipleChunks()
{
	LuaPreprocessor preprocessor;

	CPPUNIT_ASSERT_EQUAL(
		"some text\nsome other text\n6 = 6",
		preprocessor.process(
			"<? echo(\"some text\"); ?>\n"
			"some other text\n"
			"<? echo(1 + 5); ?> = 6"
		));
}

void LuaPreprocessorTest::testForLoop()
{
	LuaPreprocessor preprocessor;

	CPPUNIT_ASSERT_EQUAL(
		"\n1. line\n\n2. line\n\n3. line\n\n4. line\n\n5. line\n"
		"\n6. line\n\n7. line\n\n8. line\n\n9. line\n\n10. line\n",
		preprocessor.process(
			"<? for i = 1,10 do ?>\n"
			"<? echo(i) ?>. line\n"
			"<? end ?>"
		));
}

void LuaPreprocessorTest::testMissingClose()
{
	LuaPreprocessor preprocessor;

	CPPUNIT_ASSERT_THROW(
		preprocessor.process("<?"),
		SyntaxException);

	CPPUNIT_ASSERT_THROW(
		preprocessor.process("<? echo(\"text\"); ?> text <?"),
		SyntaxException);
}

void LuaPreprocessorTest::testNonMatchingClose()
{
	LuaPreprocessor preprocessor;

	CPPUNIT_ASSERT_EQUAL(
		"?>",
		preprocessor.process("?>"));

	CPPUNIT_ASSERT_EQUAL(
		"text text ?>",
		preprocessor.process("<? echo(\"text\"); ?> text ?>"));
}

}
