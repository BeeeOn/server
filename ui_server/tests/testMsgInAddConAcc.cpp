/*
 * File:   testMsgInAddConAcc.cpp
 * Author: pavel
 *
 * Created on 1.8.2014, 15:55:45
 */

#include "testMsgInAddConAcc.h"

#include "ServerException.h"

CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInAddConAcc);

testMsgInAddConAcc::testMsgInAddConAcc() {
}

testMsgInAddConAcc::~testMsgInAddConAcc() {
}

void testMsgInAddConAcc::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING,10);
    Logger::getInstance().setVerbose(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user1@gmail.com");
}

void testMsgInAddConAcc::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInAddConAcc::testAddOne() {
    char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInAddAccount::state.c_str(),"\" adapter=\"10\">"
                "<user email=\"newuser@gmail.com\" role=\"user\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
//    CPPUNIT_ASSERT_ASSERTION_PASS(CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),R_TRUE, state) );
    
    string userId = DBConnector::getInstance().DEBUGexec("select user_id from users where mail='newuser@gmail.com'" );
    CPPUNIT_ASSERT_ASSERTION_FAIL(CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), (string)"", userId) );
    
    string userRole = DBConnector::getInstance().DEBUGexec("select role from users_adapters where fk_user_id='"+userId+"'" );
    CPPUNIT_ASSERT_ASSERTION_PASS(CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),(string)"user", userRole) );
}

void testMsgInAddConAcc::testAddTwo() {
    char msg[300];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInAddAccount::state.c_str(),"\" adapter=\"10\">"
                "<user email=\"newuser@gmail.com\" role=\"user\"/>"
            "<user email=\"newuser2@gmail.com\" role=\"superuser\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
//    CPPUNIT_ASSERT_ASSERTION_PASS(CPPUNIT_ASSERT_EQUAL(R_TRUE, state) );
    
    string userRole1 = DBConnector::getInstance().DEBUGexec("select role from users_adapters where fk_user_id=(select user_id from users where mail='newuser@gmail.com') " );
    CPPUNIT_ASSERT_ASSERTION_PASS(CPPUNIT_ASSERT_EQUAL((string)"user", userRole1) );
    
    string userRole2 = DBConnector::getInstance().DEBUGexec("select role from users_adapters where fk_user_id=(select user_id from users where mail='newuser2@gmail.com') " );
    CPPUNIT_ASSERT_ASSERTION_PASS(CPPUNIT_ASSERT_EQUAL((string)"superuser", userRole2) );
}

void testMsgInAddConAcc::testWrongAdapter() {
    char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInAddAccount::state.c_str(),"\" adapter=\"20\">"
                "<user email=\"newuser@gmail.com\" role=\"user\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
    string  errCode = doc->child("communication").attribute("errcode").value();
    
    
//    CPPUNIT_ASSERT_ASSERTION_PASS(CPPUNIT_ASSERT_EQUAL(R_FALSE, state) );
    CPPUNIT_ASSERT_EQUAL(ServerException::MSG_RIGHT, stoi(errCode) );
    
}

void testMsgInAddConAcc::testWrongRole() {
    char msg[400];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInAddAccount::state.c_str(),"\" adapter=\"10\">"
                "<user email=\"newuser@gmail.com\" role=\"user\"/>"
                "<user email=\"newuser@gmail.com\" role=\"baduser\"/>"
                "<user email=\"newuser@gmail.com\" role=\"user\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    string  errCode = doc->child("communication").attribute("errcode").value();
    
    ostringstream oss;
    pugi::xml_node node = doc->child("communication");
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
        it->print(oss,"",pugi::format_raw);
    string  errText = oss.str();
    
//    CPPUNIT_ASSERT_ASSERTION_PASS(CPPUNIT_ASSERT_EQUAL(R_FALSE, state) );
    CPPUNIT_ASSERT_ASSERTION_PASS(CPPUNIT_ASSERT_EQUAL(ServerException::EMAIL_ROLE, stoi(errCode) ));
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), (string)"<user email=\"newuser@gmail.com\" role=\"baduser\" /><user email=\"newuser@gmail.com\" role=\"user\" />", errText );
}
