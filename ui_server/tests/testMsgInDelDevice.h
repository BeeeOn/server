/*
 * File:   testMsgInDelDevice.h
 * Author: pavel
 *
 * Created on 5.8.2014, 11:21:57
 */

#ifndef TESTMSGINDELDEVICE_H
#define	TESTMSGINDELDEVICE_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"

class testMsgInDelDevice : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInDelDevice);

    CPPUNIT_TEST(testNormalDelete);
    CPPUNIT_TEST(testWrongAdapter);

    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInDelDevice();
    virtual ~testMsgInDelDevice();
    void setUp();
    void tearDown();

private:
    void testNormalDelete();
    void testWrongAdapter();
};

#endif	/* TESTMSGINDELDEVICE_H */

