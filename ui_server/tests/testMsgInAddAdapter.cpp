/*
 * File:   testMsgInAddAdapter.cpp
 * Author: pavel
 *
 * Created on 3.9.2014, 13:27:29
 */

#include "testMsgInAddAdapter.h"


CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInAddAdapter);

testMsgInAddAdapter::testMsgInAddAdapter() {
}

testMsgInAddAdapter::~testMsgInAddAdapter() {
}

void testMsgInAddAdapter::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbosityThreshold(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
}

void testMsgInAddAdapter::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInAddAdapter::testAddAdapter() {
        char msg[300];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInAddAdapter::state.c_str(),"\"> "
                "<adapter id=\"30\" name=\"newAdapter30\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    string name = DBConnector::getInstance().DEBUGexec("select name  from users_adapters join adapters on fk_adapter_id=adapter_id where fk_user_id=(select user_id from users where mail='user2@gmail.com') and fk_adapter_id=30");
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response,R_TRUE, state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response,(string)"newAdapter30", name);
}

void testMsgInAddAdapter::testNotExistedAdapter() {
    char msg[300];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInAddAdapter::state.c_str(),"\"> "
                "<adapter id=\"0\" name=\"nonExisted\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response,R_FALSE, state);
}
