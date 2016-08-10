#include <cppunit/extensions/HelperMacros.h>

#include "dao/SociManager.h"

using namespace std;
using namespace soci;

namespace BeeeOn {

class SociManagerTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(SociManagerTest);
	CPPUNIT_TEST(testSelect);
	CPPUNIT_TEST(testSelectEmpty);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testSelect();
	void testSelectEmpty();

private:
	SociManager *m_manager;
};

CPPUNIT_TEST_SUITE_REGISTRATION(SociManagerTest);

void SociManagerTest::setUp()
{
	m_manager = new SociManager();
	m_manager->setBackend("sqlite3");
	m_manager->setConnector(":memory:");
	m_manager->setPoolSize(1);

	session db(m_manager->pool());
	db << "create table test ("
		<< "  id integer primary key,"
		<< "  name varchar(200),"
		<< "  valid boolean"
		<< ");";
	db << "insert into test (id, name, valid) values (1, 'first', 1)";
}

void SociManagerTest::tearDown()
{
	delete m_manager;
}

void SociManagerTest::testSelect()
{
	session db(m_manager->pool());
	row r;

	db << "select * from test where id = 1", into(r);

	CPPUNIT_ASSERT_EQUAL(1, r.get<int>("id"));
	CPPUNIT_ASSERT(r.get<string>("name").compare("first") == 0);
	CPPUNIT_ASSERT(r.get<int>("valid"));
}

void SociManagerTest::testSelectEmpty()
{
	session db(m_manager->pool());

	rowset<row> rs = db.prepare << "select * from test where id = 2;";
	CPPUNIT_ASSERT(rs.begin() == rs.end());
}

}
