/*
 * File:   testMsgInAddRooms.h
 * Author: pavel
 *
 * Created on 23.7.2014, 13:04:34
 */

#ifndef TESTMSGINADDROOMS_H
#define	TESTMSGINADDROOMS_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"



class testMsgInAddRooms : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInAddRooms);

    CPPUNIT_TEST(testNormalFunction);
    CPPUNIT_TEST(testWrongRoomType);
    CPPUNIT_TEST(testWrongRoomTypeNotInt);
    CPPUNIT_TEST(testWrongAdapter);
    
    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInAddRooms();
    virtual ~testMsgInAddRooms();
    void setUp();
    void tearDown();

private:
    void testNormalFunction();
    void testWrongRoomType();
    void testWrongRoomTypeNotInt();
    void testWrongAdapter();
};

#endif	/* TESTMSGINADDROOMS_H */

