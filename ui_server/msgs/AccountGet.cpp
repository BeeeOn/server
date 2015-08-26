/* 
 * File:   MsgInGetConAccount.cpp
 * Author: pavel
 * 
 * Created on 2. červenec 2014, 11:23
 */

#include <string.h>

#include "AccountGet.h"
#include "../DAO/DAOUsersAdapters.h"


using namespace std;

const string AccountGet::state = "getgateusers";
AccountGet::AccountGet(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc)  {
}

AccountGet::~AccountGet() {
}

int AccountGet::getMsgAuthorization() {
    return permissions::guest;
}

string AccountGet::createResponseMsgOut()
{     /*
    string xml = DAOUsersAdapters::getInstance().getXMLconAccounts(_adapterId);
    pugi::xml_document doc;
    cout<<xml<<endl;
    xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + xml;
   
    char *cstr = new char[xml.length() + 1];
    strcpy(cstr, xml.c_str());
    // do stuff
    cout<< "cstr" <<  cstr <<endl; 
    pugi::xml_node n;
    //pugi::xml_parse_result result = doc.load("<user userid=\"1\" email=\"pvampola@gmail.com\" role=\"permissions::superuser\" name=\"Pavel\" surname=\"Vampola\" gender=\"other\"/>");
    pugi::xml_parse_result result = doc.load(cstr);
    if (!result)
    {
        Logger::error()<< "XML [] parsed with errors" << endl;
        Logger::error()<< "Error description: " << result.description() << endl;
        Logger::error() << "Error offset: " << result.offset << " (error at [..." << (cstr + result.offset) << "]"<<endl;
    }
    
    delete [] cstr;
    _outputMainNode.append_copy(doc.first_child());
    return getXMLreply(proto::replyAccounts);*/
    return envelopeResponse(proto::replyAccounts, DAOUsersAdapters::getInstance().getXMLconAccounts(_gatewayId));
}
