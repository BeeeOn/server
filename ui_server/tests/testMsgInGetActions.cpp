/*
 * File:   testMsgInGetActions.cpp
 * Author: pavel
 *
 * Created on 17.9.2014, 14:03:05
 */

#include "testMsgInGetActions.h"
#include "MsgInGetActions.h"


CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInGetActions);

testMsgInGetActions::testMsgInGetActions() {
}

testMsgInGetActions::~testMsgInGetActions() {
}

void testMsgInGetActions::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbosityThreshold(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
}

void testMsgInGetActions::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInGetActions::testMethod() {
        
    char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInGetActions::state.c_str(),"\">"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),MsgOutActions::state, state);
}

void testMsgInGetActions::testFailedMethod() {
    CPPUNIT_ASSERT(true);
}

