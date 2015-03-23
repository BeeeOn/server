
#include "MsgInSignMe.h"
#include "ServerException.h"
#include "../DAO/DAOUsers.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
const std::string MsgInSignMe::state = "getuid";

long long int MsgInSignMe::_IHAtokenGenerator = 100;

MsgInSignMe::MsgInSignMe(char* msg, pugi::xml_document* doc): IMsgInLoginUnwanted(msg, doc)
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
    string gId  =  _doc->child(P_COMMUNICATION).attribute(P_GOOGLE_ID).value();
    string gToken = _doc->child(P_COMMUNICATION).attribute(P_GOOGLE_TOKEN).value();
    string phoneId = _doc->child(P_COMMUNICATION).attribute(P_PHONE_ID).value();
    string phoneLocale = _doc->child(P_COMMUNICATION).attribute(P_LOCALIZATION).value();
    
    googleInfo gInfo;
    //DEBUG
    if(gInfo.email == "")
        gInfo.email = phoneId+"@debug";
    
    if( !isGTokenOk(gToken, gId, gInfo) )
        throw ServerException(ServerException::TOKEN_EMAIL);
    
    User user;
    user.familyName = gInfo.family_name;
    user.gender = gInfo.gender;
    user.givenName = gInfo.given_name;
    user.googleId = gInfo.id;
    user.googleLocale = gInfo.locale;
    user.link = gInfo.link;
    user.mail = gInfo.email;
    user.name = "todo";
    user.phoneLocale = gInfo.locale;
    user.picture = gInfo.picture;
    user.verifiedMail = gInfo.verified_email;
            
     MobileDevice mobile;
     mobile.locale = phoneLocale;
     mobile.mobile_id = phoneId;
     mobile.push_notification = "";
     
     _IHAtoken = getnewIHAtoken();
     mobile.token = _IHAtoken;
     mobile.type = "android";
     
    DAOUsers::getInstance().upsertUserWithMobileDevice(user, mobile);
    //TODO upsert
  /*  if( DBConnector::getInstance().insertNewUser(gId, gInfo) == 0)
            Logger::getInstance(Logger::DEBUG3)<<gId<<" already exist?"<<endl;
    
       long long int IHAtoken = getnewIHAtoken();
       
    if( DBConnector::getInstance().insertNewMobileDevice(IHAtoken, gId, phoneId, phoneLocale) == 0)
        throw ServerException(ServerException::TOKEN_EMAIL);
       
       //string attr = makeXMLattribute(P_SESSION_ID, to_string(IHAtoken));*/
  
    _mainNode.append_attribute(P_SESSION_ID) = _IHAtoken.c_str();
    pugi::xml_document doc;
    doc.load("<foo bar='baz'><call>hey</call></foo>");
    _mainNode.append_copy(doc);
    return genOutputXMLwithVersionAndState(R_BEEEON_TOKEN);
    //return envelopeResponseWithAttributes(R_BEEEON_TOKEN, P_SESSION_ID "=\""+_IHAtoken+"\" ");
}

string MsgInSignMe::getnewIHAtoken() {
    string r;
    string newToken;
    int i=0;
    do{
        i++;
        if(i >= 5)
            throw ServerException(ServerException::RANDOMGEN);
        char * block;
        int len = 10;

        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

        int fd = open("/dev/urandom", O_RDONLY);
        if (fd != -1) {
            // uint8_t buffer[len];
            char buffer[len];
            if (read(fd, buffer, sizeof (buffer)) == sizeof (buffer)) {
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
