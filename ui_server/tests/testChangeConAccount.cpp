/*
 * File:   testChangeConAccount.cpp
 * Author: pavel
 *
 * Created on 9.8.2014, 11:13:06
 */

#include "testChangeConAccount.h"
#include "../testHelper.h"
#include "ServerException.h"

CPPUNIT_TEST_SUITE_REGISTRATION(testChangeConAccount);

testChangeConAccount::testChangeConAccount() {
}

testChangeConAccount::~testChangeConAccount() {
}

void testChangeConAccount::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbosityThreshold(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
    ComTable::getInstance().setComInfo("user1@gmail.com");
}

void testChangeConAccount::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testChangeConAccount::testOneChange() {
    char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInChangeConAccount::state.c_str(),"\" adapter=\"21\">"
                "<user email=\"user2a@gmail.com\" role=\"user\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    string role = DBConnector::getInstance().DEBUGexec("select role from users_adapters where fk_user_id=(select user_id from users where mail='user2a@gmail.com') and fk_adapter_id=21");

    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), (string)"true",state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), (string)"user",role );
}

void testChangeConAccount::testWrongRole() {
    char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInChangeConAccount::state.c_str(),"\" adapter=\"21\">"
                "<user email=\"user2a@gmail.com\" role=\"userr\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();

    int  errCode = doc->child("communication").attribute("errcode").as_int(-1000);
    
    ostringstream oss;
    pugi::xml_node node = doc->child("communication");
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    it->print(oss,"",pugi::format_raw);
    string errText = oss.str();
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), (string)"false",state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), ServerException::EMAIL_ROLE,errCode);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), (string)"<user email=\"user2a@gmail.com\" role=\"userr\" />",errText );
}

void testChangeConAccount::testChangeRoleOfLastSU() {
    char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInChangeConAccount::state.c_str(),"\" adapter=\"21\">"
                "<user email=\"user2@gmail.com\" role=\"user\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();

    int  errCode = doc->child("communication").attribute("errcode").as_int(-1000);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), (string)"false", state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), ServerException::EMAIL_ROLE,errCode);
}

void testChangeConAccount::testChangeRoleOfSU() {
    char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"2\" state=\"",MsgInChangeConAccount::state.c_str(),"\" adapter=\"10\">"
                "<user email=\"user1su@gmail.com\" role=\"user\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), (string)"true",state);
}

void testChangeConAccount::testWrongUser() {
    char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInChangeConAccount::state.c_str(),"\" adapter=\"20\">"
                "<user email=\"user1su@gmail.com\" role=\"user\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
    int  errCode = doc->child("communication").attribute("errcode").as_int(-1000);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), (string)"false",state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), ServerException::EMAIL_ROLE,errCode);
}

void testChangeConAccount::testWrongUserAndAdapter() {
    char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInChangeConAccount::state.c_str(),"\" adapter=\"10\">"
                "<user email=\"user1su@gmail.com\" role=\"user\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
    int  errCode = doc->child("communication").attribute("errcode").as_int(-1000);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), (string)"false",state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), ServerException::MSG_RIGHT,errCode);
}