/*
 * File:   SignInTest.cpp
 * Author: pavel
 *
 * Created on 27.3.2014, 21:36:58
 */

#include "SignInTest.h"

#include "myTestHelper.cpp"
#include "../communication.h"
#include "../DBConnector.h"
#include "../Logger.h"
#include "soci-postgresql.h"
#include "soci.h"
#include <stdio.h>
#include <string.h>

#define XMLTESTPATH "/home/pavel/Server/tests/XMLtestFiles/signin/"
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(SignInTest);

SignInTest::SignInTest() {
}

SignInTest::~SignInTest() {
}

void SignInTest::setUp() {
    DBConnector::getInstance().setConnectionString("port = '5432' dbname = 'smarthometest1' user = 'pavel' password = '1234' connect_timeout = '3'");
    Logger::getInstance().setVerbosityLevel(Logger::DEBUG);
}

void SignInTest::tearDown() {
}
void SignInTest::notRegistred() {
    string response = resolveMsg((char*)file2String("/home/pavel/Server/tests/XMLtestFiles/signin/notRegistred").c_str(), "ssl", "ip");
    CPPUNIT_ASSERT_EQUAL(response,
            file2String("/home/pavel/Server/tests/XMLtestFiles/signin/notRegistred_r") );

}
void SignInTest::test0AdaptersAssignedFreeAdapterAvailable() {
    CPPUNIT_ASSERT(true);
}
void SignInTest::test0AdaptersAssignedNoAdapterAvailable() {
    CPPUNIT_ASSERT(true);
}
void SignInTest::test1AdaptersAssigned() { 
    string response = resolveMsg((char*)file2String("/home/pavel/Server/tests/XMLtestFiles/signin/1Adapter").c_str(), "ssl", "ip");
    CPPUNIT_ASSERT_EQUAL(response ,
        file2String("/home/pavel/Server/tests/XMLtestFiles/signin/1Adapter_r"));
}
void SignInTest::test2AdaptersAssigned() {
    string response = resolveMsg((char*)file2String("/home/pavel/Server/tests/XMLtestFiles/signin/2Adapter").c_str(), "ssl", "ip");
    CPPUNIT_ASSERT_EQUAL(response ,
        file2String("/home/pavel/Server/tests/XMLtestFiles/signin/2Adapter_r"));
}

