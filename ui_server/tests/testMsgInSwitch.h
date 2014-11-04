/*
 * File:   testMsgInSwitch.h
 * Author: pavel
 *
 * Created on 6.8.2014, 10:06:55
 */

#ifndef TESTMSGINSWITCH_H
#define	TESTMSGINSWITCH_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"

class testMsgInSwitch : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInSwitch);

    CPPUNIT_TEST(testMethod);
    CPPUNIT_TEST(testFailedMethod);

    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInSwitch();
    virtual ~testMsgInSwitch();
    void setUp();
    void tearDown();

private:
    void testMethod();
    void testFailedMethod();
};

#endif	/* TESTMSGINSWITCH_H */

