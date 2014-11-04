/*
 * File:   testMsgInUpdate.h
 * Author: pavel
 *
 * Created on 9.8.2014, 10:35:39
 */

#ifndef TESTMSGINUPDATE_H
#define	TESTMSGINUPDATE_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"

class testMsgInUpdate : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInUpdate);

    CPPUNIT_TEST(testOneDevice);
    CPPUNIT_TEST(testNoDevice);
    CPPUNIT_TEST(testTwoDevice);
    CPPUNIT_TEST(testWrongDevice);

    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInUpdate();
    virtual ~testMsgInUpdate();
    void setUp();
    void tearDown();

private:
    void testOneDevice();
    void testNoDevice();
    void testTwoDevice();
    void testWrongDevice();
};

#endif	/* TESTMSGINUPDATE_H */

