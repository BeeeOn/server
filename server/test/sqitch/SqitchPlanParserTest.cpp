#include <sstream>
#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Exception.h>

#include "cppunit/BetterAssert.h"
#include "sqitch/SqitchChange.h"
#include "sqitch/SqitchPragma.h"
#include "sqitch/SqitchTag.h"
#include "sqitch/SqitchPlanParser.h"

#include "sqitch/SqitchTestingPlans.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class SqitchPlanParserTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(SqitchPlanParserTest);
	CPPUNIT_TEST(testEmpty);
	CPPUNIT_TEST(testSimple);
	CPPUNIT_TEST(testMulti);
	CPPUNIT_TEST(testChangesOnly);
	CPPUNIT_TEST(testBadChange);
	CPPUNIT_TEST(testBadNames);
	CPPUNIT_TEST(testGoodNames);
	CPPUNIT_TEST(testReservedTags);
	CPPUNIT_TEST(testDupTag);
	CPPUNIT_TEST(testDeployRevert);
	CPPUNIT_TEST(testDependencies);
	CPPUNIT_TEST_SUITE_END();

public:
	void testEmpty();
	void testSimple();
	void testMulti();
	void testChangesOnly();
	void testBadChange();
	void testBadNames();
	void testGoodNames();
	void testReservedTags();
	void testDupTag();
	void testDeployRevert();
	void testDependencies();
};

CPPUNIT_TEST_SUITE_REGISTRATION(SqitchPlanParserTest);

void SqitchPlanParserTest::testEmpty()
{
	SqitchPlanParser parser;

	istringstream buffer;
	buffer.str("");

	CPPUNIT_ASSERT(parser.next(buffer).isNull());
}

void SqitchPlanParserTest::testSimple()
{
	SqitchPlanParser parser;

	istringstream buffer;
	buffer.str(SqitchTestingPlans::WIDGETS);

	SqitchLine::Ptr line;
	SqitchPragma::Ptr pragma;
	SqitchChange::Ptr change;
	SqitchTag::Ptr tag;

	pragma = parser.next(buffer).cast<SqitchPragma>();
	CPPUNIT_ASSERT_EQUAL("project", pragma->key());
	CPPUNIT_ASSERT_EQUAL("widgets", pragma->value());

	line = parser.next(buffer);
	CPPUNIT_ASSERT_EQUAL("This is a note", line->note());

	line = parser.next(buffer);
	CPPUNIT_ASSERT_EQUAL("And there was a blank line.", line->note());

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("hey", change->name());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", change->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", change->authorEmail());

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("you", change->name());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", change->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", change->authorEmail());

	tag = parser.next(buffer).cast<SqitchTag>();
	CPPUNIT_ASSERT_EQUAL("foo", tag->name());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", tag->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", tag->authorEmail());

	CPPUNIT_ASSERT(parser.next(buffer).isNull());
}

void SqitchPlanParserTest::testMulti()
{
	SqitchPlanParser parser;

	istringstream buffer;
	buffer.str(SqitchTestingPlans::MULTI);

	SqitchLine::Ptr line;
	SqitchPragma::Ptr pragma;
	SqitchChange::Ptr change;
	SqitchTag::Ptr tag;

	pragma = parser.next(buffer).cast<SqitchPragma>();
	CPPUNIT_ASSERT_EQUAL("project", pragma->key());
	CPPUNIT_ASSERT_EQUAL("multi", pragma->value());

	line = parser.next(buffer);
	CPPUNIT_ASSERT_EQUAL("This is a note", line->note());

	line = parser.next(buffer);
	CPPUNIT_ASSERT_EQUAL("And there was a blank line.", line->note());

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("hey", change->name());
	CPPUNIT_ASSERT_EQUAL("theory", change->author());
	CPPUNIT_ASSERT_EQUAL("t@heo.ry", change->authorEmail());

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("you", change->name());
	CPPUNIT_ASSERT_EQUAL("anna", change->author());
	CPPUNIT_ASSERT_EQUAL("a@n.na", change->authorEmail());

	tag = parser.next(buffer).cast<SqitchTag>();
	CPPUNIT_ASSERT_EQUAL("foo", tag->name());
	CPPUNIT_ASSERT_EQUAL("julie", tag->author());
	CPPUNIT_ASSERT_EQUAL("j@ul.ie", tag->authorEmail());
	CPPUNIT_ASSERT_EQUAL("look, a tag!", tag->note());

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("this/rocks", change->name());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", change->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", change->authorEmail());

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("hey-there", change->name());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", change->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", change->authorEmail());
	CPPUNIT_ASSERT_EQUAL("trailing note!", change->note());

	tag = parser.next(buffer).cast<SqitchTag>();
	CPPUNIT_ASSERT_EQUAL("bar", tag->name());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", tag->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", tag->authorEmail());

	tag = parser.next(buffer).cast<SqitchTag>();
	CPPUNIT_ASSERT_EQUAL("baz", tag->name());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", tag->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", tag->authorEmail());

	CPPUNIT_ASSERT(parser.next(buffer).isNull());
}

