/*
 * File:   testMsgInDelAction.cpp
 * Author: pavel
 *
 * Created on 17.9.2014, 15:57:32
 */

#include "testMsgInDelAction.h"
#include "MsgInDelAction.h"


CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInDelAction);

testMsgInDelAction::testMsgInDelAction() {
}

testMsgInDelAction::~testMsgInDelAction() {
}

void testMsgInDelAction::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbosityThreshold(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
}

void testMsgInDelAction::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInDelAction::testMethod() {
    char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInDelAction::state.c_str(),"\">"
            "<complexaction id=\"1\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),R_TRUE, state);
}

void testMsgInDelAction::testFailedMethod() {
    CPPUNIT_ASSERT(true);
}

