#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Environment.h>

#include "cppunit/BetterAssert.h"
#include "util/SQLPreprocessor.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class SQLPreprocessorTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(SQLPreprocessorTest);
	CPPUNIT_TEST(testNoTouch);
	CPPUNIT_TEST(testCommentLines);
	CPPUNIT_TEST(testCommentFloating);
	CPPUNIT_TEST(testCommentCareful);
	CPPUNIT_TEST(testCommentWrong);
	CPPUNIT_TEST(testWhitespaceLines);
	CPPUNIT_TEST(testWhitespaceEndings);
	CPPUNIT_TEST(testUnneededLines);
	CPPUNIT_TEST(testUnneededLinesComplex);
	CPPUNIT_TEST_SUITE_END();

public:
	void testNoTouch();
	void testCommentLines();
	void testCommentFloating();
	void testCommentCareful();
	void testCommentWrong();
	void testWhitespaceLines();
	void testWhitespaceEndings();
	void testUnneededLines();
	void testUnneededLinesComplex();
};

CPPUNIT_TEST_SUITE_REGISTRATION(SQLPreprocessorTest);

void SQLPreprocessorTest::testNoTouch()
{
	SQLPreprocessor pre;
	pre.setRemoveComments(false);
	pre.setRemoveWhitespace(false);
	pre.setPreserveUnneededLines(true);

	CPPUNIT_ASSERT_EQUAL(
		"SELECT * FROM users",
		pre.process("SELECT * FROM users"));

	CPPUNIT_ASSERT_EQUAL(
		"SELECT * FROM users\n",
		pre.process("SELECT * FROM users\n"));

	CPPUNIT_ASSERT_EQUAL(
		"SELECT * FROM users -- nice query",
		pre.process("SELECT * FROM users -- nice query"));

	CPPUNIT_ASSERT_EQUAL(
		"SELECT * FROM users -- nice query\n"
		"WHERE id = ?   \n"
		"\n"
		"  AND name = ? -- also by name\n"
		"  \n"
		"  -- some text\n",
		pre.process(
			"SELECT * FROM users -- nice query\n"
			"WHERE id = ?   \n"
			"\n"
			"  AND name = ? -- also by name\n"
			"  \n"
			"  -- some text\n"));
}

void SQLPreprocessorTest::testCommentLines()
{
	SQLPreprocessor pre;
	pre.setRemoveComments(true);
	pre.setRemoveWhitespace(false);
	pre.setPreserveUnneededLines(true);

	CPPUNIT_ASSERT_EQUAL(
		"",
		pre.process("-- SELECT * FROM users"));

	CPPUNIT_ASSERT_EQUAL(
		"\n",
		pre.process("-- SELECT * FROM users\n"));

	CPPUNIT_ASSERT_EQUAL(
		"   \n"
		"SELECT * FROM users\n"
		"\n"
		"WHERE\n"
		"\n"
		"\n"
		"  id = ?\n",
		pre.process(
			"   \n"
			"SELECT * FROM users\n"
			"-- table users\n"
			"WHERE\n"
			"-- interesting \n"
			"-- condition \n"
			"  id = ?\n"));
}

void SQLPreprocessorTest::testCommentFloating()
{
	SQLPreprocessor pre;
	pre.setRemoveComments(true);
	pre.setRemoveWhitespace(false);
	pre.setPreserveUnneededLines(true);

	CPPUNIT_ASSERT_EQUAL(
		"   ",
		pre.process("   -- SELECT * FROM users"));

	CPPUNIT_ASSERT_EQUAL(
		"   \n",
		pre.process("   -- SELECT * FROM users\n"));

	CPPUNIT_ASSERT_EQUAL(
		"   \n"
		"SELECT * FROM users \n"
		"\n"
		"WHERE\n"
		"  name = ? \n"
		"  \n"
		"\t\n"
		"  id = ?\n",
		pre.process(
			"   \n"
			"SELECT * FROM users -- users table\n"
			"-- doc comment --\n"
			"WHERE\n"
			"  name = ? -- what about name?\n"
			"  -- interesting \n"
			"\t-- condition \n"
			"  id = ?\n"));
}

void SQLPreprocessorTest::testCommentCareful()
{
	SQLPreprocessor pre;
	pre.setRemoveComments(true);
	pre.setRemoveWhitespace(false);
	pre.setPreserveUnneededLines(true);

	CPPUNIT_ASSERT_EQUAL(
		"SELECT * FROM users WHERE name = '--'",
		pre.process("SELECT * FROM users WHERE name = '--'"));

	CPPUNIT_ASSERT_EQUAL(
		"SELECT * FROM users \n"
		"WHERE \n"
		"  id = ?\n",
		pre.process(
			"SELECT * FROM users -- users table\n"
			"WHERE -- (this is a 'condition')\n"
			"  id = ?\n"));

	CPPUNIT_ASSERT_EQUAL(
		"INSERT INTO users (id, name)\n"
		"VALUES ( \n"
		"   '1242023', -- random ID\n"
		"   '-- text --', -- comment\n"
		")\n",
		pre.process(
			"INSERT INTO users (id, name)\n"
			"VALUES ( -- comment here\n"
			"   '1242023', -- random ID\n"
			"   '-- text --', -- comment\n"
			")\n"));
}

