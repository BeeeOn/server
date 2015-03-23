/* 
 * File:   DAOUsers.h
 * Author: pavel
 *
 * Created on 16. březen 2015, 20:03
 */

#ifndef DAOUSERS_H
#define	DAOUSERS_H

#include "DAO.h"

struct User
{
    int user_id;
    std::string mail;
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
            user.mail = v.get<std::string>("mail");
            user.phoneLocale = v.get<std::string>("phone_locale");
            user.verifiedMail = v.get<int>("verified_email");
            user.name = v.get<std::string>("name");
            user.givenName = v.get<std::string>("given_name");
            user.familyName = v.get<std::string>("family_name");
            user.link = v.get<std::string>("link");
            user.picture = v.get<std::string>("picture");
            user.gender = v.get<std::string>("gender");
            user.googleLocale = v.get<std::string>("google_locale");
            user.googleId = v.get<std::string>("google_id");
        }
    /*
        static void to_base(const User & p, values & v, indicator & ind)
        {
            v.set("ID", user.id);
            v.set("FIRST_NAME", user.firstName);
            v.set("LAST_NAME", user.lastName);
            v.set("GENDER", user.gender, p.gender.empty() ? i_null : i_ok);
            ind = i_ok;
        }
        */
    };
}

struct MobileDevice
{
    std::string token;
    std::string mobile_id;
    std::string type;
    std::string locale;
    std::string push_notification;
    long long int user_id;    
};

class DAOUsers:public DAO{
private:
    DAOUsers();
    DAOUsers(DAOUsers const&);// Don't Implement
    void operator=(DAOUsers const&); // Don't implement
public:
    static DAOUsers& getInstance();
    ~DAOUsers(void);
    
    int add(User user);
    int upsertUserWithMobileDevice(User user, MobileDevice mobile);
    bool isMailRegistred(std::string mail);
    std::string getUserID(std::string Btoken);
    User getUserAssociatedWithToken(std::string token);
};



#endif	/* DAOUSERS_H */

