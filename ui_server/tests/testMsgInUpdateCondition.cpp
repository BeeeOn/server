/*
 * File:   testMsgInUpdateCondition.cpp
 * Author: pavel
 *
 * Created on 16.9.2014, 15:58:09
 */

#include "testMsgInUpdateCondition.h"
#include "MsgInUpdateCondition.h"

CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInUpdateCondition);

testMsgInUpdateCondition::testMsgInUpdateCondition() {
}

testMsgInUpdateCondition::~testMsgInUpdateCondition() {
}

void testMsgInUpdateCondition::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbose(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
}

void testMsgInUpdateCondition::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInUpdateCondition::testMethod() {
    
    char msg[300];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInUpdateCondition::state.c_str(),"\">"
            "<condition id=\"1\" name=\"cond\" type=\"and\">"
               "<func type=\"eq\">"
                    "<device id=\"0.0.0.100\" type=\"1\"/>"
                "<value>ON</value>"
            "</func>"
            "</condition>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),R_TRUE, state);
}

void testMsgInUpdateCondition::testFailedMethod() {
    CPPUNIT_ASSERT(true);
}

