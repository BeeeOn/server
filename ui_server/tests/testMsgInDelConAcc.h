/*
 * File:   testMsgInDelConAcc.h
 * Author: pavel
 *
 * Created on 9.8.2014, 11:36:17
 */

#ifndef TESTMSGINDELCONACC_H
#define	TESTMSGINDELCONACC_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"

class testMsgInDelConAcc : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInDelConAcc);

    CPPUNIT_TEST(testDeleteOne);
    CPPUNIT_TEST(testWrongAdapter);
    CPPUNIT_TEST(testDelLastSU);
    CPPUNIT_TEST(testDelSU);
    
    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInDelConAcc();
    virtual ~testMsgInDelConAcc();
    void setUp();
    void tearDown();

private:
    void testDeleteOne();
    void testWrongAdapter();
    void testDelLastSU();
    void testDelSU();
};

#endif	/* TESTMSGINDELCONACC_H */

