/*
 * File:   testMsgInAddConAcc.h
 * Author: pavel
 *
 * Created on 1.8.2014, 15:55:44
 */

#ifndef TESTMSGINADDCONACC_H
#define	TESTMSGINADDCONACC_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"

#include "MsgInAddAccount.h"
class testMsgInAddConAcc : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInAddConAcc);

    CPPUNIT_TEST(testAddOne);
    CPPUNIT_TEST(testAddTwo);
    CPPUNIT_TEST(testWrongAdapter);
    CPPUNIT_TEST(testWrongRole);
    
    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInAddConAcc();
    virtual ~testMsgInAddConAcc();
    void setUp();
    void tearDown();

private:
    void testAddOne();
    void testAddTwo();
    void testWrongAdapter();
    void testWrongRole();
};

#endif	/* TESTMSGINADDCONACC_H */

