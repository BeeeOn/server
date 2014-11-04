/*
 * File:   SignInTest.h
 * Author: pavel
 *
 * Created on 27.3.2014, 21:36:57
 */

#ifndef SIGNINTEST_H
#define	SIGNINTEST_H

#include <cppunit/extensions/HelperMacros.h>

class SignInTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(SignInTest);

    CPPUNIT_TEST(notRegistred);
    //CPPUNIT_TEST(test0AdaptersAssignedFreeAdapterAvailable);
    //CPPUNIT_TEST(test0AdaptersAssignedNoAdapterAvailable);
    CPPUNIT_TEST(test1AdaptersAssigned);
    CPPUNIT_TEST(test2AdaptersAssigned);
    
    CPPUNIT_TEST_SUITE_END();

public:
    SignInTest();
    virtual ~SignInTest();
    void setUp();
    void tearDown();

private:
    void notRegistred();
    void test0AdaptersAssignedFreeAdapterAvailable();
    void test0AdaptersAssignedNoAdapterAvailable();
    void test1AdaptersAssigned();
    void test2AdaptersAssigned();
    //void testFailedMethod();
};

#endif	/* SIGNINTEST_H */

