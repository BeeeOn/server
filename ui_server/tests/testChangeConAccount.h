/*
 * File:   testChangeConAccount.h
 * Author: pavel
 *
 * Created on 9.8.2014, 11:13:06
 */

#ifndef TESTCHANGECONACCOUNT_H
#define	TESTCHANGECONACCOUNT_H

#include <cppunit/extensions/HelperMacros.h>
#include "../testHelper.h"

#include "MsgInChangeAccount.h"
class testChangeConAccount : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testChangeConAccount);

    CPPUNIT_TEST(testOneChange);
    CPPUNIT_TEST(testWrongRole);
    CPPUNIT_TEST(testChangeRoleOfLastSU);
    CPPUNIT_TEST(testChangeRoleOfSU);
    CPPUNIT_TEST(testWrongUser);
    CPPUNIT_TEST(testWrongUserAndAdapter);
    
    CPPUNIT_TEST_SUITE_END();

public:
    testChangeConAccount();
    virtual ~testChangeConAccount();
    void setUp();
    void tearDown();

private:
    void testOneChange();
    void testWrongRole();
    void testChangeRoleOfLastSU();
    void testChangeRoleOfSU();
    void testWrongUser();
    void testWrongUserAndAdapter();
};

#endif	/* TESTCHANGECONACCOUNT_H */

