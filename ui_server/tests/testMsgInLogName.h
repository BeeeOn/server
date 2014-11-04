/*
 * File:   testMsgInLogName.h
 * Author: pavel
 *
 * Created on 6.8.2014, 11:24:51
 */

#ifndef TESTMSGINLOGNAME_H
#define	TESTMSGINLOGNAME_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"

class testMsgInLogName : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInLogName);

    CPPUNIT_TEST(testEmptyLogsInterval);
    CPPUNIT_TEST(testEmptyLogsRAW);
    CPPUNIT_TEST(testLogsInterval);
    CPPUNIT_TEST(testLogsInterval2);
    CPPUNIT_TEST(testLogsRAW);
    CPPUNIT_TEST(testWrongDevice);
    
    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInLogName();
    virtual ~testMsgInLogName();
    void setUp();
    void tearDown();

private:
    void testEmptyLogsInterval();
    void testEmptyLogsRAW();
    void testLogsInterval();
    void testLogsInterval2();
    void testLogsRAW();
    void testWrongDevice();
};

#endif	/* TESTMSGINLOGNAME_H */

