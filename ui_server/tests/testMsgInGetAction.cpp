/*
 * File:   testMsgInGetAction.cpp
 * Author: pavel
 *
 * Created on 17.9.2014, 14:51:54
 */

#include "testMsgInGetAction.h"
#include "MsgInGetAction.h"


CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInGetAction);

testMsgInGetAction::testMsgInGetAction() {
}

testMsgInGetAction::~testMsgInGetAction() {
}

void testMsgInGetAction::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbosityThreshold(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
}

void testMsgInGetAction::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInGetAction::testMethod() {
        char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInGetAction::state.c_str(),"\">"
            "<complexaction id=\"1\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),MsgOutAction::state, state);
}

void testMsgInGetAction::testFailedMethod() {
    CPPUNIT_ASSERT(true);
}

