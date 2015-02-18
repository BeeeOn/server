/*
 * File:   testMsgInSetTimeZone.cpp
 * Author: pavel
 *
 * Created on 9.8.2014, 11:00:14
 */

#include "testMsgInSetTimeZone.h"


CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInSetTimeZone);

testMsgInSetTimeZone::testMsgInSetTimeZone() {
}

testMsgInSetTimeZone::~testMsgInSetTimeZone() {
}

void testMsgInSetTimeZone::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbosityThreshold(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
}

void testMsgInSetTimeZone::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInSetTimeZone::testSetUTC() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"settimezone\" adapter=\"21\">"
            "<time utc=\"-2\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    string timezone = DBConnector::getInstance().DEBUGexec("select timezone from adapters where adapter_id=21");

    CPPUNIT_ASSERT_ASSERTION_PASS(CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),timezone, (string)"-2"));
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),state, (string)"true");
}

void testMsgInSetTimeZone::testWrongUTC() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"settimezone\" adapter=\"21\">"
            "<time utc=\"13\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    int errCode = doc->child("communication").attribute("errcode").as_int(-1000);
    
    CPPUNIT_ASSERT_ASSERTION_PASS(CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),errCode, ServerException::TIMEZONE_UTC));
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),state, (string)"false");
}

