/*
 * File:   newtestclassBeta.h
 * Author: pavel
 *
 * Created on 6.8.2014, 11:22:38
 */

#ifndef NEWTESTCLASSBETA_H
#define	NEWTESTCLASSBETA_H

#include <cppunit/extensions/HelperMacros.h>

class newtestclassBeta : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(newtestclassBeta);

    CPPUNIT_TEST(testCreateResponseMsgOut);
    CPPUNIT_TEST(testGetMsgAuthorization);

    CPPUNIT_TEST_SUITE_END();

public:
    newtestclassBeta();
    virtual ~newtestclassBeta();
    void setUp();
    void tearDown();

private:
    void testCreateResponseMsgOut();
    void testGetMsgAuthorization();

};

#endif	/* NEWTESTCLASSBETA_H */

