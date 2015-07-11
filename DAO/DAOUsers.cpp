/* 
 * File:   DAOUsers.cpp
 * Author: pavel
 * 
 * Created on 16. březen 2015, 20:03
 */

#include "DAOUsers.h"
#include "../ui_logger/Logger.h"
#include "Config.h"

using namespace std;
using namespace soci;



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
            user.passwordMd5 = v.get<std::string>("password","");
            //user.phoneLocale = v.get<std::string>("phone_locale","");
            //user.verifiedMail = v.get<int>("verified_email",1);
            //user.name = v.get<std::string>("name","");
            user.givenName = v.get<std::string>("given_name","");
            user.familyName = v.get<std::string>("family_name","");
            //user.link = v.get<std::string>("link","");
            user.picture = v.get<std::string>("picture","");
            user.gender = v.get<std::string>("gender","");
            //user.googleLocale = v.get<std::string>("google_locale","");
            user.googleId = v.get<std::string>("google_id","");
            user.facebookId = v.get<std::string>("facebook_id","");
        }
    
        static void to_base(const User & user, values & v, indicator & ind)
        {           
            v.set("user_id", user.user_id);
            v.set("mail", user.mail, user.mail.empty() ? i_null : i_ok);
            v.set("password", user.passwordMd5, user.passwordMd5.empty() ? i_null : i_ok);
            ///v.set("phone_locale", user.phoneLocale);
            //v.set("verified_email", user.verifiedMail, user.name.empty() ? i_null : i_ok);
            //v.set("name", user.name, user.name.empty() ? i_null : i_ok);
            v.set("given_name", user.givenName, user.givenName.empty() ? i_null : i_ok);
            v.set("family_name", user.familyName, user.familyName.empty() ? i_null : i_ok);
            //v.set("link", user.link, user.link.empty() ? i_null : i_ok);
            v.set("picture", user.picture, user.picture.empty() ? i_null : i_ok);
            v.set("gender", user.gender, user.gender.empty() ? i_null : i_ok);
            //v.set("google_locale", user.googleLocale, user.googleLocale.empty() ? i_null : i_ok);
            v.set("google_id", user.googleId, user.googleId.empty() ? i_null : i_ok);
            v.set("facebook_id", user.facebookId, user.facebookId.empty() ? i_null : i_ok);
            ind = i_ok;
        }
    };
}

DAOUsers::DAOUsers(){
}


DAOUsers::~DAOUsers() {
}

DAOUsers& DAOUsers::getInstance(){
        static DAOUsers instance;
        return instance;
}

int DAOUsers::add(User user) {
        Logger::db() << "DB:" << "insertNewUser" << endl;
        try
        {
                soci::session sql(*_pool);
                
                /*sql << "insert into users(mail, phone_locale, verified_email, name, given_name, family_name, link, picture, gender, google_locale, google_id) "
                                                " values(:mail, :phoneLoc, :verMail,:name, "
                                            " :name1, :name2, :link, :pic, :gen, :g_loc, gid)",
                        use (user.mail,"mail"), use(user.phoneLocale, "phoneLoc"), use(user.verifiedMail, "verMail"), use(user.name, "name"),
                        use(user.givenName, "name1"), use(user.familyName, "name2"), use(user.link, "link"),use(user.picture, "pic"), use(user.gender, "gen"), use(user.googleLocale, "g_loc"), use(user.googleId, "gid");   
                */
                sql << "insert into users(mail, password, given_name, family_name, picture, gender, google_id, facebook_id) "
                                                " values(:mail, :password, "
                                            " :given_name, :family_name, :picture, :gender, :google_id, :facebook_id)", use(user);
                return 1;
        }
        catch (soci::postgresql_soci_error& e)
        {
                Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() <<endl;
                return 0;
        }
}

User DAOUsers::getUserAssociatedWithToken(std::string token) {
    Logger::db()<<"DB: get user by token:"<<token << endl;
    
    try
    {
            session sql(*_pool);
            User user;

            sql << "select * from users join mobile_devices on user_id = fk_user_id where token = :btoken", use(token, "btoken"),into(user);
            return user; 
    }
    catch (soci::postgresql_soci_error& e)
    {
            Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() <<endl;
            throw;
    }   
}

User DAOUsers::getUserByID(int userId) {
    Logger::db()<<"DB: get user by id:"<<userId << endl;
    
    try
    {
            session sql(*_pool);
            User user;

            sql << "select * from users where user_id = :id", use(userId, "id"),into(user);
            return user; 
    }
    catch (soci::postgresql_soci_error& e)
    {
            Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() <<endl;
            throw;
    }  
}



