/*
 * File:   testMsgInSetCondition.cpp
 * Author: pavel
 *
 * Created on 16.9.2014, 10:27:03
 */

#include "testMsgInSetCondition.h"
#include "MsgInSetCondition.h"

CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInSetCondition);

testMsgInSetCondition::testMsgInSetCondition() {
}

testMsgInSetCondition::~testMsgInSetCondition() {
}

void testMsgInSetCondition::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbosityThreshold(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
}

void testMsgInSetCondition::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInSetCondition::testCond() {
    char msg[400];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInSetCondition::state.c_str(),"\">"
            "<condition name=\"newCond\" type=\"and\">"
                "<func type=\"eq\">"
                    "<device id=\"12345\" type=\"0x01\"/>"
                    "<value>ON</value>"
                "</func>"
            "</condition>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
    string xml = DBConnector::getInstance().DEBUGexec("select xml from conditions where name='newCond' and fk_user_id=(select user_id from users where mail='user2@gmail.com')");
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),MsgOutConditionCreated::state, state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),(string)"<func type=\"eq\"><device id=\"12345\" type=\"0x01\" /><value>ON</value></func>", xml);
}

void testMsgInSetCondition::testEmptyCond() {
    char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInSetCondition::state.c_str(),"\">"
            "<condition name=\"cond\" type=\"and\">"
            "</condition>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),R_FALSE, state);
}

