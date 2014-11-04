/*
 * File:   testMsgInPartial.h
 * Author: pavel
 *
 * Created on 9.8.2014, 14:45:57
 */

#ifndef TESTMSGINPARTIAL_H
#define	TESTMSGINPARTIAL_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"

class testMsgInPartial : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInPartial);

    CPPUNIT_TEST(testTwoDevices);
    CPPUNIT_TEST(testEmptyMsg);
    CPPUNIT_TEST(testWrongDevice);
    
    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInPartial();
    virtual ~testMsgInPartial();
    void setUp();
    void tearDown();

private:
    void testTwoDevices();
    void testEmptyMsg();
    void testWrongDevice();
};

#endif	/* TESTMSGINPARTIAL_H */

