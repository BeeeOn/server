#ifndef DAOUSERS_H
#define	DAOUSERS_H

#include "DAO.h"
#include "DAOGateways.h"
#include "DAOMobileDevices.h"

struct UsersColumns{
    std::string id;
    std::string first_name;
    std::string surname;
    std::string picture;
    std::string gender;
    std::string mail;
    std::string password;
    std::string google_id;
    std::string facebook_id;
    
    UsersColumns():
            id("user_id"), 
            first_name("given_name"), 
            surname("family_name"),
            picture("picture"),
            gender("gender"),
            mail("mail"),
            password("password"),
            google_id("google_id"),
            facebook_id("facebook_id")
            { }
};

struct User
{
   int user_id;
   std::string mail;
   std::string password;
   std::string givenName;
   std::string familyName;
   std::string picture;
   std::string gender;
   std::string googleId;
   std::string facebookId;
};

class DAOUsers:public DAO{
public:
    DAOUsers();

    ~DAOUsers(void);
    
    std::string getDAOname();
    
    int add(User user);
    
    User getUserAssociatedWithToken(std::string token);
    User getUserByID(int userId);
    
    int getUserIdbyIhaToken(std::string token);
    int getUserIDbyAlternativeKeys(std::string mail, std::string password, std::string google_id, std::string facebook_id);
    bool isMailRegistred(std::string mail);
    bool isNameRegistred(std::string name);
    bool isGoogleIdRegistred(std::string g_id);
    std::string getUserRoleM(int userId, gatewayId_64b gateId);
    bool isGateParred(int userId, gatewayId_64b gateId);
    std::string getXMLusersGates(int userId);
    
    static const std::string tableUsers;
    static const UsersColumns col;
};

#endif	/* DAOUSERS_H */

