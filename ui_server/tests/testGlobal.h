/*
 * File:   testGlobal.h
 * Author: pavel
 *
 * Created on 22.7.2014, 15:53:11
 */

#ifndef TESTGLOBAL_H
#define	TESTGLOBAL_H

#include <cppunit/extensions/HelperMacros.h>

class testGlobal : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(testGlobal);

    CPPUNIT_TEST(testWrongVersion);
    CPPUNIT_TEST(testWrongMsg);

    CPPUNIT_TEST_SUITE_END();

public:
    testGlobal();
    virtual ~testGlobal();
    void setUp();
    void tearDown();

private:
    void testWrongVersion();
    void testWrongMsg();
};

#endif	/* TESTGLOBAL_H */

