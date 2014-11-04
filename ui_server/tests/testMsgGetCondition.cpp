/*
 * File:   testMsgGetCondition.cpp
 * Author: pavel
 *
 * Created on 16.9.2014, 11:58:48
 */

#include "testMsgGetCondition.h"


CPPUNIT_TEST_SUITE_REGISTRATION(testMsgGetCondition);

testMsgGetCondition::testMsgGetCondition() {
}

testMsgGetCondition::~testMsgGetCondition() {
}

void testMsgGetCondition::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbosityThreshold(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
}

void testMsgGetCondition::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgGetCondition::testMethod() {
        char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",msgInGetCondition::state.c_str(),"\">"
            "<condition id=\"1\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
    //CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),MsgOutCondition::state, state);
}

void testMsgGetCondition::testFailedMethod() {
            char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",msgInGetCondition::state.c_str(),"\">"
            "<condition id=\"999\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
   // CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),R_FALSE, state);
}

