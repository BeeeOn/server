/*
 * File:   testMsgInDelConAcc.cpp
 * Author: pavel
 *
 * Created on 9.8.2014, 11:36:18
 */

#include "testMsgInDelConAcc.h"
#include "MsgInDelAccount.h"
#include "../testHelper.h"

CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInDelConAcc);

testMsgInDelConAcc::testMsgInDelConAcc() {
}

testMsgInDelConAcc::~testMsgInDelConAcc() {
}

void testMsgInDelConAcc::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbosityThreshold(Logger::DEBUG3);
    ComTable::getInstance().setComInfo("user2@gmail.com");
    ComTable::getInstance().setComInfo("user4@gmail.com");
    ComTable::getInstance().setComInfo("user1@gmail.com");
}

void testMsgInDelConAcc::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInDelConAcc::testDeleteOne() {
    char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInDelConAccount::state.c_str(),"\" adapter=\"21\">"
            "<user email=\"user2a@gmail.com\" />"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    string userExist = DBConnector::getInstance().DEBUGexec("select count(*) from users_adapters where fk_user_id=(select user_id from users where mail='user2a@gmail.com') and fk_adapter_id=21");
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),state, (string)"true");
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),userExist , (string)"0");
}

void testMsgInDelConAcc::testWrongAdapter() {
    char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"",MsgInDelConAccount::state.c_str(),"\" adapter=\"20\">"
            "<user email=\"user2a@gmail.com\" />"
            "</communication>"
            );
    
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    string userExist = DBConnector::getInstance().DEBUGexec("select count(*) from users_adapters where fk_user_id=(select user_id from users where mail='user2a@gmail.com') and fk_adapter_id=21");
    int  errCode = doc->child("communication").attribute("errcode").as_int(-1000);
    ostringstream oss;
    pugi::xml_node node = doc->child("communication");
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    it->print(oss,"",pugi::format_raw);
    string errText = oss.str();
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),state, (string)"false");
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),userExist , (string)"1");
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),ServerException::EMAIL_ROLE, errCode);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),errText, (string)"<user email=\"user2a@gmail.com\" />");
     
}

void testMsgInDelConAcc::testDelLastSU() {
    char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"2\" state=\"",MsgInDelConAccount::state.c_str(),"\" adapter=\"40\">"
            "<user email=\"user4@gmail.com\" />"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    string userExist = DBConnector::getInstance().DEBUGexec("select count(*) from users_adapters where fk_user_id=(select user_id from users where mail='user4@gmail.com') and fk_adapter_id=40");
    string adapterRegistrable = DBConnector::getInstance().DEBUGexec("select registrable from adapters where adapter_id=40");
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),state, (string)"true");
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),userExist , (string)"0");
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),adapterRegistrable , (string)"1");
}

void testMsgInDelConAcc::testDelSU() {
    char msg[200];
    sprintf (msg, "%s%s%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"3\" state=\"",MsgInDelConAccount::state.c_str(),"\" adapter=\"10\">"
            "<user email=\"user1su@gmail.com\" />"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    string usersNo = DBConnector::getInstance().DEBUGexec("select count(*) from users_adapters where  fk_adapter_id=10");
    string adapterRegistrable = DBConnector::getInstance().DEBUGexec("select registrable from adapters where adapter_id=10");
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), (string)"true",state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str() , (string)"1",usersNo);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str() , (string)"0",adapterRegistrable);
}