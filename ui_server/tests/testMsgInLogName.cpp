/*
 * File:   testMsgInLogName.cpp
 * Author: pavel
 *
 * Created on 6.8.2014, 11:24:51
 */

#include "testMsgInLogName.h"
#include "MsgInGetLog.h"


CPPUNIT_TEST_SUITE_REGISTRATION(testMsgInLogName);

testMsgInLogName::testMsgInLogName() {
}

testMsgInLogName::~testMsgInLogName() {
}

void testMsgInLogName::setUp() {
    DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbosityThreshold(Logger::DEBUG3);
    ComTable::getInstance().setComInfo("user2@gmail.com");
}

void testMsgInLogName::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInLogName::testEmptyLogsInterval() {
    char msg[300];
    sprintf (msg, "%s%s%s%s%s", 
            "<com ver=",COM_VERSION," sid=\"1\" state=\"",MsgInLogName::state.c_str(),"\"  from=\"2013-08-28 10:00:00\" to=\"2013-08-28 10:01:30\" ftype=\"avg\" interval=\"60\" aid=\"20\" "
                "dtype=\"1\" did=\"0.0.20.1\" />"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child(P_COMMUNICATION).attribute(P_STATE).value();
    
    ostringstream oss;
    pugi::xml_node node = doc->child(P_COMMUNICATION);
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    it->print(oss,"",pugi::format_raw);
    string log = oss.str();
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),MsgOutLogContent::state, state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),(string)"<row>2013-08-28 10:00:00 NaN</row><row>2013-08-28 10:01:00 NaN</row>", log);
}

void testMsgInLogName::testEmptyLogsRAW() {
    char msg[300];
    sprintf (msg, "%s%s%s%s%s", 
            "<com ver=",COM_VERSION," sid=\"1\" state=\"",MsgInLogName::state.c_str(),"\"  from=\"2013-08-28 10:00:00\" to=\"2013-08-28 10:01:30\" ftype=\"avg\" interval=\"0\" aid=\"20\" "
            "dtype=\"1\" did=\"0.0.20.1\" />"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child(P_COMMUNICATION).attribute(P_STATE).value();
    
    ostringstream oss;
    pugi::xml_node node = doc->child(P_COMMUNICATION);
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    it->print(oss,"",pugi::format_raw);
    string log = oss.str();
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),MsgOutLogContent::state, state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),(string)"", log);
}

void testMsgInLogName::testLogsInterval() {
    char msg[300];
    sprintf (msg, "%s%s%s%s%s", 
            "<com ver=",COM_VERSION," sid=\"1\" state=\"",MsgInLogName::state.c_str(),"\"  from=\"2014-08-09 00:00:00\" to=\"2014-08-11 10:01:30\" ftype=\"avg\" interval=\"86400\" aid=\"21\" "
            "dtype=\"1\" did=\"0.0.21.1\" />"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child(P_COMMUNICATION).attribute(P_STATE).value();
    
    ostringstream oss;
    pugi::xml_node node = doc->child(P_COMMUNICATION);
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    it->print(oss,"",pugi::format_raw);
    string log = oss.str();
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),MsgOutLogContent::state, state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),(string)"<row>2014-08-09 00:00:00 105.50</row><row>2014-08-10 00:00:00 50.00</row><row>2014-08-11 00:00:00 NaN</row>", log);
}
void testMsgInLogName::testLogsInterval2() {
    char msg[300];
    sprintf (msg, "%s%s%s%s%s", 
            "<com ver=",COM_VERSION," sid=\"1\" state=\"",MsgInLogName::state.c_str(),"\"  from=\"2013-08-09 00:00:00\" to=\"2013-08-10 00:00:00\" ftype=\"avg\" interval=\"3600\" aid=\"21\" "
            "dtype=\"1\" did=\"0.0.21.1\" />"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child(P_COMMUNICATION).attribute(P_STATE).value();
    
    ostringstream oss;
    pugi::xml_node node = doc->child(P_COMMUNICATION);
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    it->print(oss,"",pugi::format_raw);
    string log = oss.str();
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),MsgOutLogContent::state, state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),
            (string)"<row>2013-08-09 00:00:00 NaN</row><row>2013-08-09 01:00:00 NaN</row>"
        "<row>2013-08-09 02:00:00 NaN</row><row>2013-08-09 03:00:00 NaN</row>"
        "<row>2013-08-09 04:00:00 NaN</row><row>2013-08-09 05:00:00 NaN</row>"
        "<row>2013-08-09 06:00:00 NaN</row><row>2013-08-09 07:00:00 NaN</row>"
        "<row>2013-08-09 08:00:00 NaN</row><row>2013-08-09 09:00:00 NaN</row>"
        "<row>2013-08-09 10:00:00 NaN</row><row>2013-08-09 11:00:00 NaN</row>"
        "<row>2013-08-09 12:00:00 NaN</row><row>2013-08-09 13:00:00 NaN</row>"
        "<row>2013-08-09 14:00:00 NaN</row><row>2013-08-09 15:00:00 NaN</row>"
        "<row>2013-08-09 16:00:00 NaN</row><row>2013-08-09 17:00:00 NaN</row>"
        "<row>2013-08-09 18:00:00 NaN</row>"
        "<row>2013-08-09 19:00:00 NaN</row><row>2013-08-09 20:00:00 NaN</row>"
        "<row>2013-08-09 21:00:00 NaN</row><row>2013-08-09 22:00:00 NaN</row>"
        "<row>2013-08-09 23:00:00 NaN</row><row>2013-08-10 00:00:00 NaN</row>",
        log);
}

void testMsgInLogName::testLogsRAW() {
    char msg[300];
    sprintf (msg, "%s%s%s%s%s", 
            "<com ver=",COM_VERSION," sid=\"1\" state=\"",MsgInLogName::state.c_str(),"\"  from=\"2014-08-09 00:01:00\" to=\"2014-08-09 20:01:30\" ftype=\"avg\" interval=\"0\" aid=\"21\" "
           "dtype=\"1\" did=\"0.0.21.1\" />"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child(P_COMMUNICATION).attribute(P_STATE).value();
    
    ostringstream oss;
    pugi::xml_node node = doc->child(P_COMMUNICATION);
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    it->print(oss,"",pugi::format_raw);
    string log = oss.str();
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(msg,MsgOutLogContent::state, state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(msg,(string)"<row>2014-08-09 19:00:56 100</row><row>2014-08-09 19:10:56 90</row><row>2014-08-09 19:20:56 110</row>", log);
}

void testMsgInLogName::testWrongDevice() {
    char msg[300];
    sprintf (msg, "%s%s%s%s%s", 
            "<com ver=",COM_VERSION," sid=\"1\" state=\"",MsgInLogName::state.c_str(),"\"  from=\"2014-08-09 00:00:00\" to=\"2014-08-09 20:01:30\" ftype=\"avg\" interval=\"0\" aid=\"20\" "
                "dtype=\"1\" did=\"0.0.21.1\" />"
            );
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child(P_COMMUNICATION).attribute(P_STATE).value();
    int  errCode = doc->child(P_COMMUNICATION).attribute("errcode").as_int(-1000);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response,(string)"false", state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response,ServerException::DEVICE_ID_TYPE, errCode);
}