void SqitchPlanParserTest::testChangesOnly()
{
	SqitchPlanParser parser;

	istringstream buffer;
	buffer.str(SqitchTestingPlans::CHANGES_ONLY);

	SqitchLine::Ptr line;
	SqitchPragma::Ptr pragma;
	SqitchChange::Ptr change;

	pragma = parser.next(buffer).cast<SqitchPragma>();
	CPPUNIT_ASSERT_EQUAL("project", pragma->key());
	CPPUNIT_ASSERT_EQUAL("changes_only", pragma->value());

	line = parser.next(buffer);
	CPPUNIT_ASSERT_EQUAL("This is a note", line->note());

	line = parser.next(buffer);
	CPPUNIT_ASSERT_EQUAL("And there was a blank line.", line->note());

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("hey", change->name());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", change->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", change->authorEmail());

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("you", change->name());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", change->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", change->authorEmail());

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("whatwhatwhat", change->name());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", change->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", change->authorEmail());

	CPPUNIT_ASSERT(parser.next(buffer).isNull());
}

void SqitchPlanParserTest::testBadChange()
{
	SqitchPlanParser parser;

	istringstream buffer;
	buffer.str(SqitchTestingPlans::BAD_CHANGE);

	SqitchLine::Ptr line;
	SqitchPragma::Ptr pragma;

	pragma = parser.next(buffer).cast<SqitchPragma>();
	CPPUNIT_ASSERT_EQUAL("project", pragma->key());
	CPPUNIT_ASSERT_EQUAL("bad_change", pragma->value());

	line = parser.next(buffer);
	CPPUNIT_ASSERT_EQUAL("This is a note", line->note());

	line = parser.next(buffer);
	CPPUNIT_ASSERT_EQUAL("And there was a blank line.", line->note());

	// what what what
	CPPUNIT_ASSERT_THROW(
		parser.next(buffer),
		SyntaxException
	);

	CPPUNIT_ASSERT(parser.next(buffer).isNull());
}

void SqitchPlanParserTest::testBadNames()
{
	SqitchPlanParser parser;

	istringstream buffer;

	buffer.clear();
	buffer.str("^foo 2017-01-01T00:01:02Z tester <tester@example.org>");

	// ^foo
	CPPUNIT_ASSERT_THROW(
		parser.next(buffer),
		SyntaxException
	);

	buffer.clear();
	buffer.str("foo^ 2017-01-01T00:01:02Z tester <tester@example.org>");

	// foo^
	CPPUNIT_ASSERT_THROW(
		parser.next(buffer),
		SyntaxException
	);

	buffer.clear();
	buffer.str("foo^6 2017-01-01T00:01:02Z tester <tester@example.org>");

	// foo^6
	CPPUNIT_ASSERT_THROW(
		parser.next(buffer),
		SyntaxException
	);

	buffer.clear();
	buffer.str("foo^666 2017-01-01T00:01:02Z tester <tester@example.org>");

	// foo^666
	CPPUNIT_ASSERT_THROW(
		parser.next(buffer),
		SyntaxException
	);

	buffer.clear();
	buffer.str("hi! 2017-01-01T00:01:02Z tester <tester@example.org>");

	// hi!
	CPPUNIT_ASSERT_THROW(
		parser.next(buffer),
		SyntaxException
	);

	buffer.clear();
	buffer.str("foo@bar 2017-01-01T00:01:02Z tester <tester@example.org>");

	// foo@bar
	CPPUNIT_ASSERT_THROW(
		parser.next(buffer),
		SyntaxException
	);

	buffer.clear();
	buffer.str("foo:bar 2017-01-01T00:01:02Z tester <tester@example.org>");

	// foo:bar
	CPPUNIT_ASSERT_THROW(
		parser.next(buffer),
		SyntaxException
	);
}

