
#include "UserLogIn.h"
#include "ServerException.h"
#include "../DAO/DAOUsers.h"
#include "../DAO/DAOMobileDevices.h"

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
     
     _token = getnewIHAtoken();
     mobile.token = _token;
     mobile.type = "android";
    Logger::debug3()<<"tok:"<< _token  <<endl;  
    User user;
    
    if(service == "google"){
        string gToken = parametersNode.attribute(P_GOOGLE_TOKEN).value();       
                
        googleInfo gInfo;
        
        Logger::debug3()<<"isGTokenOk"<<endl;
        
        if( !isGoogleTokenOkayCURL(gToken, gInfo) )
        {
            _outputMainNode.append_attribute(P_ERRCODE) = ServerException::TOKEN_EMAIL;
            return getXMLreply(R_FALSE);
        }

        user.familyName = gInfo.family_name;
        user.gender = gInfo.gender;
        user.givenName = gInfo.given_name;
        user.googleId = gInfo.id;
        user.mail = gInfo.email;
        user.picture = gInfo.picture;
    
    }else  if(service == "facebook"){
        
        facebookInfo fInfo;
                
        string fbToken = parametersNode.attribute(P_FACEBOOK_TOKEN).value(); 
        
        Logger::debug3()<<"isFBTokenOk"<<endl;       
        if ( !isFTokenOkay(fbToken, fInfo))
        {
            _outputMainNode.append_attribute(P_ERRCODE) = ServerException::TOKEN_EMAIL;
            return getXMLreply(R_FALSE);
        }

        if ( fInfo.email == "" )
        {
            _outputMainNode.append_attribute(P_ERRCODE) = ServerException::NO_MAIL_PROVIDED;
            return getXMLreply(R_FALSE);
        }
        
        user.familyName = fInfo.last_name;
        user.gender = fInfo.gender;
        user.givenName = fInfo.first_name;
        user.facebookId = fInfo.id;
        user.mail = fInfo.email;
        user.picture = fInfo.link;
        
    }else  if(service == "beeeon"){
        //string userName = parametersNode.attribute("name").value();       
        //string userPassword = parametersNode.attribute("pswd").value();       
    }else{
        Logger::error() << "unsupported provider : " << service <<endl;
        _outputMainNode.append_attribute(P_ERRCODE) = ServerException::WRONG_AUTH_PROVIDER;
        return getXMLreply(R_FALSE);
    }
    
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

    
    _outputMainNode.append_attribute(P_SESSION_ID) = _token.c_str();
    return getXMLreply(R_BEEEON_TOKEN);
}

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
        
     /*   ifstream urandom("/dev/random", ios::binary);
        if(urandom.is_open())
            cerr <<"!!!!!!!"<<endl;
        urandom.read(block,len);
        urandom.close();
     
        
        for (int i = 0; i < len; ++i) {
            newToken += alphanum[block[i] % (sizeof(alphanum) - 1)];  
        }
 */
        Logger::debug3()<<"gen: " << newToken << endl;
        r = DBConnector::getInstance().DEBUGexec("select  count(token)  from mobile_devices where token='"+newToken+"'" );
        
    }while(r != "0");
    
    return newToken;
}
