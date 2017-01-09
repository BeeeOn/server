#include <Poco/AutoPtr.h>
#include <Poco/Util/XMLConfiguration.h>

#include <cppunit/extensions/HelperMacros.h>

#include "di/InjectorTarget.h"
#include "di/DependencyInjector.h"
#include "Debug.h"

using namespace std;
using namespace Poco;
using namespace Poco::Util;

namespace BeeeOn {

class DependencyInjectorTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(DependencyInjectorTest);
	CPPUNIT_TEST(testDemangle);
	CPPUNIT_TEST(testSimple);
	CPPUNIT_TEST(testAlias);
	CPPUNIT_TEST(testAliasLoop);
	CPPUNIT_TEST(testAliasToAliasFails);
	CPPUNIT_TEST(testExternalVariables);
	CPPUNIT_TEST(testEarly);
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
	void testAlias();
	void testAliasLoop();
	void testAliasToAliasFails();
	void testExternalVariables();
	void testEarly();

private:
	AutoPtr<XMLConfiguration> m_config;
};

CPPUNIT_TEST_SUITE_REGISTRATION(DependencyInjectorTest);

class FakeObject : public AbstractInjectorTarget {
public:
	FakeObject():
		m_self(NULL),
		m_index(0)
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

BEEEON_OBJECT(FakeObject, BeeeOn::FakeObject)

void DependencyInjectorTest::setUp()
{
	m_config->loadEmpty("empty");
	m_config->setString("alias[1][@name]", "simpleAlias");
	m_config->setString("alias[1][@ref]", "simple");
	m_config->setString("alias[2][@name]", "secondAlias");
	m_config->setString("alias[2][@ref]", "secondAlias");
	m_config->setString("alias[3][@name]", "aliasToAlias");
	m_config->setString("alias[3][@ref]", "simpleAlias");
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
	m_config->setString("instance[3][@name]", "earlyInit0");
	m_config->setString("instance[3][@class]", "BeeeOn::FakeObject");
	m_config->setString("instance[3][@init]", "early");
	m_config->setString("instance[3].set[1][@name]", "name");
	m_config->setString("instance[3].set[1][@text]", "early0");
	m_config->setString("instance[4][@name]", "earlyInit1");
	m_config->setString("instance[4][@class]", "BeeeOn::FakeObject");
	m_config->setString("instance[4][@init]", "early");
	m_config->setString("instance[4].set[1][@name]", "name");
	m_config->setString("instance[4].set[1][@text]", "early1");
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

	FakeObject *fake = injector.find<FakeObject>("simple");
	CPPUNIT_ASSERT(fake == NULL);

	fake = injector.create<FakeObject>("simple");

	CPPUNIT_ASSERT(fake != NULL);
	CPPUNIT_ASSERT(fake->m_self == fake);
	CPPUNIT_ASSERT(fake->m_name.compare("fake") == 0);
	CPPUNIT_ASSERT(fake->m_index == 5);
}

/**
 * Test refering to an alias pointing to an instance.
 * An alias points to an instance so it just uses a different name.
 */
void DependencyInjectorTest::testAlias()
{
	DependencyInjector injector(m_config);

	FakeObject *fakeAlias = injector.find<FakeObject>("simpleAlias");
	CPPUNIT_ASSERT(fakeAlias == NULL);

	fakeAlias = injector.create<FakeObject>("simpleAlias");
	CPPUNIT_ASSERT(fakeAlias != NULL);

	FakeObject *fake = injector.find<FakeObject>("simple");
	CPPUNIT_ASSERT(fake != NULL);

	FakeObject *fakeCreated = injector.create<FakeObject>("simple");
	CPPUNIT_ASSERT(fakeCreated != NULL);

	CPPUNIT_ASSERT(fake == fakeAlias);
	CPPUNIT_ASSERT(fake == fakeCreated);
}

/**
 * Test alias points to itself throws an exception.
 */
void DependencyInjectorTest::testAliasLoop()
{
	DependencyInjector injector(m_config);

	CPPUNIT_ASSERT_THROW(injector.create<FakeObject>("secondAlias"),
			IllegalStateException);
}

/**
 * Alias cannot point to an alias.
 * The NotFoundException is thrown because the DependencyInjector
 * does not search the alias namespace recursively.
 */
void DependencyInjectorTest::testAliasToAliasFails()
{
	DependencyInjector injector(m_config);

	CPPUNIT_ASSERT_THROW(injector.create<FakeObject>("aliasToAlias"),
			NotFoundException);
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

/**
 * Test whether the init="early" instances are created automatically
 * during DependencyInjector construction.
 */
void DependencyInjectorTest::testEarly()
{
	DependencyInjector injector(m_config);

	FakeObject *early0 = injector.find<FakeObject>("earlyInit0");
	CPPUNIT_ASSERT(early0 != NULL);

	FakeObject *early1 = injector.find<FakeObject>("earlyInit1");
	CPPUNIT_ASSERT(early1 != NULL);

	CPPUNIT_ASSERT(early0 != early1);
}

}