int DAOUsers::upsertUserWithMobileDevice(User user, MobileDevice mobile){
    session sql(*_pool);
    Logger::db()<<"DB:"<<"upsertUserWithMobileDevice"<<endl;
    try
    {
        soci::session sql(*_pool);

        int newUserID;
        //d_mail TEXT, d_locale TEXT, d_ver BOOLEAN, d_name TEXT, d_g_name TEXT, d_f_name TEXT, d_link TEXT, d_picture TEXT, d_gender TEXT, d_g_loc TEXT, d_g_id TEXT

        //sql.begin(); 
        sql << "select upsert_user_returning_uid(:mail,:given_name, :family_name, :picture, :gender, :google_id, :facebook_id)",
                use(user), into(newUserID);
        Logger::debug3() << "uid:" << newUserID << endl;
        //bon_token TEXT, d_mobile_id TEXT,d_type TEXT, d_locale TEXT, d_push_n TEXT, d_uid integer
        sql << "select upsert_mobile_device(:bt, :mobileID, :type, :loc, :pushN, :userID)",
                use(mobile.token), use(mobile.mobile_id), use(mobile.type), use(mobile.locale),use(mobile.push_notification),use(newUserID);
        //sql.commit();
        return 1;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() <<endl;
        return 0;
    }
}

bool DAOUsers::isMailRegistred(std::string mail) {
    Logger::db()<<"DB: is "<<mail<<" registred?" << endl;
    int c;
    try
    {
            session sql(*_pool);
            sql << "select count(*) from users where mail=:mail;", use(mail, "mail"),into(c);
            Logger::getInstance(Logger::DEBUG3)<<mail<< c << endl;
            return c==1; 
    }
    catch (soci::postgresql_soci_error& e)
    {
            Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() <<endl;
            throw;
    }
}
bool DAOUsers::isNameRegistred(std::string name) {
    Logger::db()<<"DB: is "<<name<<" registred?" << endl;
    int c;
    try
    {
            session sql(*_pool);
            sql << "select count(*) from users where name=:name;", use(name, "name"),into(c);
            Logger::getInstance(Logger::DEBUG3)<<name<< c << endl;
            return c==1; 
    }
    catch (soci::postgresql_soci_error& e)
    {
            Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() <<endl;
            throw;
    }
}
bool DAOUsers::isGoogleIdRegistred(std::string g_id) {
    Logger::db()<<"DB: is "<<g_id<<" registred?" << endl;
    int c;
    try
    {
            session sql(*_pool);
            sql << "select count(*) from users where google_id=:google_id;", use(g_id, "google_id"),into(c);
            Logger::getInstance(Logger::DEBUG3)<<g_id<< c << endl;
            return c==1; 
    }
    catch (soci::postgresql_soci_error& e)
    {
            Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() <<endl;
            throw;
    }
}

int DAOUsers::getUserIDbyAlternativeKeys(std::string mail, std::string google_id, std::string facebook_id){
    Logger::db()<<"DB: getUID by alternative keys:("<<mail << google_id << facebook_id <<")"<< endl;
    
    try
    {
            session sql(*_pool);
            int UID=-1;
            
            indicator i1 = mail.empty()?i_null:i_ok;
            indicator i2 = google_id.empty()?i_null:i_ok;
            indicator i3 = facebook_id.empty()?i_null:i_ok;
            sql << "select user_id from users where "
                                "CASE WHEN :mail::text is not NULL THEN mail=:mail  "
                                            "WHEN :google_id::text is not NULL then google_id=:google_id "
                                            "WHEN :fb_id::text is not NULL then facebook_id=:fb_id "
                                "END",
                    use(mail,i1,"mail"), use(google_id,i2,"google_id"),use(facebook_id,i3,"fb_id"),
                    into(UID);
            Logger::getInstance(Logger::DEBUG3)<<"uid is: "<<UID<< endl;
            return UID; 
    }
    catch (soci::postgresql_soci_error& e)
    {
            Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() <<endl;
            return -1;
    }   
}

int DAOUsers::getUserIdbyIhaToken(string token) {
        Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"iha token valid?"<<"\n";
        try
        {
                soci::session sql(*_pool);
                
                indicator ind;
                int userId = -1;
                sql << "select fk_user_id from mobile_devices where token = :token",
                        use (token,"token"), into(userId,ind);   
                return userId;
        }
        catch (soci::postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                return -1;
        }
}


        
string DAOUsers::getUserRoleM(int userId, string adapterId)
{
        Logger::getInstance(Logger::DEBUG3)<<"DB: get role "<<userId<<" on "<<adapterId<<endl;
        string role;
        try
        {
                session sql(*_pool);
                sql << "select role from users_adapters where fk_user_id= :guserid and fk_adapter_id = :adapter",
                    use(userId, "guserid"), use(adapterId, "adapter"), into(role);
                return role;
        }
        catch (soci::postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                throw;
        }
        
}

std::string DAOUsers::getXMLusersAdapters(int userId)
{
        Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"make usersAdapterListXml\n";
        try
        {
                soci::session sql(*_pool);
                std::string xml;
                indicator ind;
                sql << "select xmlagg(xmlelement(name adapter, xmlattributes(adapter_id as id, adapters.name as name, role as role, timezone as utc)))"
                                    "from users join users_adapters on user_id=fk_user_id join adapters on fk_adapter_id = adapter_id where user_id = :id"
                            ,use(userId,"id"), soci::into(xml,ind);
                Logger::getInstance(Logger::DEBUG3)<<"sql done result:"<<std::endl;
                if(ind != i_ok)
                    return "";
                return xml;
        }
        catch (soci::postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                throw;
        }
}
