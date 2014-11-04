/*
 * File:   testMsgInSetAction.cpp
 * Author: pavel
 *
 * Created on 17.9.2014, 12:55:29
 */

#include "testMsgInSetAction.h"
#include "MsgInSetAction.h"


CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInSetAction);

testMsgInSetAction::testMsgInSetAction() {
}

testMsgInSetAction::~testMsgInSetAction() {
}

void testMsgInSetAction::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbosityThreshold(Logger::DEBUG3);
    ComTable::getInstance().setComInfo("user2@gmail.com");
}

void testMsgInSetAction::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInSetAction::testMethod() {
    
    char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInSetAction::state.c_str(),"\">"
                 " <complexaction name=\"newAction\" >"
                        "<action type=\"notification\"> </action>"
                  "</complexaction>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
    string xml = DBConnector::getInstance().DEBUGexec("select xml from actions where name='newAction' and fk_user_id=(select user_id from users where mail='user2@gmail.com')");
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),MsgOutActionCreated::state, state);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),(string)"<action type=\"notification\"/ >", xml);
}

void testMsgInSetAction::testFailedMethod() {
    CPPUNIT_ASSERT(true);
}

