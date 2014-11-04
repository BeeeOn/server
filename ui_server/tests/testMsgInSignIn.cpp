/*
 * File:   testMsgInSignIn.cpp
 * Author: pavel
 *
 * Created on 22.7.2014, 14:31:14
 */

#include <openssl/ssl.h>

#include "testMsgInSignIn.h"
#include "../testHelper.h"
#include "MsgInSignIn.h"


CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInSignIn);

testMsgInSignIn::testMsgInSignIn() {
}

testMsgInSignIn::~testMsgInSignIn() {
}

void testMsgInSignIn::setUp() {
    DBConnector::getInstance().setConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbosityThreshold(Logger::NO_OUTPUT);
}

void testMsgInSignIn::tearDown() {
}

void testMsgInSignIn::testSimple() {

    char msg[200];
    createMsgInWithAttributes(msg, COM_VERSION,"0",MsgInSignIn::state,P_EMAIL, "user2@gmail.com", P_GOOGLE_TOKEN,"1",P_LOCALIZATION,"cs", P_GOOGLE_CLOUD_MID,"");
    
    string response = resolveMsg(msg);
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    return ;
    string  state = getState(doc);
    int  id = getSessionId(doc);
    
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response, R_TRUE, state);
    CPPUNIT_ASSERT_MESSAGE(response, -1 != id);
    
}

void testMsgInSignIn::testMissingLocale() {
    char msg[200];
    createMsgInWithAttributes(msg, COM_VERSION,"0",MsgInSignIn::state,P_EMAIL, "user2@gmail.com", P_GOOGLE_TOKEN,"1", P_GOOGLE_CLOUD_MID,"");
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = getState(doc);
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response, R_FALSE, state);
}

void testMsgInSignIn::testWrongLocale() {
    char msg[200];
   createMsgInWithAttributes(msg, COM_VERSION,"0",MsgInSignIn::state,P_EMAIL, "user2@gmail.com", P_GOOGLE_TOKEN,"1",P_LOCALIZATION,"xx", P_GOOGLE_CLOUD_MID,"");
    
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = getState(doc);
    int  errCode = getErrCode(doc);
    
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response, R_FALSE, state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response, ServerException::LOCALE, errCode);
}

void testMsgInSignIn::testWrongGToken() {
    char msg[200];
    createMsgInWithAttributes(msg, COM_VERSION,"0",MsgInSignIn::state,P_EMAIL, "user2@gmail.com", P_GOOGLE_TOKEN,"xxxx",P_LOCALIZATION,"cs", P_GOOGLE_CLOUD_MID,"");
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = getState(doc);
    int  errCode = getErrCode(doc);
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response, R_FALSE, state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response, ServerException::TOKEN_EMAIL, errCode);
}
void testMsgInSignIn::testNewMail() {
    char msg[200];
    createMsgInWithAttributes(msg, COM_VERSION,"0",MsgInSignIn::state,P_EMAIL, "xxx@xxx", P_GOOGLE_TOKEN,"1",P_LOCALIZATION,"cs", P_GOOGLE_CLOUD_MID,"");
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = getState(doc);
    int  errCode = getErrCode(doc);
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response, R_FALSE, state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response, ServerException::NOTREG_A, errCode);
}