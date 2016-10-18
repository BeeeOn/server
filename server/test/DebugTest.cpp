#include <iostream>
#include <cppunit/extensions/HelperMacros.h>

#include "Debug.h"

using namespace Poco;

namespace BeeeOn {

class DebugTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(DebugTest);
	CPPUNIT_TEST(testClassDemangle);
	CPPUNIT_TEST_SUITE_END();
public:
	void testClassDemangle();
};

CPPUNIT_TEST_SUITE_REGISTRATION(DebugTest);

class A {
public:
	virtual ~A() {}
};

class B {
public:
};

class C : public A {
public:
};

class D : public B {
public:
};

class E : public A, public B {
public:
};

void DebugTest::testClassDemangle()
{
	A a;
	B b;
	C c;
	D d;
	E e;

	A &cAsA = c;
	B &dAsB = d;
	A &eAsA = e;
	B &eAsB = e;

	CPPUNIT_ASSERT("BeeeOn::A" == classDemangle(typeid(a).name()));
	CPPUNIT_ASSERT("BeeeOn::B" == classDemangle(typeid(b).name()));
	CPPUNIT_ASSERT("BeeeOn::C" == classDemangle(typeid(c).name()));
	CPPUNIT_ASSERT("BeeeOn::D" == classDemangle(typeid(d).name()));
	CPPUNIT_ASSERT("BeeeOn::E" == classDemangle(typeid(e).name()));
	CPPUNIT_ASSERT("BeeeOn::C" == classDemangle(typeid(cAsA).name()));
	CPPUNIT_ASSERT("BeeeOn::B" == classDemangle(typeid(dAsB).name()));
	CPPUNIT_ASSERT("BeeeOn::E" == classDemangle(typeid(eAsA).name()));
	CPPUNIT_ASSERT("BeeeOn::B" == classDemangle(typeid(eAsB).name()));

	CPPUNIT_ASSERT("BeeeOn::A" == classDemangle(typeid(&a).name()));
	CPPUNIT_ASSERT("BeeeOn::B" == classDemangle(typeid(&b).name()));
	CPPUNIT_ASSERT("BeeeOn::C" == classDemangle(typeid(&c).name()));
	CPPUNIT_ASSERT("BeeeOn::D" == classDemangle(typeid(&d).name()));
	CPPUNIT_ASSERT("BeeeOn::E" == classDemangle(typeid(&e).name()));
	CPPUNIT_ASSERT("BeeeOn::A" == classDemangle(typeid(&cAsA).name()));
	CPPUNIT_ASSERT("BeeeOn::B" == classDemangle(typeid(&dAsB).name()));
	CPPUNIT_ASSERT("BeeeOn::A" == classDemangle(typeid(&eAsA).name()));
	CPPUNIT_ASSERT("BeeeOn::B" == classDemangle(typeid(&eAsB).name()));
}

}