/**
 * This test shows a very insane behaviour that can happen
 * for PostgreSQL syntax with multiline strings. Multiline
 * strings uses $<MARK>$ ... $<MARK>$ syntax and can cross
 * lines which we do not consider in SQLPreprocessor. Thus,
 * such query would not work as expected.
 */
void SQLPreprocessorTest::testCommentWrong()
{
	SQLPreprocessor pre;
	pre.setRemoveComments(true);
	pre.setRemoveWhitespace(false);
	pre.setPreserveUnneededLines(true);

	CPPUNIT_ASSERT_EQUAL(
		"INSERT INTO users (name)\n"
		"VALUES (\n"
		"   $$\n"
		"   \n" // <<< this is the insane thing
		"   $$\n"
		")\n",
		pre.process(
			"INSERT INTO users (name)\n"
			"VALUES (\n"
			"   $$\n"
			"   -- this is not a comment\n"
			"   $$\n"
			")\n"));
}

void SQLPreprocessorTest::testWhitespaceLines()
{
	SQLPreprocessor pre;
	pre.setRemoveComments(false);
	pre.setRemoveWhitespace(true);
	pre.setPreserveUnneededLines(true);

	CPPUNIT_ASSERT_EQUAL(
		"\n"
		"\n"
		"\n",
		pre.process(
			"  \n"
			"\t\t\n"
			"   \t  \t\n"));

	CPPUNIT_ASSERT_EQUAL(
		"SELECT * FROM\n"
		"\n"
		"   users\n"
		"\n"
		"\tWHERE id = ?\n"
		"\n"
		"\n",
		pre.process(
			"SELECT * FROM\n"
			"      \n"
			"   users\n"
			"\t\n"
			"\tWHERE id = ?\n"
			"\n"
			"  \n"));
}

void SQLPreprocessorTest::testWhitespaceEndings()
{
	SQLPreprocessor pre;
	pre.setRemoveComments(false);
	pre.setRemoveWhitespace(true);
	pre.setPreserveUnneededLines(true);

	CPPUNIT_ASSERT_EQUAL(
		"-- anything\n"
		"SELECT\n"
		"*\n"
		"   FROM  users -- text\n"
		"WHERE name = 'x x x '\n"
		"\n",
		pre.process(
			"-- anything  \n"
			"SELECT \t\t\n"
			"*   \t  \t\n"
			"   FROM  users -- text    \n"
			"WHERE name = 'x x x '   \n"
			"   \n"));

	CPPUNIT_ASSERT_EQUAL(
		"SELECT * FROM users",
		pre.process(
			"SELECT * FROM users   "));
}

void SQLPreprocessorTest::testUnneededLines()
{
	SQLPreprocessor pre;
	pre.setRemoveComments(false);
	pre.setRemoveWhitespace(false);
	pre.setPreserveUnneededLines(false);

	CPPUNIT_ASSERT_EQUAL(
		"SELECT * FROM users",
		pre.process(
			"SELECT * FROM users\n"));


	CPPUNIT_ASSERT_EQUAL(
		"SELECT * FROM   \n"
		"-- some table  \n"
		"  users",
		pre.process(
			"\n"
			"SELECT * FROM   \n"
			"\n"
			"-- some table  \n"
			"\n"
			"  users"));
}

void SQLPreprocessorTest::testUnneededLinesComplex()
{
	SQLPreprocessor pre;
	pre.setRemoveComments(true);
	pre.setRemoveWhitespace(true);
	pre.setPreserveUnneededLines(false);

	CPPUNIT_ASSERT_EQUAL(
		"SELECT * FROM\n"
		"  users",
		pre.process(
			"\n"
			"SELECT * FROM   \n"
			"\n"
			"-- some table  \n"
			"\n"
			"  users"));

	CPPUNIT_ASSERT_EQUAL(
		" SELECT * FROM\n"
		"  users\n"
		" WHERE id = '-- text --'  -- comment\n"
		"    AND name = 'xxxx' -- comment",
		pre.process(
			" -- nice query to be used\n"
			" -- query using 'quotes' and comments  \n"
			" SELECT * FROM  -- table  \n"
			"  users\t\t   \n"
			"  -- users are happy\n"
			" WHERE id = '-- text --'  -- comment \n"
			"    AND name = 'xxxx' -- comment  \n"
			"  -- unimportant query\n"));
}

}
