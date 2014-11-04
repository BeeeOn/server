/*
 * File:   testMsgInDeleteCondition.h
 * Author: pavel
 *
 * Created on 16.9.2014, 16:15:02
 */

#ifndef TESTMSGINDELETECONDITION_H
#define	TESTMSGINDELETECONDITION_H

#include <cppunit/extensions/HelperMacros.h>

#include "../testHelper.h"

class testMsgInDeleteCondition : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInDeleteCondition);

    CPPUNIT_TEST(testMethod);
    CPPUNIT_TEST(testFailedMethod);

    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInDeleteCondition();
    virtual ~testMsgInDeleteCondition();
    void setUp();
    void tearDown();

private:
    void testMethod();
    void testFailedMethod();
};

#endif	/* TESTMSGINDELETECONDITION_H */