void SqitchPlanParserTest::testGoodNames()
{
	SqitchPlanParser parser;
	SqitchChange::Ptr change;

	istringstream buffer;

	buffer.clear();
	buffer.str("foo 2017-01-01T00:01:02Z tester <tester@example.org>");

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("foo", change->name());

	buffer.clear();
	buffer.str("12 2017-01-01T00:01:02Z tester <tester@example.org>");

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("12", change->name());

	buffer.clear();
	buffer.str("t 2017-01-01T00:01:02Z tester <tester@example.org>");

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("t", change->name());

	buffer.clear();
	buffer.str("6 2017-01-01T00:01:02Z tester <tester@example.org>");

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("6", change->name());

	buffer.clear();
	buffer.str("foo/bar 2017-01-01T00:01:02Z tester <tester@example.org>");

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("foo/bar", change->name());

	buffer.clear();
	buffer.str("beta1 2017-01-01T00:01:02Z tester <tester@example.org>");

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("beta1", change->name());

	buffer.clear();
	buffer.str("foo_ 2017-01-01T00:01:02Z tester <tester@example.org>");

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("foo_", change->name());

	buffer.clear();
	buffer.str("_foo 2017-01-01T00:01:02Z tester <tester@example.org>");

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("_foo", change->name());

	buffer.clear();
	buffer.str("v1.0-1b 2017-01-01T00:01:02Z tester <tester@example.org>");

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("v1.0-1b", change->name());

	buffer.clear();
	buffer.str("v1.2-1 2017-01-01T00:01:02Z tester <tester@example.org>");

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("v1.2-1", change->name());

	buffer.clear();
	buffer.str("v1.2+1 2017-01-01T00:01:02Z tester <tester@example.org>");

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("v1.2+1", change->name());

	buffer.clear();
	buffer.str("v1.2_1 2017-01-01T00:01:02Z tester <tester@example.org>");

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("v1.2_1", change->name());
}

void SqitchPlanParserTest::testReservedTags()
{
	SqitchPlanParser parser;

	istringstream buffer;

	buffer.clear();
	buffer.str("@HEAD 2017-01-01T00:01:02Z tester <tester@example.org>");

	CPPUNIT_ASSERT_THROW(
		parser.next(buffer),
		SyntaxException
	);

	buffer.clear();
	buffer.str("@ROOT 2017-01-01T00:01:02Z tester <tester@example.org>");

	CPPUNIT_ASSERT_THROW(
		parser.next(buffer),
		SyntaxException
	);
}

void SqitchPlanParserTest::testDupTag()
{
	SqitchPlanParser parser;

	istringstream buffer;

	buffer.clear();
	buffer.str(SqitchTestingPlans::DUPE_TAG);

	SqitchPragma::Ptr pragma;

	pragma = parser.next(buffer).cast<SqitchPragma>();
	CPPUNIT_ASSERT_EQUAL("project", pragma->key());
	CPPUNIT_ASSERT_EQUAL("dupe_tag", pragma->value());

	CPPUNIT_ASSERT(!parser.next(buffer).cast<SqitchChange>().isNull());
	CPPUNIT_ASSERT(!parser.next(buffer).cast<SqitchTag>().isNull());
	CPPUNIT_ASSERT(!parser.next(buffer).cast<SqitchChange>().isNull());
	CPPUNIT_ASSERT(!parser.next(buffer).cast<SqitchTag>().isNull());
	CPPUNIT_ASSERT(!parser.next(buffer).cast<SqitchTag>().isNull());

	// @bar is duplicate but we do not check for duplicates in SqitchPlanParser
	CPPUNIT_ASSERT(!parser.next(buffer).cast<SqitchTag>().isNull());
	CPPUNIT_ASSERT(!parser.next(buffer).cast<SqitchTag>().isNull());
	CPPUNIT_ASSERT(!parser.next(buffer).cast<SqitchTag>().isNull());
	CPPUNIT_ASSERT(!parser.next(buffer).cast<SqitchChange>().isNull());

	CPPUNIT_ASSERT(parser.next(buffer).isNull());
}

