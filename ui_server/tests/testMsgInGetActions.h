/*
 * File:   testMsgInGetActions.h
 * Author: pavel
 *
 * Created on 17.9.2014, 14:03:05
 */

#ifndef TESTMSGINGETACTIONS_H
#define	TESTMSGINGETACTIONS_H

#include <cppunit/extensions/HelperMacros.h>

#include "../testHelper.h"

class testMsgInGetActions : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInGetActions);

    CPPUNIT_TEST(testMethod);
    CPPUNIT_TEST(testFailedMethod);

    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInGetActions();
    virtual ~testMsgInGetActions();
    void setUp();
    void tearDown();

private:
    void testMethod();
    void testFailedMethod();
};

#endif	/* TESTMSGINGETACTIONS_H */

