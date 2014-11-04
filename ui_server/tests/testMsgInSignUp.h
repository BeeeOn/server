/*
 * File:   testMsgInSignUp.h
 * Author: pavel
 *
 * Created on 22.7.2014, 14:31:55
 */

#ifndef TESTMSGINSIGNUP_H
#define	TESTMSGINSIGNUP_H

#include <cppunit/extensions/HelperMacros.h>

class testMsgInSignUp : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInSignUp);

    CPPUNIT_TEST(testNewUser);
    CPPUNIT_TEST(testExistingUser);
    CPPUNIT_TEST(testWrongGToken);
    
    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInSignUp();
    virtual ~testMsgInSignUp();
    void setUp();
    void tearDown();

private:
    void testNewUser();
    void testExistingUser();
    void testWrongGToken();
};

#endif	/* TESTMSGINSIGNUP_H */

