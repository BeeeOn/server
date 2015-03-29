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
                user.verifiedMail = 1;
                sql << "insert into users(mail, password, phone_locale, verified_email, name, given_name, family_name, link, picture, gender, google_locale, google_id) "
                                                " values(:mail, :password, :phone_locale, :verified_email,:name, "
                                            " :given_name, :family_name, :link, :picture, :gender, :google_locale, :google_id)", use(user);
                return 1;
        }
        catch (soci::postgresql_soci_error& e)
        {
                Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() <<endl;
                return 0;
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

int DAOUsers::upsertUserWithMobileDevice(User user, MobileDevice mobile){
    session sql(*_pool);
    Logger::db()<<"DB:"<<"upsertUserWithMobileDevice"<<endl;
    try
    {
            soci::session sql(*_pool);

            int newUserID;
            //d_mail TEXT, d_locale TEXT, d_ver BOOLEAN, d_name TEXT, d_g_name TEXT, d_f_name TEXT, d_link TEXT, d_picture TEXT, d_gender TEXT, d_g_loc TEXT, d_g_id TEXT
            
            user.verifiedMail = 1;
            sql.begin();
            sql << "select upsert_user_returning_uid(:mail, :pLoc, :verMail, :name, :gName, :fName, :link, :picture, :gender, :googleLocale, :googleID)",
                    use(user.mail), use(user.phoneLocale), use(user.verifiedMail), use(user.name), use(user.givenName), use(user.familyName), use(user.link), use(user.picture), use(user.gender), 
                    use(user.googleLocale), use(user.googleId), into(newUserID);
            
            //bon_token TEXT, d_mobile_id TEXT,d_type TEXT, d_locale TEXT, d_push_n TEXT, d_uid integer
            sql << "select upsert_mobile_device(:bt, :mobileID, :type, :loc, :pushN, :userID)",
                    use(mobile.token), use(mobile.mobile_id), use(mobile.type), use(mobile.locale),use(mobile.push_notification),use(newUserID);
            sql.commit();
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

std::string DAOUsers::getUserID(std::string Btoken) {
    Logger::db()<<"DB: getUID by token:"<<Btoken << endl;
    
    try
    {
            session sql(*_pool);
            string UID;
            sql << "select user_id from users join mobile_devices on user_id = fk_user_id where token = :btoken", use(Btoken, "btoken"),into(UID);
            Logger::getInstance(Logger::DEBUG3)<<"uid is: "<<UID<< endl;
            return UID; 
    }
    catch (soci::postgresql_soci_error& e)
    {
            Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() <<endl;
            throw;
    }   
}

User DAOUsers::getUserAssociatedWithToken(std::string token) {
    Logger::db()<<"DB: get user by token:"<<token << endl;
    
    try
    {
            session sql(*_pool);
            User user;

            sql << "select user_id, mail, phone_locale,verified_email,name,given_name,family_name,link,picture,gender,google_locale,google_id from users join mobile_devices on user_id = fk_user_id where token = :btoken", use(token, "btoken"),into(user);
            return user; 
    }
    catch (soci::postgresql_soci_error& e)
    {
            Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() <<endl;
            throw;
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
                                    "from users join users_adapters on user_id=fk_user_id join adapters on fk_adapter_id = adapter_id where user_id = :gid"
                            ,use(userId,"gid"), soci::into(xml,ind);
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