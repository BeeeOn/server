/* 
 * File:   DAOUsers.h
 * Author: pavel
 *
 * Created on 16. březen 2015, 20:03
 */

#ifndef DAOUSERS_H
#define	DAOUSERS_H

#include "DAO.h"

#include "DAOMobileDevices.h"
struct User
{
    int user_id;
    std::string mail;
    std::string password;
   std:: string phoneLocale;
   int verifiedMail;
   std::string name;
   std::string givenName;
   std::string familyName;
   std::string link;
   std::string picture;
   std::string gender;
   std::string googleLocale;
  std::string googleId;
};

namespace soci
{
    template<>
    struct type_conversion<User>
    {
        typedef values base_type;

        static void from_base(values const & v, indicator /* ind */, User & user)
        {
            user.user_id = v.get<int>("user_id");
            user.mail = v.get<std::string>("mail","");
            user.password = v.get<std::string>("password","");
            user.phoneLocale = v.get<std::string>("phone_locale","");
            user.verifiedMail = v.get<int>("verified_email");
            user.name = v.get<std::string>("name","");
            user.givenName = v.get<std::string>("given_name","");
            user.familyName = v.get<std::string>("family_name","");
            user.link = v.get<std::string>("link","");
            user.picture = v.get<std::string>("picture","");
            user.gender = v.get<std::string>("gender","");
            user.googleLocale = v.get<std::string>("google_locale","");
            user.googleId = v.get<std::string>("google_id","");
        }
    
        static void to_base(const User & user, values & v, indicator & ind)
        {           
            v.set("user_id", user.user_id);
            v.set("mail", user.mail, user.mail.empty() ? i_null : i_ok);
            v.set("password", user.password, user.password.empty() ? i_null : i_ok);
            v.set("phone_locale", user.phoneLocale, user.phoneLocale.empty() ? i_null : i_ok);
            v.set("verified_email", user.verifiedMail);
            v.set("name", user.name, user.name.empty() ? i_null : i_ok);
            v.set("given_name", user.givenName, user.givenName.empty() ? i_null : i_ok);
            v.set("family_name", user.familyName, user.familyName.empty() ? i_null : i_ok);
            v.set("link", user.link, user.link.empty() ? i_null : i_ok);
            v.set("picture", user.picture, user.picture.empty() ? i_null : i_ok);
            v.set("gender", user.gender, user.gender.empty() ? i_null : i_ok);
            v.set("google_locale", user.googleLocale, user.googleLocale.empty() ? i_null : i_ok);
            v.set("google_id", user.googleId, user.googleId.empty() ? i_null : i_ok);
            ind = i_ok;
        }
    };
}



class DAOUsers:public DAO{
private:
    DAOUsers();
    DAOUsers(DAOUsers const&);// Don't Implement
    void operator=(DAOUsers const&); // Don't implement
public:
    static DAOUsers& getInstance();
    ~DAOUsers(void);
    
    int add(User user);
    int getUserIdbyIhaToken(std::string token);
    int upsertUserWithMobileDevice(User user, MobileDevice mobile);
    bool isMailRegistred(std::string mail);
    bool isNameRegistred(std::string name);
    bool isGoogleIdRegistred(std::string g_id);
    std::string getUserID(std::string Btoken);
    User getUserAssociatedWithToken(std::string token);
    std::string getUserRoleM(int userId, std::string adapterId);
    std::string getXMLusersAdapters(int userId);
};



#endif	/* DAOUSERS_H */

