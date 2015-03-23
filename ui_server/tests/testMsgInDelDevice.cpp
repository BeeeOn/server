/*
 * File:   testMsgInDelDevice.cpp
 * Author: pavel
 *
 * Created on 5.8.2014, 11:21:58
 */

#include "testMsgInDelDevice.h"

#include "ServerException.h"

CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInDelDevice);

testMsgInDelDevice::testMsgInDelDevice() {
}

testMsgInDelDevice::~testMsgInDelDevice() {
}

void testMsgInDelDevice::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbose(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
}

void testMsgInDelDevice::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInDelDevice::testNormalDelete() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"deldevice\" adapter=\"21\">"
                "<device type=\"1\" id=\"0.0.0.210\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),(string)"true", state);
}

void testMsgInDelDevice::testWrongAdapter() {
            char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"deldevice\" adapter=\"10\">"
                "<device type=\"0x01\" id=\"0.0.0.100\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    string  errCode = doc->child("communication").attribute("errcode").value();
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),(string)"false", state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),ServerException::MSG_RIGHT ,stoi(errCode));
}

