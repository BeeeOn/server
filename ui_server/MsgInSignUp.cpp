
#include "MsgInSignUp.h"
#include "ServerException.h"
const std::string MsgInGetUID::state = "getuid";

long long int MsgInGetUID::_IHAtokenGenerator = 100;

MsgInGetUID::MsgInGetUID(char* msg, pugi::xml_document* doc): IMsgInLoginUnwanted(msg, doc)
{
}


MsgInGetUID::~MsgInGetUID(void)
{
}

int MsgInGetUID::getMsgAuthorization() {
    return EVERYONE;
}

string MsgInGetUID::createResponseMsgOut()
{
    Logger::getInstance(Logger::DEBUG)<<"GUID"<<"\n";
    string gId  =  _doc->child(P_COMMUNICATION).attribute(P_GOOGLE_ID).value();
    string gToken = _doc->child(P_COMMUNICATION).attribute(P_GOOGLE_TOKEN).value();
    string phoneLocale = _doc->child(P_COMMUNICATION).attribute(P_LOCALIZATION).value();
    
    googleInfo gInfo;
    if( !isGTokenOk(gToken, gId, gInfo) )
        throw ServerException(ServerException::TOKEN_EMAIL);
    
    if( DBConnector::getInstance().insertNewUser(gId, gInfo) == 0)
            Logger::getInstance(Logger::DEBUG3)<<gId<<" already exist?"<<"\n";
    
       long long int IHAtoken = getnewIHAtoken();
    
    if( DBConnector::getInstance().insertNewIHAtoken(IHAtoken, gId) == 0)
        throw ServerException(ServerException::TOKEN_EMAIL);
       
       //string attr = makeXMLattribute(P_SESSION_ID, to_string(IHAtoken));
       _IHAtoken = IHAtoken;
    return envelopeResponse(R_UID);
}

long long int MsgInGetUID::getnewIHAtoken() {
    _IHAtokenGenerator++;
    return _IHAtokenGenerator;
}
