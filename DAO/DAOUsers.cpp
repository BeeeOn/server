#include "DAOUsers.h"
#include "../ui_logger/Logger.h"
#include "Config.h"
#include "DAOUsersGateways.h"
#include "DAOGateways.h"

using namespace std;
using namespace soci;

const std::string DAOUsers::tableUsers = "users";
const UsersColumns DAOUsers::col;

namespace soci
{
    template<>
    struct type_conversion<User>
    {
        typedef values base_type;

        static void from_base(values const & v, indicator /* ind */, User & user)
        {
            user.user_id = v.get<int>(DAOUsers::col.id);
            user.mail = v.get<std::string>(DAOUsers::col.mail,"");
            user.password = v.get<std::string>(DAOUsers::col.password,"");
            user.givenName = v.get<std::string>(DAOUsers::col.first_name,"");
            user.familyName = v.get<std::string>(DAOUsers::col.surname,"");
            user.picture = v.get<std::string>(DAOUsers::col.picture,"");
            user.gender = v.get<std::string>(DAOUsers::col.gender,"");
            user.googleId = v.get<std::string>(DAOUsers::col.google_id,"");
            user.facebookId = v.get<std::string>(DAOUsers::col.facebook_id,"");
        }
    
        static void to_base(const User & user, values & v, indicator & ind)
        {           
            v.set(DAOUsers::col.id, user.user_id);
            v.set(DAOUsers::col.mail, user.mail, user.mail.empty() ? i_null : i_ok);
            v.set(DAOUsers::col.password, user.password, user.password.empty() ? i_null : i_ok);
            v.set(DAOUsers::col.first_name, user.givenName, user.givenName.empty() ? i_null : i_ok);
            v.set(DAOUsers::col.surname, user.familyName, user.familyName.empty() ? i_null : i_ok);
            v.set(DAOUsers::col.picture, user.picture, user.picture.empty() ? i_null : i_ok);
            v.set(DAOUsers::col.gender, user.gender, user.gender.empty() ? i_null : i_ok);
            v.set(DAOUsers::col.google_id, user.googleId, user.googleId.empty() ? i_null : i_ok);
            v.set(DAOUsers::col.facebook_id, user.facebookId, user.facebookId.empty() ? i_null : i_ok);
            ind = i_ok;
        }
    };
}

DAOUsers::DAOUsers()
{
}

DAOUsers::~DAOUsers() 
{
}

std::string DAOUsers::getDAOname() 
{
    return "USER";
}

