/*
 * File:   testMsgInSignUp.cpp
 * Author: pavel
 *
 * Created on 22.7.2014, 14:31:55
 */

#include "testMsgInSignUp.h"

#include "testHelper.h"

CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInSignUp);

testMsgInSignUp::testMsgInSignUp() {
}

testMsgInSignUp::~testMsgInSignUp() {
}

void testMsgInSignUp::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbose(Logger::NO_OUTPUT);
}

void testMsgInSignUp::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInSignUp::testNewUser() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," state=\"signup\" >"
            "<user email=\"newuser@gmail.com\" gtoken=\"1\" />"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response,(string)"true", state);
    CPPUNIT_ASSERT(true);
}

void testMsgInSignUp::testExistingUser() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," state=\"signup\" >"
            "<user email=\"user2@gmail.com\" gtoken=\"1\" />"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    int  errcode = doc->child("communication").attribute("errcode").as_int(-1);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response,(string)"false", state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response, ServerException::TOKEN_EMAIL, errcode);
}

void testMsgInSignUp::testWrongGToken() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," state=\"signup\" >"
            "<user email=\"newuser2@gmail.com\" gtoken=\"-1\" />"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    int  errcode = doc->child("communication").attribute("errcode").as_int(-1);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response,(string)"false", state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response, ServerException::TOKEN_EMAIL, errcode);
    
}