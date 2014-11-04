/*
 * File:   testMsgInDelAction.h
 * Author: pavel
 *
 * Created on 17.9.2014, 15:57:32
 */

#ifndef TESTMSGINDELACTION_H
#define	TESTMSGINDELACTION_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"

class testMsgInDelAction : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInDelAction);

    CPPUNIT_TEST(testMethod);
    CPPUNIT_TEST(testFailedMethod);

    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInDelAction();
    virtual ~testMsgInDelAction();
    void setUp();
    void tearDown();

private:
    void testMethod();
    void testFailedMethod();
};

#endif	/* TESTMSGINDELACTION_H */

