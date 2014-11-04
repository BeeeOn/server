/*
 * File:   testMsgInSetCondition.h
 * Author: pavel
 *
 * Created on 16.9.2014, 10:27:02
 */

#ifndef TESTMSGINSETCONDITION_H
#define	TESTMSGINSETCONDITION_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"

class testMsgInSetCondition : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInSetCondition);

    CPPUNIT_TEST(testCond);
    CPPUNIT_TEST(testEmptyCond);

    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInSetCondition();
    virtual ~testMsgInSetCondition();
    void setUp();
    void tearDown();

private:
    void testCond();
    void testEmptyCond();
};

#endif	/* TESTMSGINSETCONDITION_H */

