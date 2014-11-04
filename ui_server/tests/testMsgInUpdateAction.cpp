/*
 * File:   testMsgInUpdateAction.cpp
 * Author: pavel
 *
 * Created on 17.9.2014, 15:34:24
 */

#include "testMsgInUpdateAction.h"
#include "MsgInUpdateAction.h"


CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInUpdateAction);

testMsgInUpdateAction::testMsgInUpdateAction() {
}

testMsgInUpdateAction::~testMsgInUpdateAction() {
}

void testMsgInUpdateAction::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbosityThreshold(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
}

void testMsgInUpdateAction::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInUpdateAction::testMethod() {
    char msg[300];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInUpdateAction::state.c_str(),"\">"
           "<complexaction id=\"1\" name=\"updatedAction\">"
            "<action type=\"actor\">"
                 "<device id=\"1234\" type=\"1\">"
                     "<value>ON<value>"
                "</device>"
            "</action>"
            "</complexaction>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),R_TRUE, state);

}

void testMsgInUpdateAction::testFailedMethod() {
    CPPUNIT_ASSERT(true);
}

