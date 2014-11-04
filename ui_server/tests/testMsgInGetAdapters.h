/*
 * File:   testMsgInGetAdapters.h
 * Author: pavel
 *
 * Created on 9.8.2014, 14:18:28
 */

#ifndef TESTMSGINGETADAPTERS_H
#define	TESTMSGINGETADAPTERS_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"

class testMsgInGetAdapters : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInGetAdapters);

    CPPUNIT_TEST(testOneSU);
    CPPUNIT_TEST(testTwoSU);
    CPPUNIT_TEST(testAdminOnAdapter);
    CPPUNIT_TEST(testWithoutAdapters);
    
    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInGetAdapters();
    virtual ~testMsgInGetAdapters();
    void setUp();
    void tearDown();

private:
    void testOneSU();
    void testTwoSU();
    void testAdminOnAdapter();
    void testWithoutAdapters();
};

#endif	/* TESTMSGINGETADAPTERS_H */

