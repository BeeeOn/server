/* 
 * File:   ServerException.cpp
 * Author: pavel
 * 
 * Created on 17. červenec 2014, 13:46
 */

#include <string.h>

#include "ServerException.h"

using namespace std;

ServerException::ServerException(int errCode) : runtime_error("x") {
    _errCode = errCode;
    _errText ="";
}

ServerException::ServerException(int errCode, string errText)  : runtime_error("x") {
    _errCode = errCode;
    _errText = errText;
}
/*
ServerException::ServerException(int errCode, string errText, string id, string state)  : runtime_error("x") {
    _errCode = errCode;
    _errText = errText;
    _id = id;
    _state = state;
}*/
/*
 ServerException::~ServerException() {
 }*/

const char* ServerException::what() const throw()
{
    //TODO number or longer text
    return "Server exception: ";
}

int ServerException::getErrCode(){
    return _errCode;
}
string ServerException::getErrText(){
    return _errText;
}  /*
    string ServerException::getId(){
        return _id;
    }
    string ServerException::getState(){
        return _state;
    }*/