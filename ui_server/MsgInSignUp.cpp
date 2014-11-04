
#include "MsgInSignUp.h"
#include "ServerException.h"
const std::string MsgInSignUp::state = "signup";
MsgInSignUp::MsgInSignUp(char* msg, pugi::xml_document* doc): IMsgInLoginUnwanted(msg, doc)
{
}


MsgInSignUp::~MsgInSignUp(void)
{
}

int MsgInSignUp::getMsgAuthorization() {
    return EVERYONE;
}

string MsgInSignUp::createResponseMsgOut()
{
    Logger::getInstance(Logger::DEBUG)<<"register new user"<<"\n";
    string mail  =  _doc->child(P_COMMUNICATION).attribute(P_EMAIL).value();
    string gToken = _doc->child(P_COMMUNICATION).attribute(P_GOOGLE_TOKEN).value();
    
    googleInfo gInfo;
    if( !isGTokenOk(gToken, mail, gInfo) )
        throw ServerException(ServerException::TOKEN_EMAIL);
    
    if( DBConnector::getInstance().insertNewUser(mail) == 0)
        throw ServerException(ServerException::TOKEN_EMAIL);
    
    return envelopeResponse(R_TRUE);
}