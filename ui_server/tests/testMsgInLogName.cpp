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
    Logger::getInstance().setVerbosityThreshold(Logger::NO_OUTPUT);
    ComTable::getInstance().setComInfo("user2@gmail.com");
}

void testMsgInLogName::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

void testMsgInLogName::testEmptyLogsInterval() {
    char msg[300];    
    createMsgInWithAttributes(msg, COM_VERSION, "1", MsgInLogName::state, P_LOG_FROM, "1377684000", P_LOG_TO, "1377684090", P_LOG_FUNC_TYPE, "avg", P_LOG_INTERVAL, "60", 
            P_DEVICE_ID, "0.0.20.1", P_DEVICE_TYPE, "1", P_ADAPTER_ID, "20");
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = getState(doc);
    
    ostringstream oss;
    pugi::xml_node node = doc->child(P_COMMUNICATION);
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    it->print(oss,"",pugi::format_raw);
    string log = oss.str();
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),MsgOutLogContent::state, state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),(string)"", log);
}

void testMsgInLogName::testEmptyLogsRAW() {
    char msg[300];
    createMsgInWithAttributes(msg, COM_VERSION, "1", MsgInLogName::state, P_LOG_FROM, "1377684000", P_LOG_TO, "1377684090", P_LOG_FUNC_TYPE, "avg", P_LOG_INTERVAL, "0", 
            P_DEVICE_ID, "0.0.20.1", P_DEVICE_TYPE, "1", P_ADAPTER_ID, "20");
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = getState(doc);
    
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
    createMsgInWithAttributes(msg, COM_VERSION, "1", MsgInLogName::state, P_LOG_FROM, "1414086980", P_LOG_TO, "1414086999", P_LOG_FUNC_TYPE, "avg", P_LOG_INTERVAL, "3", 
            P_DEVICE_ID, "0.0.21.1", P_DEVICE_TYPE, "1", P_ADAPTER_ID, "21");

    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = getState(doc);
    
    ostringstream oss;
    pugi::xml_node node = doc->child(P_COMMUNICATION);
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    it->print(oss,"",pugi::format_raw);
    string log = oss.str();
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),MsgOutLogContent::state, state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(response.c_str(),(string)"<row>1414086983 122.00</row><row>1414086987 110.00</row><row>1414086985 80.00</row>", log);
}
void testMsgInLogName::testLogsInterval2() {
    CPPUNIT_ASSERT_MESSAGE(NOT_IMPLEMENTED, false);
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
    
    CPPUNIT_ASSERT_MESSAGE(NOT_IMPLEMENTED, false);
    char msg[300];
    createMsgInWithAttributes(msg, COM_VERSION, "1", MsgInLogName::state, P_LOG_FROM, "1414086983", P_LOG_TO, "1414086985", P_LOG_FUNC_TYPE, "avg", P_LOG_INTERVAL, "0", 
            P_DEVICE_ID, "0.0.21.1", P_DEVICE_TYPE, "1", P_ADAPTER_ID, "21");

    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = getState(doc);
    
    ostringstream oss;
    pugi::xml_node node = doc->child(P_COMMUNICATION);
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    it->print(oss,"",pugi::format_raw);
    string log = oss.str();
    
//    CPPUNIT_ASSERT_EQUAL_MESSAGE(msg,MsgOutLogContent::state, state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE(msg,(string)"<row>1414086983 122</row><row>1414086984 50</row><row>1414086985 100</row>", log);
}

void testMsgInLogName::testWrongDevice() {
    
    CPPUNIT_ASSERT_MESSAGE(NOT_IMPLEMENTED, false);
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