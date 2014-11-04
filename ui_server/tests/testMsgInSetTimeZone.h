/*
 * File:   testMsgInSetTimeZone.h
 * Author: pavel
 *
 * Created on 9.8.2014, 11:00:14
 */

#ifndef TESTMSGINSETTIMEZONE_H
#define	TESTMSGINSETTIMEZONE_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"

class testMsgInSetTimeZone : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInSetTimeZone);

    CPPUNIT_TEST(testSetUTC);
    CPPUNIT_TEST(testWrongUTC);

    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInSetTimeZone();
    virtual ~testMsgInSetTimeZone();
    void setUp();
    void tearDown();

private:
    void testSetUTC();
    void testWrongUTC();
};

#endif	/* TESTMSGINSETTIMEZONE_H */

