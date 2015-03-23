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
    _connectionString = Config::getInstance().getDBConnectionString();
    _poolSize = 2;
    _pool = new connection_pool(_poolSize);
    setConnectionStringAndOpenSessions(_connectionString, _poolSize);
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
                
                sql << "insert into users(mail, phone_locale, verified_email, name, given_name, family_name, link, picture, gender, google_locale, google_id) "
                                                " values(:mail, :phoneLoc, :verMail,:name, "
                                            " :name1, :name2, :link, :pic, :gen, :g_loc, gid)",
                        use (user.mail,"mail"), use(user.phoneLocale, "phoneLoc"), use(user.verifiedMail, "verMail"), use(user.name, "name"),
                        use(user.givenName, "name1"), use(user.familyName, "name2"), use(user.link, "link"),use(user.picture, "pic"), use(user.gender, "gen"), use(user.googleLocale, "g_loc"), use(user.googleId, "gid");   
                return 1;
        }
        catch (soci::postgresql_soci_error& e)
        {
                Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() <<endl;
                return 0;
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


        