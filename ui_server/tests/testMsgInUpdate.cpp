/*
 * File:   testMsgInUpdate.cpp
 * Author: pavel
 *
 * Created on 9.8.2014, 10:35:40
 */

#include "testMsgInUpdate.h"


CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInUpdate);

testMsgInUpdate::testMsgInUpdate() {
}

testMsgInUpdate::~testMsgInUpdate() {
}

void testMsgInUpdate::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbose(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
    ComTable::getInstance().setComInfo("user4@gmail.com");
}

void testMsgInUpdate::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInUpdate::testOneDevice() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"getdevices\" adapter=\"21\">"
            "<device id=\"0.0.0.210\" type=\"1\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
    string s1= doc->child("communication").child("device").attribute("id").as_string();
    
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL(s1, (string)"0.0.0.210") );    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), R_DEVICES, state);
}

void testMsgInUpdate::testNoDevice() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"getdevices\" adapter=\"21\">"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
    pugi::xml_node n = doc->child("communication").child("device");

    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL(n.empty(), true) );
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),R_DEVICES ,state);
}

void testMsgInUpdate::testTwoDevice() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"getdevices\" adapter=\"21\">"
            "<device id=\"0.0.0.210\" type=\"1\"/>"
            "<device id=\"0.0.0.211\" type=\"1\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    string s1= doc->child("communication").child("device").attribute("id").as_string();
    string s2= doc->child("communication").child("device").next_sibling().attribute("id").as_string();
    
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL(s1, (string)"0.0.0.210") );   
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL(s2, (string)"0.0.0.211") );   
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), R_DEVICES,state);
}

void testMsgInUpdate::testWrongDevice() {
    char msg[300];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"getdevices\" adapter=\"21\">"
            "<device id=\"0.0.0.210\" type=\"1\"/>"
            "<device id=\"0.0.0.200\" type=\"1\"/>"//wrong
            "<device id=\"0.0.0.211\" type=\"1\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    int  errCode = doc->child("communication").attribute("errcode").as_int(-1000);
    
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), (string)"false",state) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),errCode, ServerException::DEVICE_ID_TYPE);
}
