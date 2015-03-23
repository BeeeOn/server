/*
 * File:   testMsgInGetRooms.cpp
 * Author: pavel
 *
 * Created on 24.7.2014, 13:21:24
 */

#include "testMsgInGetRooms.h"
#include "MsgInGetRooms.h"



CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInGetRooms);

testMsgInGetRooms::testMsgInGetRooms() {
}

testMsgInGetRooms::~testMsgInGetRooms() {
}

void testMsgInGetRooms::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbose(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
    ComTable::getInstance().setComInfo("user4@gmail.com");
}

void testMsgInGetRooms::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInGetRooms::test2rooms() {
    char msg[200];
    
    createMsgInWithAttributes(msg,COM_VERSION,"1",MsgInGetRooms::state,P_ADAPTER_ID, "21");
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = getState(doc);
    
    pugi::xml_node n1 = doc->child(P_COMMUNICATION).child(P_ROOM);
    pugi::xml_node n2 = n1.next_sibling();
    pugi::xml_node end = n2.next_sibling();
    
    string s1 = n1.attribute(P_IN_ROOM_ID).value();
    string s2 = n2.attribute(P_IN_ROOM_ID).value();
    
    //TODO tento test nebude casem fungovat - poradi roomu v xml
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response, (string)"210", s1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response, (string)"211", s2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response, true, end.empty());
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response, state, (string)R_ROOMS);
}

void testMsgInGetRooms::test0Rooms() {
    char msg[200];    
    createMsgInWithAttributes(msg,COM_VERSION,"2",MsgInGetRooms::state,P_ADAPTER_ID, "40");
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state =getState(doc);
    
    pugi::xml_node n =  doc->child(P_COMMUNICATION).child(P_ROOM);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response, true, n.empty());
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response, state, (string)R_ROOMS);
}

