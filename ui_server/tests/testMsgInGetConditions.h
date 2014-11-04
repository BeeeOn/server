/*
 * File:   testMsgInGetConditions.h
 * Author: pavel
 *
 * Created on 16.9.2014, 15:35:15
 */

#ifndef TESTMSGINGETCONDITIONS_H
#define	TESTMSGINGETCONDITIONS_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"

class testMsgInGetConditions : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInGetConditions);

    CPPUNIT_TEST(testMethod);
    CPPUNIT_TEST(testFailedMethod);

    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInGetConditions();
    virtual ~testMsgInGetConditions();
    void setUp();
    void tearDown();

private:
    void testMethod();
    void testFailedMethod();
};

#endif	/* TESTMSGINGETCONDITIONS_H */