void SqitchPlanParserTest::testDeployRevert()
{
	SqitchPlanParser parser;

	istringstream buffer;

	buffer.clear();
	buffer.str(SqitchTestingPlans::DEPLOY_AND_REVERT);

	SqitchLine::Ptr line;
	SqitchPragma::Ptr pragma;
	SqitchChange::Ptr change;
	SqitchTag::Ptr tag;

	pragma = parser.next(buffer).cast<SqitchPragma>();
	CPPUNIT_ASSERT_EQUAL("project", pragma->key());
	CPPUNIT_ASSERT_EQUAL("deploy_and_revert", pragma->value());

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("hey", change->name());
	CPPUNIT_ASSERT_EQUAL(SqitchChange::DEPLOY, change->operation());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", change->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", change->authorEmail());

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("you", change->name());
	CPPUNIT_ASSERT_EQUAL(SqitchChange::DEPLOY, change->operation());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", change->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", change->authorEmail());

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("dr_evil", change->name());
	CPPUNIT_ASSERT_EQUAL(SqitchChange::DEPLOY, change->operation());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", change->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", change->authorEmail());

	tag = parser.next(buffer).cast<SqitchTag>();
	CPPUNIT_ASSERT_EQUAL("foo", tag->name());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", tag->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", tag->authorEmail());

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("this/rocks", change->name());
	CPPUNIT_ASSERT_EQUAL(SqitchChange::DEPLOY, change->operation());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", change->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", change->authorEmail());

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("hey-there", change->name());
	CPPUNIT_ASSERT_EQUAL(SqitchChange::DEPLOY, change->operation());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", change->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", change->authorEmail());

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("dr_evil", change->name());
	CPPUNIT_ASSERT_EQUAL(SqitchChange::REVERT, change->operation());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", change->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", change->authorEmail());

	tag = parser.next(buffer).cast<SqitchTag>();
	CPPUNIT_ASSERT_EQUAL("bar", tag->name());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", tag->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", tag->authorEmail());

	CPPUNIT_ASSERT(parser.next(buffer).isNull());
}

void SqitchPlanParserTest::testDependencies()
{
	SqitchPlanParser parser;

	istringstream buffer;

	buffer.clear();
	buffer.str(SqitchTestingPlans::DEPENDENCIES);

	SqitchLine::Ptr line;
	SqitchPragma::Ptr pragma;
	SqitchChange::Ptr change;
	SqitchTag::Ptr tag;

	pragma = parser.next(buffer).cast<SqitchPragma>();
	CPPUNIT_ASSERT_EQUAL("project", pragma->key());
	CPPUNIT_ASSERT_EQUAL("dependencies", pragma->value());

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("roles", change->name());
	CPPUNIT_ASSERT_EQUAL(SqitchChange::DEPLOY, change->operation());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", change->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", change->authorEmail());
	CPPUNIT_ASSERT(change->dependencies().empty());

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("users", change->name());
	CPPUNIT_ASSERT_EQUAL(SqitchChange::DEPLOY, change->operation());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", change->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", change->authorEmail());
	CPPUNIT_ASSERT(change->dependencies().find(SqitchRef("roles")) != change->dependencies().end());
	CPPUNIT_ASSERT_EQUAL(1, change->dependencies().size());

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("add_user", change->name());
	CPPUNIT_ASSERT_EQUAL(SqitchChange::DEPLOY, change->operation());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", change->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", change->authorEmail());
	CPPUNIT_ASSERT(change->dependencies().find(SqitchRef("users")) != change->dependencies().end());
	CPPUNIT_ASSERT(change->dependencies().find(SqitchRef("roles")) != change->dependencies().end());
	CPPUNIT_ASSERT_EQUAL(2, change->dependencies().size());

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("dr_evil", change->name());
	CPPUNIT_ASSERT_EQUAL(SqitchChange::DEPLOY, change->operation());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", change->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", change->authorEmail());

	tag = parser.next(buffer).cast<SqitchTag>();
	CPPUNIT_ASSERT_EQUAL("alpha", tag->name());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", tag->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", tag->authorEmail());

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("users", change->name());
	CPPUNIT_ASSERT_EQUAL(SqitchChange::DEPLOY, change->operation());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", change->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", change->authorEmail());
	CPPUNIT_ASSERT(change->dependencies().find(SqitchRef("users@alpha")) != change->dependencies().end());
	CPPUNIT_ASSERT_EQUAL(1, change->dependencies().size());

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("dr_evil", change->name());
	CPPUNIT_ASSERT_EQUAL(SqitchChange::REVERT, change->operation());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", change->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", change->authorEmail());

	change = parser.next(buffer).cast<SqitchChange>();
	CPPUNIT_ASSERT_EQUAL("del_user", change->name());
	CPPUNIT_ASSERT_EQUAL(SqitchChange::DEPLOY, change->operation());
	CPPUNIT_ASSERT_EQUAL("Barack Obama", change->author());
	CPPUNIT_ASSERT_EQUAL("potus@whitehouse.gov", change->authorEmail());
	CPPUNIT_ASSERT(change->conflicts().find(SqitchRef("dr_evil")) != change->dependencies().end());
	CPPUNIT_ASSERT_EQUAL(1, change->conflicts().size());
	CPPUNIT_ASSERT(change->dependencies().find(SqitchRef("users")) != change->dependencies().end());
	CPPUNIT_ASSERT_EQUAL(1, change->dependencies().size());
}

}
