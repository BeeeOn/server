/*
 * File:   testMsgInGetAdapters.cpp
 * Author: pavel
 *
 * Created on 9.8.2014, 14:18:28
 */

#include "testMsgInGetAdapters.h"


CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInGetAdapters);

testMsgInGetAdapters::testMsgInGetAdapters() {
}

testMsgInGetAdapters::~testMsgInGetAdapters() {
}

void testMsgInGetAdapters::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbose(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
    ComTable::getInstance().setComInfo("user4@gmail.com");
    ComTable::getInstance().setComInfo("user2a@gmail.com");
    ComTable::getInstance().setComInfo("userwoadapter@gmail.com");
}

void testMsgInGetAdapters::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInGetAdapters::testOneSU() {
       char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"2\" state=\"getadapters\" >"
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
    //    CPPUNIT_ASSERT_ASSERTION_PASS(CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), R_ADAPTERS, state) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), (string)"<adapter id=\"40\" name=\"ad40\" role=\"superuser\" utc=\"1\" />", errText );
}

void testMsgInGetAdapters::testTwoSU() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"getadapters\" >"
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
    
//    CPPUNIT_ASSERT_ASSERTION_PASS(CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), R_ADAPTERS, state) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), (string)"<adapter id=\"20\" name=\"ad20\" role=\"superuser\" utc=\"1\" /><adapter id=\"21\" name=\"ad21\" role=\"superuser\" utc=\"1\" />", errText );
}

void testMsgInGetAdapters::testAdminOnAdapter() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"3\" state=\"getadapters\" >"
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
    
//    CPPUNIT_ASSERT_ASSERTION_PASS(CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),R_ADAPTERS, state) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), (string)"<adapter id=\"21\" name=\"ad21\" role=\"admin\" utc=\"1\" />", errText );
}

void testMsgInGetAdapters::testWithoutAdapters() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"4\" state=\"getadapters\" >"
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
    
//    CPPUNIT_ASSERT_ASSERTION_PASS(CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), R_ADAPTERS, state) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(), (string)"", errText );
}