int DAOUsers::add(User user) 
{
        Logger::db() << "DB:" << "insertNewUser" << endl;
        try
        {
                soci::session sql(*_pool);
                
                sql << "insert into " << tableUsers << "(" << col.mail << ", " << col.password << ", " << col.first_name << ", " << col.surname<< ", " << col.picture << ", " << col.gender << ", " << col.google_id << ", " << col.facebook_id << ") "
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

User DAOUsers::getUserAssociatedWithToken(std::string token) 
{
    Logger::db()<<"DB: get user by token:"<<token << endl;
    
    try
    {
            session sql(*_pool);
            User user;

            sql << "select * from " << tableUsers << " join mobile_devices on " << col.id << " = fk_user_id where token = :btoken", use(token, "btoken"),into(user);
            return user; 
    }
    catch (soci::postgresql_soci_error& e)
    {
            Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() <<endl;
            throw;
    }   
}

User DAOUsers::getUserByID(int userId) 
{
    Logger::db()<<"DB: get user by id:"<<userId << endl;
    
    try
    {
            session sql(*_pool);
            User user;

            sql << "select * from " << tableUsers << " where " << col.id << " = :id", use(userId, "id"),into(user);
            return user; 
    }
    catch (soci::postgresql_soci_error& e)
    {
            Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() <<endl;
            throw;
    }  
}

bool DAOUsers::isMailRegistred(std::string mail) 
{
    Logger::db()<<"DB: is "<<mail<<" registred?" << endl;
    int c;
    try
    {
            session sql(*_pool);
            sql << "select count(*) from " << tableUsers << " where " << col.mail << "=:mail;", use(mail, "mail"),into(c);
            Logger::getInstance(Logger::DEBUG3)<<mail<< c << endl;
            return c==1; 
    }
    catch (soci::postgresql_soci_error& e)
    {
            Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() <<endl;
            throw;
    }
}

bool DAOUsers::isNameRegistred(std::string name) 
{
    Logger::db()<<"DB: is "<<name<<" registred?" << endl;
    int c;
    try
    {
            session sql(*_pool);
            sql << "select count(*) from " << tableUsers << " where name=:name;", use(name, "name"),into(c);
            Logger::getInstance(Logger::DEBUG3)<<name<< c << endl;
            return c==1; 
    }
    catch (soci::postgresql_soci_error& e)
    {
            Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() <<endl;
            throw;
    }
}

bool DAOUsers::isGoogleIdRegistred(std::string g_id) 
{
    Logger::db()<<"DB: is "<<g_id<<" registred?" << endl;
    int c;
    try
    {
            session sql(*_pool);
            sql << "select count(*) from " << tableUsers << " where " << col.google_id << "=:google_id;", use(g_id, "google_id"),into(c);
            Logger::getInstance(Logger::DEBUG3)<<g_id<< c << endl;
            return c==1; 
    }
    catch (soci::postgresql_soci_error& e)
    {
            Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() <<endl;
            throw;
    }
}

int DAOUsers::getUserIDbyAlternativeKeys(std::string mail, std::string password, std::string google_id, std::string facebook_id)
{
    Logger::db()<<"DB: getUID by alternative keys:("<<mail << google_id << facebook_id <<")"<< endl;
    
    try
    {
            session sql(*_pool);
            int UID=-1;
            
            indicator i1 = mail.empty()?i_null:i_ok;
            indicator i2 = google_id.empty()?i_null:i_ok;
            indicator i3 = facebook_id.empty()?i_null:i_ok;
            sql << "select user_id from " << tableUsers << " where "
                                "CASE "
                                        "WHEN :google_id::text is not NULL then google_id=:google_id "
                                        "WHEN :fb_id::text is not NULL then facebook_id=:fb_id "
                                        "WHEN :mail::text is not NULL THEN mail=:mail and password=:password "
                                        
                                "END",
                    use(mail,i1,"mail"), use(password,"password"), use(google_id,i2,"google_id"),use(facebook_id,i3,"fb_id"),
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

int DAOUsers::getUserIdbyIhaToken(string token) 
{
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
        
string DAOUsers::getUserRoleM(int userId, gatewayId_64b gateId)
{
        Logger::getInstance(Logger::DEBUG3)<<"DB: get role "<<userId<<" on "<<gateId<<endl;
        string role;
        try
        {
                session sql(*_pool);
                sql << "select " << DAOUsersGateways::col.permission << " from " << DAOUsersGateways::tableUsersGateways << 
                        " where " << DAOUsersGateways::col.user_id << "= :guserid and " << DAOUsersGateways::col.gateway_id << " = :gate " <<
                        "limit 1",
                    use(userId, "guserid"), use(gateId, "gate"), into(role);
                return role;
        }
        catch (soci::postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                throw;
        }
        
}

bool DAOUsers::isGateParred(int userId, gatewayId_64b gateId) 
{
        Logger::getInstance(Logger::DEBUG3)<<"DB: get role "<<userId<<" on "<<gateId<<endl;
        int result = false;
        try
        {
                session sql(*_pool);
                sql << "select exists (" <<
                            "select 1 from " << DAOUsersGateways::tableUsersGateways << 
                            " where " << DAOUsersGateways::col.user_id << "= :guserid and " << DAOUsersGateways::col.gateway_id << " = :gate " <<
                            "limit 1"
                        ")",
                        use(userId, "guserid"), use(gateId, "gate"), into(result);
                return result != 0;
        }
        catch (soci::postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                throw;
        }
}

std::string DAOUsers::getXMLusersGates(int userId)
{
        Logger::getInstance(Logger::DEBUG3)<<"DB:"<<getDAOname()<<"make usersGateListXml\n";
        try
        {
                soci::session sql(*_pool);
                std::string xml;
                indicator ind;
                sql << "select xmlagg(xmlelement(name gate, xmlattributes(" << DAOGateways::col.id << " as id, " << DAOGateways::col.name << " as name, " 
                        << DAOUsersGateways::col.permission << " as permission, "<< DAOGateways::col.status << " as status, " << DAOGateways::col.timezone << " as timezone)))"
                                    "from " << tableUsers << " join " << DAOUsersGateways::tableUsersGateways << " using(" << col.id << ") " << 
                                " join " << DAOGateways::tableGateway << " using(" << DAOGateways::col.id << ") where " << col.id << " = :id"
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
