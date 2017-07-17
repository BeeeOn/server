#include <sstream>
#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Exception.h>

#include "cppunit/BetterAssert.h"
#include "sqitch/SqitchChange.h"
#include "sqitch/SqitchPragma.h"
#include "sqitch/SqitchTag.h"
#include "sqitch/SqitchPlan.h"

#include "sqitch/SqitchTestingPlans.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class SqitchPlanTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(SqitchPlanTest);
	CPPUNIT_TEST(testEmpty);
	CPPUNIT_TEST(testSimple);
	CPPUNIT_TEST(testMulti);
	CPPUNIT_TEST(testDupTag);
	CPPUNIT_TEST(testDeployRevert);
	CPPUNIT_TEST(testDependencies);
	CPPUNIT_TEST_SUITE_END();

public:
	void testEmpty();
	void testSimple();
	void testMulti();
	void testChangesOnly();
	void testDupTag();
	void testDeployRevert();
	void testDependencies();
};

CPPUNIT_TEST_SUITE_REGISTRATION(SqitchPlanTest);

void SqitchPlanTest::testEmpty()
{
	SqitchPlan plan;

	istringstream buffer;
	buffer.str("");

	CPPUNIT_ASSERT_NO_THROW(
		plan.load(buffer)
	);

	CPPUNIT_ASSERT(plan.begin() == plan.end());
	CPPUNIT_ASSERT(plan.rbegin() == plan.rend());
	CPPUNIT_ASSERT(plan.rfind(SqitchRef("any")) == plan.rend());
	CPPUNIT_ASSERT(plan.rfindTag("any") == plan.rend());
}

void SqitchPlanTest::testSimple()
{
	SqitchPlan plan;

	istringstream buffer;
	buffer.str(SqitchTestingPlans::WIDGETS);

	CPPUNIT_ASSERT_NO_THROW(
		plan.load(buffer);
	);

	CPPUNIT_ASSERT_EQUAL("widgets", plan.project());
	CPPUNIT_ASSERT(plan.begin() != plan.end());
	CPPUNIT_ASSERT(plan.rbegin() != plan.rend());

	SqitchChange::Ptr change;
	SqitchTag::Ptr tag;

	// change non-existing does not exist in the plan
	CPPUNIT_ASSERT(plan.rfind(SqitchRef("non-existing")) == plan.rend());
	// looking up for change of another plan should fail
	CPPUNIT_ASSERT(plan.rfind(SqitchRef("second_project:hey")) == plan.rend());
	// hey under tag new-version does not exist
	CPPUNIT_ASSERT(plan.rfind(SqitchRef("hey@new-version")) == plan.rend());

	// no such change foo
	auto it = plan.rfind(SqitchRef("foo"));
	CPPUNIT_ASSERT(it == plan.rend());

	// there is a tag @foo
	tag = plan.rfindTag("foo")->cast<SqitchTag>();
	CPPUNIT_ASSERT(!tag.isNull());
	CPPUNIT_ASSERT_EQUAL("foo", tag->name());

	// there is a change hey
	change = plan.rfind(SqitchRef("hey"))->cast<SqitchChange>();
	CPPUNIT_ASSERT(!change.isNull());
	CPPUNIT_ASSERT_EQUAL("hey", change->name());

	// there is a change hey before tag @foo
	change = plan.rfind(SqitchRef("hey@foo"))->cast<SqitchChange>();
	CPPUNIT_ASSERT(!change.isNull());
	CPPUNIT_ASSERT_EQUAL("hey", change->name());

	// there is a change hey before tag @foo in this project
	change = plan.rfind(SqitchRef("widgets:hey@foo"))->cast<SqitchChange>();
	CPPUNIT_ASSERT(!change.isNull());
	CPPUNIT_ASSERT_EQUAL("hey", change->name());

	// there is a change you
	change = plan.rfind(SqitchRef("you"))->cast<SqitchChange>();
	CPPUNIT_ASSERT(!change.isNull());
	CPPUNIT_ASSERT_EQUAL("you", change->name());

	// there is a change you before tag @foo
	change = plan.rfind(SqitchRef("you@foo"))->cast<SqitchChange>();
	CPPUNIT_ASSERT(!change.isNull());
	CPPUNIT_ASSERT_EQUAL("you", change->name());

	// there is a change you before tag @foo in this project
	change = plan.rfind(SqitchRef("widgets:you@foo"))->cast<SqitchChange>();
	CPPUNIT_ASSERT(!change.isNull());
	CPPUNIT_ASSERT_EQUAL("you", change->name());
}

