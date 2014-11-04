
#include "MsgInSignIn.h"
#include "ServerException.h"
using namespace std;
const std::string MsgInSignIn::state = "signin";

MsgInSignIn::MsgInSignIn(char* msg, pugi::xml_document* doc): IMsgInLoginUnwanted(msg, doc)
{
}

MsgInSignIn::~MsgInSignIn(void)
{
}

int MsgInSignIn::getMsgAuthorization(){
    return EVERYONE;
}

string MsgInSignIn::createResponseMsgOut()
{
    
        pugi::xml_node comNode =  _doc->child(P_COMMUNICATION);
        string email = comNode.attribute(P_EMAIL).value();
        string gToken = comNode.attribute(P_GOOGLE_TOKEN).value();
        string phoneLocale = comNode.attribute(P_LOCALIZATION).value();
        //TODO gcmid
        string gcmid = comNode.attribute(P_GOOGLE_CLOUD_MID).value();

        if(phoneLocale != "cs" && phoneLocale != "en" && phoneLocale != "sk")
            throw ServerException(ServerException::LOCALE);
        
        googleInfo gInfo;
        if( !isGTokenOk(gToken, email, gInfo) )
            throw ServerException(ServerException::TOKEN_EMAIL);
        
        
        
        int userId = DBConnector::getInstance().getUserId(email);
        if(  userId > 0)
        {
            //TODO DBConnector::getInstance().updatePhoneLocale(email,phoneLocale);
            try{
                DBConnector::getInstance().updateUserGoogleInformation(userId, gInfo);
            }catch(...){
                   Logger::getInstance(Logger::ERROR) << "google info update failed"<<endl;
            }
            _comId = ComTable::getInstance().setComInfo(userId ,email);
             return envelopeResponse(R_TRUE);
        }
        else if( DBConnector::getInstance().isAnyUnregistredAdapter() )
            throw ServerException(ServerException::NOTREG_A);
        else
            throw ServerException(ServerException::NOTREG_B);
}
