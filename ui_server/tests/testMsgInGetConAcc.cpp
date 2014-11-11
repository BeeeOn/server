/*
 * File:   testMsgInGetConAcc.cpp
 * Author: pavel
 *
 * Created on 9.8.2014, 14:08:40
 */

#include "testMsgInGetConAcc.h"
#include "MsgInGetAccount.h"

#include "ServerException.h"

CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInGetConAcc);

testMsgInGetConAcc::testMsgInGetConAcc() {
}

testMsgInGetConAcc::~testMsgInGetConAcc() {
}

void testMsgInGetConAcc::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING,10);
    Logger::getInstance().setVerbosityThreshold(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
    ComTable::getInstance().setComInfo("user1@gmail.com");
}

void testMsgInGetConAcc::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInGetConAcc::testTwoUsers() {
    char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInGetConAccount::state.c_str(),"\" adapter=\"21\">"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
    ostringstream oss;
    pugi::xml_node node = doc->child("communication");
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
        it->print(oss,"",pugi::format_raw);
    string  errText = oss.str();
    
//    CPPUNIT_ASSERT_ASSERTION_PASS(CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), R_ACCOUNTS, state) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), (string)"<user mail=\"user2@gmail.com\" role=\"superuser\" /><user mail=\"user2a@gmail.com\" role=\"admin\" />", errText );
}

void testMsgInGetConAcc::testWrongAdapter() {
        char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInGetConAccount::state.c_str(),"\" adapter=\"10\">"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    int  errCode = doc->child("communication").attribute("errcode").as_int(-1000);
    
    CPPUNIT_ASSERT_ASSERTION_PASS(CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), (string)"false", state) );
    CPPUNIT_ASSERT_ASSERTION_PASS(CPPUNIT_ASSERT_EQUAL(ServerException::MSG_RIGHT, errCode ); );
}

