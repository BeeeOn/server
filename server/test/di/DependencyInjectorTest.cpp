#include <Poco/AutoPtr.h>
#include <Poco/Util/XMLConfiguration.h>

#include <cppunit/extensions/HelperMacros.h>

#include "di/InjectorTarget.h"
#include "di/DependencyInjector.h"

using namespace std;
using namespace Poco;
using namespace Poco::Util;

namespace BeeeOn {

class DependencyInjectorTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(DependencyInjectorTest);
	CPPUNIT_TEST(testDemangle);
	CPPUNIT_TEST(testSimple);
	CPPUNIT_TEST(testExternalVariables);
	CPPUNIT_TEST_SUITE_END();

public:
	DependencyInjectorTest():
		m_config(new XMLConfiguration())
	{
	}

	void setUp();
	void tearDown();
	void testDemangle();
	void testSimple();
	void testExternalVariables();

private:
	AutoPtr<XMLConfiguration> m_config;
};

CPPUNIT_TEST_SUITE_REGISTRATION(DependencyInjectorTest);

class FakeObject : public AbstractInjectorTarget {
public:
	FakeObject()
	{
		injector<FakeObject, FakeObject>("self", &FakeObject::setSelf);
		textInjector("name", (TextSetter) &FakeObject::setName);
		numberInjector("index", (NumberSetter) &FakeObject::setIndex);
		textInjector("other", (TextSetter) &FakeObject::setOther);
	}

	void setSelf(FakeObject *self)
	{
		m_self = self;
	}

	void setName(const string &name)
	{
		m_name = name;
	}

	void setOther(const string &other)
	{
		m_other = other;
	}

	void setIndex(int n)
	{
		m_index = n;
	}
public:
	FakeObject *m_self;
	string m_name;
	string m_other;
	int m_index;
};

BEEEON_OBJECT(FakeObject, BeeeOn::FakeObject);

void DependencyInjectorTest::setUp()
{
	m_config->loadEmpty("empty");
	m_config->setString("instance[1][@name]", "simple");
	m_config->setString("instance[1][@class]", "BeeeOn::FakeObject");
	m_config->setString("instance[1].set[1][@name]", "self");
	m_config->setString("instance[1].set[1][@ref]", "simple");
	m_config->setString("instance[1].set[2][@name]", "name");
	m_config->setString("instance[1].set[2][@text]", "fake");
	m_config->setString("instance[1].set[3][@name]", "index");
	m_config->setString("instance[1].set[3][@number]", "5");
	m_config->setString("instance[2][@name]", "variable");
	m_config->setString("instance[2][@class]", "BeeeOn::FakeObject");
	m_config->setString("instance[2].set[1][@name]", "name");
	m_config->setString("instance[2].set[1][@text]", "${FakeText}");
	m_config->setString("instance[2].set[2][@name]", "index");
	m_config->setString("instance[2].set[2][@number]", "${FakeNumber}");
	m_config->setString("instance[2].set[3][@name]", "other");
	m_config->setString("instance[2].set[3][@text]", "${NotExisting}");
	m_config->setString("FakeText", "any string");
	m_config->setInt("FakeNumber", 42);

	if (Logger::get("Test").debug())
		m_config->save(cerr);
}

void DependencyInjectorTest::tearDown()
{
}

/**
 * Make sure that classDemangle function works as expected and so
 * we are able to perform dependency injection correctly.
 */
void DependencyInjectorTest::testDemangle()
{
	DependencyInjector injector(m_config);
	const string diname(classDemangle(typeid(injector).name()));

	CPPUNIT_ASSERT(diname.compare("BeeeOn::DependencyInjector") == 0);

	FakeObject *fake;
	const string fakename(classDemangle(typeid(fake).name()));
	CPPUNIT_ASSERT(fakename.compare("BeeeOn::FakeObject") == 0);
}

/**
 * Simple test of the most important properties of DependencyInjector.
 * It creates an instance of the FakeObject and sets ref, text and
 * number on it.
 */
void DependencyInjectorTest::testSimple()
{
	DependencyInjector injector(m_config);

	FakeObject *fake = injector.create<FakeObject>("simple");

	CPPUNIT_ASSERT(fake != NULL);
	CPPUNIT_ASSERT(fake->m_self == fake);
	CPPUNIT_ASSERT(fake->m_name.compare("fake") == 0);
	CPPUNIT_ASSERT(fake->m_index == 5);
}

/**
 * Test that ${VAR} constructs are expanded when constructing objects.
 */
void DependencyInjectorTest::testExternalVariables()
{
	DependencyInjector injector(m_config);

	FakeObject *fake = injector.create<FakeObject>("variable");

	CPPUNIT_ASSERT(fake != NULL);
	CPPUNIT_ASSERT(fake->m_name.compare("any string") == 0);
	CPPUNIT_ASSERT(fake->m_other.compare("${NotExisting}") == 0);
	CPPUNIT_ASSERT(fake->m_index == 42);
}

}
