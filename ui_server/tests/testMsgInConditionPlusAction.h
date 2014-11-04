/*
 * File:   testMsgInConditionPlusAction.h
 * Author: pavel
 *
 * Created on 17.9.2014, 16:17:49
 */

#ifndef TESTMSGINCONDITIONPLUSACTION_H
#define	TESTMSGINCONDITIONPLUSACTION_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"
class testMsgInConditionPlusAction : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInConditionPlusAction);

    CPPUNIT_TEST(testMethod);
    CPPUNIT_TEST(testFailedMethod);

    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInConditionPlusAction();
    virtual ~testMsgInConditionPlusAction();
    void setUp();
    void tearDown();

private:
    void testMethod();
    void testFailedMethod();
};

#endif	/* TESTMSGINCONDITIONPLUSACTION_H */

