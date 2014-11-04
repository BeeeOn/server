/*
 * File:   testMsgInGetConAcc.h
 * Author: pavel
 *
 * Created on 9.8.2014, 14:08:40
 */

#ifndef TESTMSGINGETCONACC_H
#define	TESTMSGINGETCONACC_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"

class testMsgInGetConAcc : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInGetConAcc);

    CPPUNIT_TEST(testTwoUsers);
    CPPUNIT_TEST(testWrongAdapter);

    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInGetConAcc();
    virtual ~testMsgInGetConAcc();
    void setUp();
    void tearDown();

private:
    void testTwoUsers();
    void testWrongAdapter();
};

#endif	/* TESTMSGINGETCONACC_H */

