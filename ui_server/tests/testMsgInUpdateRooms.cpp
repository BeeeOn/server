/*
 * File:   testMsgInUpdateRooms.cpp
 * Author: pavel
 *
 * Created on 24.7.2014, 12:05:42
 */

#include "testMsgInUpdateRooms.h"


CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInUpdateRooms);

testMsgInUpdateRooms::testMsgInUpdateRooms() {
}

testMsgInUpdateRooms::~testMsgInUpdateRooms() {
}

void testMsgInUpdateRooms::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbosityThreshold(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
}

void testMsgInUpdateRooms::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInUpdateRooms::testWrongAdapterExistingRoom() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"updaterooms\" adapter=\"20\">"
            "<location id=\"210\" type=\"8\">new</location>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
//    CPPUNIT_ASSERT_EQUAL(state, R_FALSE);
}

void testMsgInUpdateRooms::testWrongRoom() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"updaterooms\" adapter=\"21\">"
            "<location id=\"999\" type=\"8\">new</location>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
//    CPPUNIT_ASSERT_EQUAL(state, R_FALSE);
}

void testMsgInUpdateRooms::testUpdate() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"updaterooms\" adapter=\"21\">"
            "<location id=\"210\" type=\"8\">new</location>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
    string r = DBConnector::getInstance().DEBUGexec("select name from rooms where room_id=210 and fk_adapter_id=21");

//    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL(state, R_TRUE) );
    CPPUNIT_ASSERT_EQUAL((string)"new",r);
}

void testMsgInUpdateRooms::testMoreUpdates() {
    char msg[400];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"updaterooms\" adapter=\"21\">"
            "<location id=\"210\" type=\"8\">new</location>"
            "<location id=\"210\" type=\"8\">new210</location>"
            "<location id=\"211\" type=\"8\">new211</location>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
    string r = DBConnector::getInstance().DEBUGexec("select name from rooms where room_id=210 and fk_adapter_id=21");
    string r2 = DBConnector::getInstance().DEBUGexec("select name from rooms where room_id=211 and fk_adapter_id=21");

//    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL(state, R_TRUE) );
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL((string)"new210",r ) );
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL((string)"new211",r2) );
    CPPUNIT_ASSERT(true);
}


void testMsgInUpdateRooms::testMoreUpdatesWithError() {
    char msg[400];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"updaterooms\" adapter=\"21\">"
            "<location id=\"210\" type=\"8\">new</location>"
            "<location id=\"210\" type=\"8\">new210</location>"
            "<location id=\"200\" type=\"8\">newxx</location>"//id on different adapter
            "<location id=\"211\" type=\"8\">new211</location>"
            "</communication>"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    //TODO zpráva ve false    
    
    string r = DBConnector::getInstance().DEBUGexec("select name from rooms where room_id=210 and fk_adapter_id=21");
    string r2 = DBConnector::getInstance().DEBUGexec("select name from rooms where room_id=211 and fk_adapter_id=21");

//    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL(state, R_FALSE) );
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL((string)"new210",r) );
    CPPUNIT_ASSERT_ASSERTION_PASS(CPPUNIT_ASSERT_EQUAL((string)"new211",r2) );
    CPPUNIT_ASSERT(true);
}