/*
 * File:   testMsgInUpdateAction.h
 * Author: pavel
 *
 * Created on 17.9.2014, 15:34:24
 */

#ifndef TESTMSGINUPDATEACTION_H
#define	TESTMSGINUPDATEACTION_H

#include <cppunit/extensions/HelperMacros.h>

#include "../testHelper.h"
class testMsgInUpdateAction : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInUpdateAction);

    CPPUNIT_TEST(testMethod);
    CPPUNIT_TEST(testFailedMethod);

    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInUpdateAction();
    virtual ~testMsgInUpdateAction();
    void setUp();
    void tearDown();

private:
    void testMethod();
    void testFailedMethod();
};

#endif	/* TESTMSGINUPDATEACTION_H */

