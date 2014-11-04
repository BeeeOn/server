/*
 * File:   testMsgInDelRoom.h
 * Author: pavel
 *
 * Created on 24.7.2014, 9:45:56
 */

#ifndef TESTMSGINDELROOM_H
#define	TESTMSGINDELROOM_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"

class testMsgInDelRoom : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInDelRoom);

    CPPUNIT_TEST(testFreeRoom);
    CPPUNIT_TEST(testOccupiedRoom);
    CPPUNIT_TEST(testWrongRoom);
    CPPUNIT_TEST(testDefaultRoom);
   

    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInDelRoom();
    virtual ~testMsgInDelRoom();
    void setUp();
    void tearDown();

private:
    void testFreeRoom();
    void testOccupiedRoom();
    void testWrongRoom();
    void testDefaultRoom();
};

#endif	/* TESTMSGINDELROOM_H */

