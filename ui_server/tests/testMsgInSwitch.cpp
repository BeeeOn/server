/*
 * File:   testMsgInSwitch.cpp
 * Author: pavel
 *
 * Created on 6.8.2014, 10:06:56
 */

#include "testMsgInSwitch.h"


CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInSwitch);

testMsgInSwitch::testMsgInSwitch() {
}

testMsgInSwitch::~testMsgInSwitch() {
}

void testMsgInSwitch::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbosityThreshold(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
}

void testMsgInSwitch::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInSwitch::testMethod() {
        char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"switch\" adapter=\"21\">"
                "<device type=\"1\" id=\"0.0.0.210\">"
                    "<value>ON</value>"
            "<device/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),(string)"true", state);
}

void testMsgInSwitch::testFailedMethod() {
        char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"switch\" adapter=\"10\">"
                "<device type=\"0x01\" id=\"0.0.0.100\">"
                    "<value>ON</value>"
            "<device/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),(string)"false", state);
}

