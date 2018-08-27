#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Environment.h>

#include "cppunit/BetterAssert.h"
#include "dao/FilesystemQueryLoader.h"
#include "util/SQLPreprocessor.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class FilesystemQueryLoaderTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(FilesystemQueryLoaderTest);
	CPPUNIT_TEST(testEmpty);
	CPPUNIT_TEST(testWhitespace);
	CPPUNIT_TEST(testComments);
	CPPUNIT_TEST(testCommentsAndWhitespace);
	CPPUNIT_TEST(testQuerySingleLine);
	CPPUNIT_TEST(testQueryMultiLine);
	CPPUNIT_TEST(testQueryWithComments);
	CPPUNIT_TEST(testQueryWithCommentsAndWhitespace);
	CPPUNIT_TEST(testQueryEndingWithWhitespace);
	CPPUNIT_TEST(testLoadWithExtension);
	CPPUNIT_TEST(testNotFound);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();

	void testEmpty();
	void testWhitespace();
	void testComments();
	void testCommentsAndWhitespace();
	void testQuerySingleLine();
	void testQueryMultiLine();
	void testQueryWithComments();
	void testQueryWithCommentsAndWhitespace();
	void testQueryEndingWithWhitespace();
	void testLoadWithExtension();
	void testNotFound();

private:
	FilesystemQueryLoader loader;
	SQLPreprocessor::Ptr preprocessor;
};

CPPUNIT_TEST_SUITE_REGISTRATION(FilesystemQueryLoaderTest);

void FilesystemQueryLoaderTest::setUp()
{
	static const string ROOT_PATH = "test/dao/";
	loader.setRootPath(Environment::get("QUERIES_PATH", ROOT_PATH));

	preprocessor = new SQLPreprocessor;
	preprocessor->setPreserveUnneededLines(false);

	loader.addPreprocessor(preprocessor);
}

/**
 * Test we can load an empty file.
 */
void FilesystemQueryLoaderTest::testEmpty()
{
	const string &empty = loader.find("queries.empty.empty");
	CPPUNIT_ASSERT(empty.empty());
}

/**
 * Test we can load a file containing only whitespace. As default,
 * the whitespace is discarded. When preserving unneeded lines, the
 * whitespace is also discarded, however, *empty* lines are still
 * preserved.
 */
void FilesystemQueryLoaderTest::testWhitespace()
{
	const string &empty = loader.find("queries.empty.whitespace");
	CPPUNIT_ASSERT(empty.empty());

	preprocessor->setPreserveUnneededLines(true);
	const string lines = loader.find("queries.empty.whitespace");
	CPPUNIT_ASSERT(!lines.empty());
	CPPUNIT_ASSERT_EQUAL("\n\n\n\n\n", lines);
}

/**
 * Test we can load file containing only comments. The comments
 * are discarded as default. When preserving unneeded lines, the
 * comments are discarded but *empty* lines stays there.
 */
void FilesystemQueryLoaderTest::testComments()
{
	const string empty = loader.find("queries.empty.comments");
	CPPUNIT_ASSERT(empty.empty());

	preprocessor->setPreserveUnneededLines(true);
	const string lines = loader.find("queries.empty.comments");

	CPPUNIT_ASSERT(!lines.empty());
	CPPUNIT_ASSERT_EQUAL("\n\n\n", lines);
}

/**
 * Test behaviour of loading file containing only whitespace and comments.
 * Both are discarded as default. When preserving unneeded lines, *empty*
 * lines (free of whitespace and comments) are preserved.
 */
void FilesystemQueryLoaderTest::testCommentsAndWhitespace()
{
	const string empty = loader.find("queries.empty.comments_and_whitespace");
	CPPUNIT_ASSERT(empty.empty());

	preprocessor->setPreserveUnneededLines(true);
	const string lines = loader.find("queries.empty.comments_and_whitespace");

	CPPUNIT_ASSERT(!lines.empty());
	CPPUNIT_ASSERT_EQUAL("\n\n\n\n\n\n", lines);
}

/**
 * Test we can load a single-line content.
 * No EOL character is to be present at the end of the loaded query
 * unless we preserve unneeded lines.
 */
void FilesystemQueryLoaderTest::testQuerySingleLine()
{
	const string query = loader.find("queries.sql.single_line");
	CPPUNIT_ASSERT_EQUAL("SELECT * FROM users WHERE id = ?", query);

	preprocessor->setPreserveUnneededLines(true);
	const string preserved = loader.find("queries.sql.single_line");
	CPPUNIT_ASSERT_EQUAL("SELECT * FROM users WHERE id = ?\n", preserved);
}

