/*
 * File:   newtestclassBeta.cpp
 * Author: pavel
 *
 * Created on 6.8.2014, 11:22:39
 */

#include "newtestclassBeta.h"
#include "MsgInLogName.h"


CPPUNIT_TEST_SUITE_REGISTRATION(newtestclassBeta);

newtestclassBeta::newtestclassBeta() {
}

newtestclassBeta::~newtestclassBeta() {
}

void newtestclassBeta::setUp() {
}

void newtestclassBeta::tearDown() {
}

void newtestclassBeta::testCreateResponseMsgOut() {
    MsgInLogName msgInLogName;
    IMsgOut* result = msgInLogName.createResponseMsgOut();
    if (true /*check result*/) {
        CPPUNIT_ASSERT(false);
    }
}

void newtestclassBeta::testGetMsgAuthorization() {
    MsgInLogName msgInLogName;
    int result = msgInLogName.getMsgAuthorization();
    if (true /*check result*/) {
        CPPUNIT_ASSERT(false);
    }
}

