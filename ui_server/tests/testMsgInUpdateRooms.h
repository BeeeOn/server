/*
 * File:   testMsgInUpdateRooms.h
 * Author: pavel
 *
 * Created on 24.7.2014, 12:05:42
 */

#ifndef TESTMSGINUPDATEROOMS_H
#define	TESTMSGINUPDATEROOMS_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"

class testMsgInUpdateRooms : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInUpdateRooms);

    CPPUNIT_TEST(testWrongAdapterExistingRoom);
    CPPUNIT_TEST(testUpdate);
    CPPUNIT_TEST(testMoreUpdates);
    CPPUNIT_TEST(testWrongRoom);
    CPPUNIT_TEST(testMoreUpdatesWithError);
    
    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInUpdateRooms();
    virtual ~testMsgInUpdateRooms();
    void setUp();
    void tearDown();

private:
    void testWrongAdapterExistingRoom();
    void testUpdate();
    void testMoreUpdates();
    void testWrongRoom();
    void testMoreUpdatesWithError();
};

#endif	/* TESTMSGINUPDATEROOMS_H */