void SqitchPlanTest::testMulti()
{
	SqitchPlan plan;

	istringstream buffer;
	buffer.str(SqitchTestingPlans::MULTI);

	CPPUNIT_ASSERT_NO_THROW(
		plan.load(buffer);
	);

	CPPUNIT_ASSERT_EQUAL("multi", plan.project());
	CPPUNIT_ASSERT(plan.begin() != plan.end());
	CPPUNIT_ASSERT(plan.rbegin() != plan.rend());

	SqitchChange::Ptr change;
	SqitchTag::Ptr tag;

	list<SqitchLine::Ptr>::const_reverse_iterator nonVersioned;
	list<SqitchLine::Ptr>::const_reverse_iterator bazVersion;
	list<SqitchLine::Ptr>::const_reverse_iterator barVersion;
	list<SqitchLine::Ptr>::const_reverse_iterator fooVersion;

	nonVersioned = plan.rfind(SqitchRef("hey"));
	CPPUNIT_ASSERT(nonVersioned != plan.rend());
	change = nonVersioned->cast<SqitchChange>();
	CPPUNIT_ASSERT(!change.isNull());
	CPPUNIT_ASSERT_EQUAL("hey", change->name());

	bazVersion = plan.rfind(SqitchRef("hey@baz"));
	CPPUNIT_ASSERT(bazVersion != plan.rend());
	change = bazVersion->cast<SqitchChange>();
	CPPUNIT_ASSERT(!change.isNull());
	CPPUNIT_ASSERT_EQUAL("hey", change->name());

	barVersion = plan.rfind(SqitchRef("hey@bar"));
	CPPUNIT_ASSERT(barVersion != plan.rend());
	change = barVersion->cast<SqitchChange>();
	CPPUNIT_ASSERT(!change.isNull());
	CPPUNIT_ASSERT_EQUAL("hey", change->name());

	fooVersion = plan.rfind(SqitchRef("hey@foo"));
	CPPUNIT_ASSERT(fooVersion != plan.rend());
	change = fooVersion->cast<SqitchChange>();
	CPPUNIT_ASSERT(!change.isNull());
	CPPUNIT_ASSERT_EQUAL("hey", change->name());

	CPPUNIT_ASSERT(nonVersioned == bazVersion);
	CPPUNIT_ASSERT(bazVersion == barVersion);
	CPPUNIT_ASSERT(barVersion == fooVersion);
}

void SqitchPlanTest::testDupTag()
{
	SqitchPlan plan;

	istringstream buffer;
	buffer.str(SqitchTestingPlans::DUPE_TAG);

	CPPUNIT_ASSERT_THROW(
		plan.load(buffer),
		ExistsException
	);

	CPPUNIT_ASSERT_EQUAL("dupe_tag", plan.project());
	list<SqitchLine::Ptr>::const_iterator it;

	it = plan.begin();

	CPPUNIT_ASSERT(it != plan.end());
	CPPUNIT_ASSERT(!it->cast<SqitchChange>().isNull());
	CPPUNIT_ASSERT_EQUAL("whatever", it->cast<SqitchChange>()->name());
	++it;

	CPPUNIT_ASSERT(it != plan.end());
	CPPUNIT_ASSERT(!it->cast<SqitchTag>().isNull());
	CPPUNIT_ASSERT_EQUAL("foo", it->cast<SqitchTag>()->name());
	++it;

	CPPUNIT_ASSERT(it != plan.end());
	CPPUNIT_ASSERT(!it->cast<SqitchChange>().isNull());
	CPPUNIT_ASSERT_EQUAL("hi", it->cast<SqitchChange>()->name());
	++it;

	CPPUNIT_ASSERT(it != plan.end());
	CPPUNIT_ASSERT(!it->cast<SqitchTag>().isNull());
	CPPUNIT_ASSERT_EQUAL("bar", it->cast<SqitchTag>()->name());
	++it;

	CPPUNIT_ASSERT(it != plan.end());
	CPPUNIT_ASSERT(!it->cast<SqitchTag>().isNull());
	CPPUNIT_ASSERT_EQUAL("stink", it->cast<SqitchTag>()->name());
	++it;

	CPPUNIT_ASSERT(it != plan.end());
	CPPUNIT_ASSERT(!it->cast<SqitchTag>().isNull());
	CPPUNIT_ASSERT_EQUAL("blah", it->cast<SqitchTag>()->name());
	++it;

	CPPUNIT_ASSERT(it == plan.end());
}

void SqitchPlanTest::testDeployRevert()
{
	SqitchPlan plan;

	istringstream buffer;

	buffer.clear();
	buffer.str(SqitchTestingPlans::DEPLOY_AND_REVERT);

	CPPUNIT_ASSERT_NO_THROW(
		plan.load(buffer);
	);

	CPPUNIT_ASSERT_EQUAL("deploy_and_revert", plan.project());
	auto deploy = plan.rfind(SqitchRef("dr_evil@foo"));
	auto revert = plan.rfind(SqitchRef("dr_evil"));

	CPPUNIT_ASSERT(deploy != plan.rend());
	CPPUNIT_ASSERT(!deploy->cast<SqitchChange>().isNull());
	CPPUNIT_ASSERT_EQUAL(
		SqitchChange::DEPLOY,
		deploy->cast<SqitchChange>()->operation()
	);

	CPPUNIT_ASSERT(revert != plan.rend());
	CPPUNIT_ASSERT_EQUAL(
		SqitchChange::REVERT,
		revert->cast<SqitchChange>()->operation()
	);

	CPPUNIT_ASSERT(deploy != revert);
}

void SqitchPlanTest::testDependencies()
{
	SqitchPlan plan;

	istringstream buffer;

	buffer.clear();
	buffer.str(SqitchTestingPlans::DEPENDENCIES);

	CPPUNIT_ASSERT_NO_THROW(
		plan.load(buffer);
	);

	CPPUNIT_ASSERT_EQUAL("dependencies", plan.project());
}

}
