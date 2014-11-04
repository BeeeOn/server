/*
 * File:   testMsgInSetAction.h
 * Author: pavel
 *
 * Created on 17.9.2014, 12:55:28
 */

#ifndef TESTMSGINSETACTION_H
#define	TESTMSGINSETACTION_H

#include <cppunit/extensions/HelperMacros.h>

#include "../testHelper.h"

class testMsgInSetAction : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInSetAction);

    CPPUNIT_TEST(testMethod);
    CPPUNIT_TEST(testFailedMethod);

    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInSetAction();
    virtual ~testMsgInSetAction();
    void setUp();
    void tearDown();

private:
    void testMethod();
    void testFailedMethod();
};

#endif	/* TESTMSGINSETACTION_H */

