/*
 * File:   testMsgInGetDevs.cpp
 * Author: pavel
 *
 * Created on 11.11.2014, 20:35:31
 */

#include "testMsgInGetDevs.h"
#include "../testHelper.h"
#include "MsgInGetDevices.h"

CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInGetDevs);

testMsgInGetDevs::testMsgInGetDevs() {
}

testMsgInGetDevs::~testMsgInGetDevs() {
}

void testMsgInGetDevs::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING,10);
    Logger::getInstance().setVerbose(Logger::NO_OUTPUT);
    
    ComTable::getInstance().setComInfo("user1@gmail.com");
    ComTable::getInstance().setComInfo("user2@gmail.com");
}

void testMsgInGetDevs::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInGetDevs::testEmptyList() {
    char msg[200];
    createMsgInWithAttributes(msg, COM_VERSION, "1", MsgInGetDevs::state);
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = getState(doc);
    
    pugi::xml_node n = doc->child(P_COMMUNICATION).child(P_ADAPTER);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),state, (string)R_DEVICES);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),n.empty(), true) ;
}

void testMsgInGetDevs::testOneDev() {
    char msg[200];
    //TODO
    createMsgInWithAttributes(msg, COM_VERSION, "1", MsgInGetDevs::state);
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = getState(doc);
    
    pugi::xml_node n = doc->child(P_COMMUNICATION).child(P_ADAPTER);
}

