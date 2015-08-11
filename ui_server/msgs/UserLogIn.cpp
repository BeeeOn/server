
#include "UserLogIn.h"
#include "ServerException.h"
#include "../DAO/DAOUsers.h"
#include "../DAO/DAOMobileDevices.h"
#include "SessionsTable.h"
#include "TokenChecker.h"

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
const std::string UserLogIn::state = "signin";


UserLogIn::UserLogIn(pugi::xml_document* doc): IMsgInFreeAccess(doc)
{
}


UserLogIn::~UserLogIn(void)
{
}

int UserLogIn::getMsgAuthorization() {
    return EVERYONE;
}

string UserLogIn::createResponseMsgOut()
{
    Logger::debug()<<"loging in"<<endl;
    pugi::xml_node parametersNode =  _doc->child(P_COMMUNICATION).child(P_SIGN_PARAMS);
    
    string service = _doc->child(P_COMMUNICATION).attribute(P_SIGN_SERVICE).value();
    
    string phoneId =  _doc->child(P_COMMUNICATION).attribute(P_PHONE_ID).value();
    string phoneLocale =  _doc->child(P_COMMUNICATION).attribute(P_LOCALIZATION).value();
            
     MobileDevice mobile;
     mobile.locale = phoneLocale;
     mobile.mobile_id = phoneId;
     mobile.push_notification = "";
     
     //_token = getnewIHAtoken();
     //mobile.token = _token;
     mobile.type = "android";
    //Logger::debug3()<<"tok:"<< _token  <<endl;  
    User user;
    
    string token;
    if(service == "google")
    {
        token = parametersNode.attribute(P_GOOGLE_TOKEN).value();    
    }
    else if(service == "facebook")
    {
        token = parametersNode.attribute(P_FACEBOOK_TOKEN).value(); 
    }
    else
    {
        Logger::error() << "unsupported provider : " << service <<endl;
        return getNegativeXMLReply(ServerException::WRONG_AUTH_PROVIDER);
    }
    
    TokenChecker tokenChecker(service);
    
    if( !tokenChecker.isTokenOkay(token) )
    {
        return getNegativeXMLReply(ServerException::TOKEN_EMAIL);
    }
    
    if ( tokenChecker.email == "" )
    {
        _outputMainNode.append_attribute(P_ERRCODE) = ServerException::NO_MAIL_PROVIDED;
        return getXMLreply(R_FALSE);
    }
    
    if(service == "google")
    {
        user.googleId = tokenChecker.id;
    }
    else if(service == "facebook")
    {
        user.facebookId = tokenChecker.id;
    }
    
    user.familyName = tokenChecker.family_name;
    user.gender = tokenChecker.gender;
    user.givenName = tokenChecker.given_name;
    user.mail = tokenChecker.email;
    user.picture = tokenChecker.picture;  
    
    
    int userId = DAOUsers::getInstance().getUserIDbyAlternativeKeys(user.mail, user.googleId, user.facebookId);
    
    
    if(userId < 0)
    {
        _outputMainNode.append_attribute(P_ERRCODE) = ServerException::USER_DONOT_EXISTS;
        return getXMLreply(R_FALSE);
    }
    if(DAOUsers::getInstance().upsertUserWithMobileDevice(user, mobile) == 0 )
    {
        _outputMainNode.append_attribute(P_ERRCODE) = ServerException::USER_DONOT_EXISTS;
        return getXMLreply(R_FALSE);
    }
    
    string ttoken = SessionsTable::getInstance().addNewSession(userId, phoneId);
    
    _outputMainNode.append_attribute(P_SESSION_ID) = ttoken.c_str();
    return getXMLreply(R_BEEEON_TOKEN);
}
/*
string UserLogIn::getnewIHAtoken() {
    
    string r;
    string newToken;
    int i=0;
    do{
        i++;
        if(i >= 5)
            throw ServerException(ServerException::RANDOMGEN);
        
        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

        int fd = open("/dev/urandom", O_RDONLY);
        if (fd != -1) {
            // uint8_t buffer[len];
            int len = 10;
            char buffer[len];
            if (read(fd, buffer, sizeof (buffer)) == (int)sizeof (buffer)) {
                
                for (int i = 0; i < len; ++i) {
                    newToken += alphanum[buffer[i] % (sizeof (alphanum) - 1)];
                }
            } else {
                continue;
            }
        }
        
     
        Logger::debug3()<<"gen: " << newToken << endl;
        r = DBConnector::getInstance().DEBUGexec("select  count(token)  from mobile_devices where token='"+newToken+"'" );
        
    }while(r != "0");
    
    return newToken;
}
*/