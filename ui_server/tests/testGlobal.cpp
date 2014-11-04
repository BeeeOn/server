/*
 * File:   testGlobal.cpp
 * Author: pavel
 *
 * Created on 22.7.2014, 15:53:11
 */

#include "testGlobal.h"
#include "../testHelper.h"

CPPUNIT_TEST_SUITE_REGISTRATION(testGlobal);

testGlobal::testGlobal() {
}

testGlobal::~testGlobal() {
}

void testGlobal::setUp() {
    DBConnector::getInstance().setConnectionStringAndOpenSessions(TESTDB1_CON_STRING);
    Logger::getInstance().setVerbosityThreshold(Logger::NO_OUTPUT);
}

void testGlobal::tearDown() {
}

void testGlobal::testWrongVersion() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=","0.0"," id=\"0\" state=\"signin\">"
            "<user email=\"xxx@xxx\" gtoken=\"1\"  locale=\"en\" />"
            "</communication>"
            );
    
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    string  errCode = doc->child("communication").attribute("errcode").value();
    
    if(state != "false")
        CPPUNIT_ASSERT_EQUAL((string)"false", state);
    CPPUNIT_ASSERT_EQUAL(0, stoi(errCode) );
}

void testGlobal::testWrongMsg() {
    char msg[200];
    sprintf (msg, "%s%s%s", 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<communication version=",COM_VERSION," id=\"1\" state=\"???\">"
            "<user email=\"xxx@xxx\" gtoken=\"1\"  locale=\"en\" />"
            "</communication>"
            );
    
    string response = resolveMsg(msg);
    
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(response.c_str());
    
    string  state = doc->child("communication").attribute("state").value();
    
    CPPUNIT_ASSERT_EQUAL((string)"false", state);

}

//TODO pristup k spatnemu adapteru, device, zkontrolovat prava