/**
 * Test we can load a multi-line content.
 * No EOL character is to be present at the end of the loaded query
 * unless we preserve unneeded lines.
 */
void FilesystemQueryLoaderTest::testQueryMultiLine()
{
	const string query = loader.find("queries.sql.multi_line");
	CPPUNIT_ASSERT_EQUAL(
		"SELECT\n\tname,\n\tage\nFROM\n\tusers\nWHERE\n\tid = ?",
		query);

	preprocessor->setPreserveUnneededLines(true);
	const string preserved = loader.find("queries.sql.multi_line");
	CPPUNIT_ASSERT_EQUAL(
		"SELECT\n\tname,\n\tage\nFROM\n\tusers\nWHERE\n\tid = ?\n",
		preserved);
}

/**
 * Test we can load a query containing some comments that are discarded
 * as default. When preserving unneeded lines, only empty lines are
 * preserved instead of those comments.
 */
void FilesystemQueryLoaderTest::testQueryWithComments()
{   
	const string query = loader.find("queries.sql.with_comments");
	CPPUNIT_ASSERT_EQUAL(
		"SELECT\n\tname,\n\temail\nFROM\n\tusers\nWHERE\n\tid = ?",
		query);

	preprocessor->setPreserveUnneededLines(true);
	const string preserved = loader.find("queries.sql.with_comments");
	CPPUNIT_ASSERT_EQUAL(
		"\n\nSELECT\n\tname,\n\temail\nFROM\n\tusers\nWHERE\n\tid = ?\n",
		preserved);
}

/**
 * Test we can load a query mixed with comments and whitespace.
 * As default *all* comments are discarded and all empty lines
 * (or empty lines containing only whitespace) are discarded.
 * When preserving unneeded lines, the lines after discarding
 * of comments and whitespaces are left in the query.
 */
void FilesystemQueryLoaderTest::testQueryWithCommentsAndWhitespace()
{
	const string query = loader.find("queries.sql.with_comments_and_whitespace");
	CPPUNIT_ASSERT_EQUAL(
		"SELECT\nname, age\nFROM\n\tusers\nWHERE\n\tid = ?",
		query);

	preprocessor->setPreserveUnneededLines(true);
	const string preserved = loader.find("queries.sql.with_comments_and_whitespace");
	CPPUNIT_ASSERT_EQUAL(
		"\n\n\nSELECT\n\nname, age\n\nFROM\n\tusers\n\nWHERE\n\tid = ?\n",
		preserved);
}

/**
 * Test that whitespace *after* the query is discarded unless preserving
 * unneeded lines.
 */
void FilesystemQueryLoaderTest::testQueryEndingWithWhitespace()
{
	const string query = loader.find("queries.sql.ending_with_whitespace");
	CPPUNIT_ASSERT_EQUAL(
		"SELECT\n\tname\nFROM\n\tusers\nWHERE\n\tid = ?",
		query);

	preprocessor->setPreserveUnneededLines(true);
	const string preserved = loader.find("queries.sql.ending_with_whitespace");
	CPPUNIT_ASSERT_EQUAL(
		"SELECT\n\tname\nFROM\n\tusers\nWHERE\n\tid = ?\n\n\n",
		preserved);

}

/**
 * Test that when the loader is configured to load files with an extension,
 * we can load such queries without any error.
 */
void FilesystemQueryLoaderTest::testLoadWithExtension()
{
	loader.setExtension("sql");

	string empty1;
	CPPUNIT_ASSERT_NO_THROW(
		empty1 = loader.find("queries.empty.empty1"));
	CPPUNIT_ASSERT(empty1.empty());

	string select;
	CPPUNIT_ASSERT_NO_THROW(
		select = loader.find("queries.sql.select"));
	CPPUNIT_ASSERT_EQUAL(
		"SELECT * FROM users\nWHERE id = ?",
		select);
}

/**
 * Test the NotFoundException is thrown when a query is
 * not found. Test the same behaviour for queries stored
 * in files with an extension.
 *
 * Note that when a query exists and its file has not any
 * extension, we must fail when loading queries *with*
 * a configured extension.
 */
void FilesystemQueryLoaderTest::testNotFound()
{
	CPPUNIT_ASSERT_THROW(
		loader.find("queries.sql.insert"),
		NotFoundException);

	// exists but has extension .sql
	CPPUNIT_ASSERT_THROW(
		loader.find("queries.sql.select"),
		NotFoundException);

	loader.setExtension("sql");

	// exists but only without extension .sql
	CPPUNIT_ASSERT_THROW(
		loader.find("queries.sql.with_comments"),
		NotFoundException);
}

}

