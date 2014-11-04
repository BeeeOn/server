/*
 * File:   testMsgInGetAction.h
 * Author: pavel
 *
 * Created on 17.9.2014, 14:51:54
 */

#ifndef TESTMSGINGETACTION_H
#define	TESTMSGINGETACTION_H

#include <cppunit/extensions/HelperMacros.h>

#include "../testHelper.h"

class testMsgInGetAction : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInGetAction);

    CPPUNIT_TEST(testMethod);
    CPPUNIT_TEST(testFailedMethod);

    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInGetAction();
    virtual ~testMsgInGetAction();
    void setUp();
    void tearDown();

private:
    void testMethod();
    void testFailedMethod();
};

#endif	/* TESTMSGINGETACTION_H */

