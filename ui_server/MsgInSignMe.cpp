
#include "MsgInSignMe.h"
#include "ServerException.h"
#include "../DAO/DAOUsers.h"
#include "../DAO/DAOMobileDevices.h"

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
const std::string MsgInSignMe::state = "signin";


MsgInSignMe::MsgInSignMe(char* msg, pugi::xml_document* doc): IMsgInFreeAccess(msg, doc)
{
}


MsgInSignMe::~MsgInSignMe(void)
{
}

int MsgInSignMe::getMsgAuthorization() {
    return EVERYONE;
}

string MsgInSignMe::createResponseMsgOut()
{
    Logger::getInstance(Logger::DEBUG)<<"GUID"<<endl;
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
    
    User user;
    
    if(service == "google"){
        string gToken = parametersNode.attribute(P_GOOGLE_TOKEN).value();       
                
        googleInfo gInfo;

        if( !isGTokenOk(gToken, gInfo) )
            throw ServerException(ServerException::TOKEN_EMAIL);

        user.familyName = gInfo.family_name;
        user.gender = gInfo.gender;
        user.givenName = gInfo.given_name;
        user.googleId = gInfo.id;
        user.mail = gInfo.email;
        user.picture = gInfo.picture;
    
    }else  if(service == "facebook"){
        
        facebookInfo fInfo;
                
        string fbToken = parametersNode.attribute(P_FACEBOOK_TOKEN).value();   
        if ( !isFTokenOkay(fbToken, fInfo))
            throw ServerException(ServerException::TOKEN_EMAIL);

        user.familyName = fInfo.last_name;
        //user.gender = fInfo.gender;
        user.givenName = fInfo.first_name;
        user.googleId = fInfo.id;
        user.mail = fInfo.email;
        //user.picture = fInfo.picture;
        
    }else  if(service == "beeeon"){
        string userName = parametersNode.attribute("name").value();       
        string userPassword = parametersNode.attribute("pswd").value();       
    }else{
        Logger::error() << "unsupported provider : " << service <<endl;
        throw ServerException(ServerException::WRONG_AUTH_PROVIDER);
    }
    
    int userId = DAOUsers::getInstance().getUserIDbyAlternativeKeys(user.mail, user.googleId, user.facebookId);
    
    
    if(userId < 0)
        throw ServerException(ServerException::USER_DONOT_EXISTS);
    
    if(DAOUsers::getInstance().upsertUserWithMobileDevice(user, mobile) == 0 )
         throw ServerException(ServerException::USER_DONOT_EXISTS);

    _outputMainNode.append_attribute(P_SESSION_ID) = _token.c_str();
    return genOutputXMLwithVersionAndState(R_BEEEON_TOKEN);
}

string MsgInSignMe::getnewIHAtoken() {
    string r;
    string newToken;
    int i=0;
    do{
        i++;
        if(i >= 5)
            throw ServerException(ServerException::RANDOMGEN);
        
        int len = 10;

        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

        int fd = open("/dev/urandom", O_RDONLY);
        if (fd != -1) {
            // uint8_t buffer[len];
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
        r = DBConnector::getInstance().DEBUGexec("select  count(token)  from mobile_devices where token='"+newToken+"'" );
    
    }while(r != "0");
    
    return newToken;
}
