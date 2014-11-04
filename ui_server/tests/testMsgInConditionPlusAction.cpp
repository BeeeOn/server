/*
 * File:   testMsgInConditionPlusAction.cpp
 * Author: pavel
 *
 * Created on 17.9.2014, 16:17:49
 */

#include "testMsgInConditionPlusAction.h"
#include "MsgInConditionPlusAction.h"


CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInConditionPlusAction);

testMsgInConditionPlusAction::testMsgInConditionPlusAction() {
}

testMsgInConditionPlusAction::~testMsgInConditionPlusAction() {
}

void testMsgInConditionPlusAction::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbosityThreshold(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
}

void testMsgInConditionPlusAction::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInConditionPlusAction::testMethod() {
        char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInConditionPlusAction::state.c_str(),"\">"
            "<condition id=\"1\"/>"
            "<action id=\"1\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    string actId = DBConnector::getInstance().DEBUGexec("select fk_action_id from conditions where cond_id=1");
    
    //CPPUNIT_ASSERT_EQUAL_MESSAGE(response,R_TRUE, state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response,(string)"1", actId);
}

void testMsgInConditionPlusAction::testFailedMethod() {
    CPPUNIT_ASSERT(true);
}

