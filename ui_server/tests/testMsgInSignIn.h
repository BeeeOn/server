/*
 * File:   testMsgInSignIn.h
 * Author: pavel
 *
 * Created on 22.7.2014, 14:31:13
 */

#ifndef TESTMSGINSIGNIN_H
#define	TESTMSGINSIGNIN_H

#include <cppunit/extensions/HelperMacros.h>

class testMsgInSignIn : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInSignIn);

    CPPUNIT_TEST(testSimple);
    CPPUNIT_TEST(testMissingLocale);
    CPPUNIT_TEST(testWrongLocale);
    CPPUNIT_TEST(testWrongGToken);
    CPPUNIT_TEST(testNewMail);

    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInSignIn();
    virtual ~testMsgInSignIn();
    void setUp();
    void tearDown();

private:
    void testSimple();
    void testMissingLocale();
    void testWrongLocale();
    void testWrongGToken();
    void testNewMail();
};

#endif	/* TESTMSGINSIGNIN_H */

