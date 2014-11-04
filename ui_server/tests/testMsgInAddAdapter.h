/*
 * File:   testMsgInAddAdapter.h
 * Author: pavel
 *
 * Created on 3.9.2014, 13:27:28
 */

#ifndef TESTMSGINADDADAPTER_H
#define	TESTMSGINADDADAPTER_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"

#include "MsgInAddAdapter.h"
class testMsgInAddAdapter : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInAddAdapter);

    CPPUNIT_TEST(testAddAdapter);
    CPPUNIT_TEST(testNotExistedAdapter);

    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInAddAdapter();
    virtual ~testMsgInAddAdapter();
    void setUp();
    void tearDown();

private:
    void testAddAdapter();
    void testNotExistedAdapter();
};

#endif	/* TESTMSGINADDADAPTER_H */

