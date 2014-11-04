/*
 * File:   testMsgInPartial.cpp
 * Author: pavel
 *
 * Created on 9.8.2014, 14:45:57
 */

#include "testMsgInPartial.h"
#include "../testHelper.h"

CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInPartial);

testMsgInPartial::testMsgInPartial() {
}

testMsgInPartial::~testMsgInPartial() {
}

void testMsgInPartial::setUp() {    
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING,10);
    Logger::getInstance().setVerbosityThreshold(Logger::NO_OUTPUT);
    
    ComTable::getInstance().setComInfo("user1@gmail.com");
    ComTable::getInstance().setComInfo("user2@gmail.com");
}

void testMsgInPartial::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInPartial::testEmptyMsg() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"2\" state=\"devices\" adapter=\"21\">"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
    pugi::xml_node n = doc->child("communication").child("device");
    
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),state, (string)"true"));
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL(n.empty(), true) );
}

void testMsgInPartial::testTwoDevices() {
    char msg[300];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"2\" state=\"devices\" adapter=\"21\">"
            "<device  type=\"1\" id=\"0.0.0.210\">"
                "<name>New Name</name>"
            "</device>"
            
            "<device  type=\"1\" id=\"0.0.0.211\">"
                "<refresh>531</refresh>"
	   //<logging enabled="0"/>
            "</device>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
   
    string  state = doc->child("communication").attribute("state").value();
    string name = DBConnector::getInstance().DEBUGexec("select name from devices where mac='0.0.0.210'  and type='1' "); 
    string refresh = DBConnector::getInstance().DEBUGexec("select refresh from devices where mac='0.0.0.211' and type='1' "); 
   
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL_MESSAGE(response,(string)"true",state));
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL_MESSAGE(response,(string)"New Name",name));
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL_MESSAGE(response,(string)"531",refresh));
}
void testMsgInPartial::testWrongDevice() {
    char msg[300];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"2\" state=\"devices\" adapter=\"21\">"
            "<device  type=\"1\" id=\"0.0.0.210\">"
                "<name>Teplota u dverí</name>"
            "</device>"
            
            "<device  type=\"1\" id=\"0.0.0.200\">"
                "<refresh>531</refresh>"
            "</device>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    string refresh = DBConnector::getInstance().DEBUGexec("select refresh from devices where mac='0.0.0.200' and type='1' ");
    int errCode = doc->child("communication").attribute("errcode").as_int(-1000);
    
    ostringstream oss;
    pugi::xml_node node = doc->child("communication");
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    it->print(oss,"",pugi::format_raw);
    string errText = oss.str();

    
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL_MESSAGE(response,(string)"false",state));
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL_MESSAGE(response,ServerException::DEVICE_ID_TYPE, errCode));
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL_MESSAGE(response,true, refresh!="531"));
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL_MESSAGE(response,(string)"<device id=\"0.0.0.200\" type=\"1\" />",errText));
}