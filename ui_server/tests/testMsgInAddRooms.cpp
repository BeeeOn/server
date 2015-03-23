/*
 * File:   testMsgInAddRooms.cpp
 * Author: pavel
 *
 * Created on 23.7.2014, 13:04:34
 */

#include "testMsgInAddRooms.h"
#include "../testHelper.h"
#include "ServerException.h"
CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInAddRooms);

testMsgInAddRooms::testMsgInAddRooms() {
}

testMsgInAddRooms::~testMsgInAddRooms() {
}

void testMsgInAddRooms::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbose(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user1@gmail.com");
}

void testMsgInAddRooms::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInAddRooms::testNormalFunction() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"addroom\" adapter=\"10\">"
                "<location type=\"2\">Obývací pokoj</location>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    string  newLoc = doc->child("communication").child("location").attribute("id").value();
    cout<<response;
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response, R_ROOM_CREATED, state);
    CPPUNIT_ASSERT_MESSAGE("wrong new location",stoi(newLoc) > 0 );
}
        
void testMsgInAddRooms::testWrongRoomType() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"addroom\" adapter=\"10\">"
                "<location type=\"999\">Obývací pokoj</location>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    string  errCode = doc->child("communication").attribute("errcode").value();
    
    
//    CPPUNIT_ASSERT_ASSERTION_PASS(CPPUNIT_ASSERT_EQUAL(R_FALSE, state) );
    CPPUNIT_ASSERT_EQUAL(ServerException::ROOM_TYPE, stoi(errCode) );
}

void testMsgInAddRooms::testWrongRoomTypeNotInt() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"addroom\" adapter=\"10\">"
                "<location type=\"a\">Obývací pokoj</location>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    string  errCode = doc->child("communication").attribute("errcode").value();
    
    
//    CPPUNIT_ASSERT_ASSERTION_PASS(CPPUNIT_ASSERT_EQUAL(R_FALSE, state) );
    CPPUNIT_ASSERT_EQUAL(ServerException::ROOM_TYPE, stoi(errCode) );
}

void testMsgInAddRooms::testWrongAdapter() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"addroom\" adapter=\"20\">"
                "<location type=\"11\">Obývací pokoj</location>"
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