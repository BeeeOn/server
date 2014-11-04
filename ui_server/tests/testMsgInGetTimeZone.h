/*
 * File:   testMsgInGetTimeZone.h
 * Author: pavel
 *
 * Created on 9.8.2014, 11:07:39
 */

#ifndef TESTMSGINGETTIMEZONE_H
#define	TESTMSGINGETTIMEZONE_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"

class testMsgInGetTimeZone : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInGetTimeZone);

    CPPUNIT_TEST(testGetUTC);

    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInGetTimeZone();
    virtual ~testMsgInGetTimeZone();
    void setUp();
    void tearDown();

private:
    void testGetUTC();
};

#endif	/* TESTMSGINGETTIMEZONE_H */

