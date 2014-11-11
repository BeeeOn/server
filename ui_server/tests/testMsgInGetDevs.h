/*
 * File:   testMsgInGetDevs.h
 * Author: pavel
 *
 * Created on 11.11.2014, 20:35:30
 */

#ifndef TESTMSGINGETDEVS_H
#define	TESTMSGINGETDEVS_H

#include <cppunit/extensions/HelperMacros.h>


class testMsgInGetDevs : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testMsgInGetDevs);

    CPPUNIT_TEST(testEmptyList);
    CPPUNIT_TEST(testOneDev);

    CPPUNIT_TEST_SUITE_END();

public:
    testMsgInGetDevs();
    virtual ~testMsgInGetDevs();
    void setUp();
    void tearDown();

private:
    void testEmptyList();
    void testOneDev();
};

#endif	/* TESTMSGINGETDEVS_H */

