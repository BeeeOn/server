/*
 * File:   testMsgInDeleteCondition.cpp
 * Author: pavel
 *
 * Created on 16.9.2014, 16:15:02
 */

#include "testMsgInDeleteCondition.h"
#include "MsgInDelCondition.h"


CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInDeleteCondition);

testMsgInDeleteCondition::testMsgInDeleteCondition() {
}

testMsgInDeleteCondition::~testMsgInDeleteCondition() {
}

void testMsgInDeleteCondition::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbosityThreshold(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
}

void testMsgInDeleteCondition::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInDeleteCondition::testMethod() {
    char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInDelCondition::state.c_str(),"\">"
            "<condition id=\"1\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),R_TRUE, state);
}

void testMsgInDeleteCondition::testFailedMethod() {
    CPPUNIT_ASSERT(true);
}

