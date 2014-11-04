/*
 * File:   testMsgInGetRooms.h
 * Author: pavel
 *
 * Created on 24.7.2014, 13:21:24
 */

#ifndef TESTMSGINGETROOMS_H
#define	TESTMSGINGETROOMS_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"

class testMsgInGetRooms : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInGetRooms);

    CPPUNIT_TEST(test2rooms);
    CPPUNIT_TEST(test0Rooms);

    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInGetRooms();
    virtual ~testMsgInGetRooms();
    void setUp();
    void tearDown();

private:
    void test2rooms();
    void test0Rooms();
};

#endif	/* TESTMSGINGETROOMS_H */

