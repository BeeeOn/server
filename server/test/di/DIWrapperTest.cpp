#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Logger.h>

#include "di/DIWrapper.h"
#include "cppunit/BetterAssert.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class DIWrapperTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(DIWrapperTest);
	CPPUNIT_TEST(testCreate);
	CPPUNIT_TEST(testPolymorphicBehaviour);
	CPPUNIT_TEST(testInheritanceOfTarget);
	CPPUNIT_TEST(testSetSharedPtr);
	CPPUNIT_TEST_SUITE_END();
public:
	void testCreate();
	void testPolymorphicBehaviour();
	void testInheritanceOfTarget();
	void testSetSharedPtr();
};

CPPUNIT_TEST_SUITE_REGISTRATION(DIWrapperTest);

class DITest {
public:
	void setName(const std::string &name)
	{
		m_name = name;
	}

	void setOffset(int v)
	{
		m_offset = v;
	}

	void setSelf(DITest *self)
	{
		m_self = self;
	}

	void call()
	{
		m_called = true;
	}

	std::string m_name;
	int m_offset = 0;
	DITest *m_self = NULL;
	bool m_called = false;
};

struct DITestChild : public DITest {
};

}

BEEEON_OBJECT_BEGIN(BeeeOn, DITest)
BEEEON_OBJECT_TEXT("name", &DITest::setName)
BEEEON_OBJECT_NUMBER("offset", &DITest::setOffset)
BEEEON_OBJECT_REF("self", &DITest::setSelf)
BEEEON_OBJECT_HOOK("call", &DITest::call)
BEEEON_OBJECT_END(BeeeOn, DITest)

BEEEON_OBJECT_BEGIN(BeeeOn, DITestChild)
BEEEON_OBJECT_CASTABLE(DITest)
BEEEON_OBJECT_TEXT("name", &DITestChild::setName)
BEEEON_OBJECT_NUMBER("offset", &DITestChild::setOffset)
BEEEON_OBJECT_REF("self", &DITestChild::setSelf)
BEEEON_OBJECT_HOOK("call", &DITestChild::call)
BEEEON_OBJECT_END(BeeeOn, DITestChild)

namespace BeeeOn {

struct ProtectedAccess : DIWrapper {
	using DIWrapper::injectRef;
	using DIWrapper::injectNumber;
	using DIWrapper::injectText;
	using DIWrapper::callHook;
};

#define ACCESS_CALL(instance, method) \
	((instance).*&ProtectedAccess::method)

void DIWrapperTest::testCreate()
{
	DITestDIW wrapper;

	SharedPtr<DITest> test = wrapper.instance();

	CPPUNIT_ASSERT(test->m_name.empty());
	CPPUNIT_ASSERT_EQUAL(0, test->m_offset);
	CPPUNIT_ASSERT(test->m_self == NULL);
	CPPUNIT_ASSERT(!test->m_called);

	ACCESS_CALL(wrapper, injectText)("name", "TEST NAME");
	ACCESS_CALL(wrapper, injectNumber)("offset", 16);
	ACCESS_CALL(wrapper, injectRef)("self", wrapper);
	ACCESS_CALL(wrapper, callHook)("call");

	CPPUNIT_ASSERT_EQUAL("TEST NAME", test->m_name);
	CPPUNIT_ASSERT_EQUAL(16, test->m_offset);
	CPPUNIT_ASSERT(test.get() == test->m_self);
	CPPUNIT_ASSERT(test->m_called);
}

void DIWrapperTest::testPolymorphicBehaviour()
{
	DIWrapper *w = new DITestDIW();
	AbstractDIWrapper<DITest> *wrapper =
		dynamic_cast<AbstractDIWrapper<DITest> *>(w);

	CPPUNIT_ASSERT(wrapper != NULL);

	SharedPtr<DITest> test = wrapper->instance();

	CPPUNIT_ASSERT(test->m_name.empty());
	CPPUNIT_ASSERT_EQUAL(0, test->m_offset);
	CPPUNIT_ASSERT(test->m_self == NULL);
	CPPUNIT_ASSERT(!test->m_called);

	ACCESS_CALL(*wrapper, injectText)("name", "TEST NAME2");
	ACCESS_CALL(*wrapper, injectNumber)("offset", 18);
	ACCESS_CALL(*wrapper, injectRef)("self", *w);
	ACCESS_CALL(*wrapper, callHook)("call");

	CPPUNIT_ASSERT_EQUAL("TEST NAME2", test->m_name);
	CPPUNIT_ASSERT_EQUAL(18, test->m_offset);
	CPPUNIT_ASSERT(test.get() == test->m_self);
	CPPUNIT_ASSERT(test->m_called);

	delete w;
}

void DIWrapperTest::testInheritanceOfTarget()
{
	DITestChildDIW wrapper;

	SharedPtr<DITestChild> test = wrapper.instance();

	CPPUNIT_ASSERT(test->m_name.empty());
	CPPUNIT_ASSERT_EQUAL(0, test->m_offset);
	CPPUNIT_ASSERT(test->m_self == NULL);
	CPPUNIT_ASSERT(!test->m_called);

	ACCESS_CALL(wrapper, injectText)("name", "TEST NAME3");
	ACCESS_CALL(wrapper, injectNumber)("offset", 19);
	ACCESS_CALL(wrapper, injectRef)("self", wrapper);
	ACCESS_CALL(wrapper, callHook)("call");

	CPPUNIT_ASSERT_EQUAL("TEST NAME3", test->m_name);
	CPPUNIT_ASSERT_EQUAL(19, test->m_offset);
	CPPUNIT_ASSERT(test.get() == test->m_self);
	CPPUNIT_ASSERT(test->m_called);
}

class DIAnother {
public:
	void setTest(SharedPtr<DITest> test)
	{
		m_test = test;
	}

	SharedPtr<DITest> m_test;
};

}

BEEEON_OBJECT_BEGIN(BeeeOn, DIAnother)
BEEEON_OBJECT_REF("test", &DIAnother::setTest)
BEEEON_OBJECT_END(BeeeOn, DIAnother)

namespace BeeeOn {

void DIWrapperTest::testSetSharedPtr()
{
	DIAnotherDIW wrapper;

	SharedPtr<DIAnother> another = wrapper.instance();

	CPPUNIT_ASSERT(another->m_test.isNull());

	DITestDIW test;
	ACCESS_CALL(wrapper, injectRef)("test", test);
	CPPUNIT_ASSERT_EQUAL(test.instance(), another->m_test);

	DITestChildDIW childTest;
	ACCESS_CALL(wrapper, injectRef)("test", childTest);

	DITest *rawTest = another->m_test.get();
	CPPUNIT_ASSERT(childTest.instance().get() == rawTest);
}

}
