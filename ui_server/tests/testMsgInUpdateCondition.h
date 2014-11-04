/*
 * File:   testMsgInUpdateCondition.h
 * Author: pavel
 *
 * Created on 16.9.2014, 15:58:09
 */

#ifndef TESTMSGINUPDATECONDITION_H
#define	TESTMSGINUPDATECONDITION_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"

class testMsgInUpdateCondition : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInUpdateCondition);

    CPPUNIT_TEST(testMethod);
    CPPUNIT_TEST(testFailedMethod);

    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInUpdateCondition();
    virtual ~testMsgInUpdateCondition();
    void setUp();
    void tearDown();

private:
    void testMethod();
    void testFailedMethod();
};

#endif	/* TESTMSGINUPDATECONDITION_H */

