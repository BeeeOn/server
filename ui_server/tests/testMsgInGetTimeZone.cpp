/*
 * File:   testMsgInGetTimeZone.cpp
 * Author: pavel
 *
 * Created on 9.8.2014, 11:07:39
 */

#include "testMsgInGetTimeZone.h"


CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInGetTimeZone);

testMsgInGetTimeZone::testMsgInGetTimeZone() {
}

testMsgInGetTimeZone::~testMsgInGetTimeZone() {
}

void testMsgInGetTimeZone::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbosityThreshold(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
}

void testMsgInGetTimeZone::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInGetTimeZone::testGetUTC() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"gettimezone\" adapter=\"21\">"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    int  timezone = doc->child("communication").child("time").attribute("utc").as_int(-20);

    CPPUNIT_ASSERT_ASSERTION_PASS(CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),timezone <= 12 && timezone >= -12 , true));
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),state, (string)"timezone");
}

