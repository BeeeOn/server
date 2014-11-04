/*
 * File:   testMsgInGetConditions.cpp
 * Author: pavel
 *
 * Created on 16.9.2014, 15:35:16
 */

#include "testMsgInGetConditions.h"
#include "MsgInGetConditions.h"

CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInGetConditions);

testMsgInGetConditions::testMsgInGetConditions() {
}

testMsgInGetConditions::~testMsgInGetConditions() {
}

void testMsgInGetConditions::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbosityThreshold(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
}

void testMsgInGetConditions::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInGetConditions::testMethod() {
    
    char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInGetConditions::state.c_str(),"\">"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),MsgOutConditions::state, state);
}

void testMsgInGetConditions::testFailedMethod() {
    CPPUNIT_ASSERT(true);
}

