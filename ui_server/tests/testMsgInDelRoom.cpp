/*
 * File:   testMsgInDelRoom.cpp
 * Author: pavel
 *
 * Created on 24.7.2014, 9:45:58
 */

#include "testMsgInDelRoom.h"


CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInDelRoom);

testMsgInDelRoom::testMsgInDelRoom() {
}

testMsgInDelRoom::~testMsgInDelRoom() {
}

void testMsgInDelRoom::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbosityThreshold(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
}

void testMsgInDelRoom::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInDelRoom::testFreeRoom() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"delroom\" adapter=\"21\">"
            "<location id=\"211\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
    CPPUNIT_ASSERT_EQUAL((string)"true", state);
}

void testMsgInDelRoom::testOccupiedRoom() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"delroom\" adapter=\"21\">"
            "<location id=\"210\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    string  newLoc = doc->child("communication").child("location").attribute("id").value();
    
    /*
     if(state != "roomcreated")
     CPPUNIT_ASSERT_EQUAL((string)"roomcreated", state);*/
    // CPPUNIT_ASSERT_MESSAGE("wrong new location",stoi(newLoc) > 0 );
    CPPUNIT_ASSERT_EQUAL((string)"true", state);
}

void testMsgInDelRoom::testWrongRoom() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"delroom\" adapter=\"21\">"
            "<location id=\"999\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    string  errCode = doc->child("communication").attribute("errcode").value();
    
    CPPUNIT_ASSERT_EQUAL((string)"true", state);
}

void testMsgInDelRoom::testDefaultRoom() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"delroom\" adapter=\"21\">"
            "<location id=\"0\"/>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    string  errCode = doc->child("communication").attribute("errcode").value();
    
    
    CPPUNIT_ASSERT_EQUAL((string)"true", state);
}