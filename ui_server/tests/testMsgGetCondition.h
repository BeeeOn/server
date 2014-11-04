/*
 * File:   testMsgGetCondition.h
 * Author: pavel
 *
 * Created on 16.9.2014, 11:58:47
 */

#ifndef TESTMSGGETCONDITION_H
#define	TESTMSGGETCONDITION_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"

#include "msgInGetCondition.h"
class testMsgGetCondition : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgGetCondition);

    CPPUNIT_TEST(testMethod);
    CPPUNIT_TEST(testFailedMethod);

    CPPUNIT_TEST_SUITE_END();

public:
    testMsgGetCondition();
    virtual ~testMsgGetCondition();
    void setUp();
    void tearDown();

private:
    void testMethod();
    void testFailedMethod();
};

#endif	/* TESTMSGGETCONDITION_H */

