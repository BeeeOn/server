/* 
 * File:   MsgInGetXml.cpp
 * Author: pavel
 * 
 * Created on 3. červenec 2014, 13:03
 */

#include "MsgInGetXml.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"

using namespace std;

const std::string MsgInGetXml::state = "getxml";

MsgInGetXml::MsgInGetXml(char* msg, pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(msg, doc) {
}

MsgInGetXml::~MsgInGetXml() {
}

IMsgOut* MsgInGetXml::createResponseMsgOut()
{
        if( !isComIdValid())
                return new MsgOutReSign();
          
        return new MsgOutCommunication(_comId, _adapterId, _state );
}
