/* 
 * File:   DAOUsersAdapters.cpp
 * Author: pavel
 * 
 * Created on 29. březen 2015, 21:51
 */

#include "DAOUsersAdapters.h"

using namespace std;
using namespace soci;

DAOUsersAdapters::DAOUsersAdapters(){
}


DAOUsersAdapters::~DAOUsersAdapters() {
}

DAOUsersAdapters& DAOUsersAdapters::getInstance(){
        static DAOUsersAdapters instance;
        return instance;
}


string DAOUsersAdapters::getXMLconAccounts(string adapterId){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"conAccountList (adapter="<<adapterId<<")\n";
    try{
        soci::session sql(*_pool);
        
        string xml;
        indicator ind;
        sql << "select xmlagg ("
                                                "xmlelement(name user, xmlattributes(mail as email, role as role, given_name as name, family_name as surname , gender as gender))"
                                                ")"
                "from users left join users_adapters on user_id = fk_user_id where fk_adapter_id = :adapter"
                ,use(adapterId,"adapter"), into(xml, ind);
        
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

int DAOUsersAdapters::delConAccount(string adapterId, string userMail){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"del acc (adapter="<<adapterId<<" acc:"<<userMail<<")\n";
    try{
        soci::session sql(*_pool);
        
        statement st=(sql.prepare << "delete from users_adapters where fk_user_id=(select user_id from users where mail=:mail) and fk_adapter_id=:adapter"
                ,use(adapterId,"adapter"), use(userMail,"mail") );
        st.execute(true);
        return st.get_affected_rows();
        
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        return 0;
    }
}

int DAOUsersAdapters::changeConAccount(string adapterId, string userMail, string newRole){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"change acc (adapter="<<adapterId<<" acc:"<<userMail<<" role:"<<newRole<<")\n";
    try{
        soci::session sql(*_pool);
        statement st = (sql.prepare << " update users_adapters set role=:role  where fk_user_id=(select user_id from users where mail=:mail) and fk_adapter_id=:adapter "
                //"and check_downgrade_last_superuser(:adapter, (select user_id from users where mail=:mail))"
                ,use(newRole, "role"),use(adapterId,"adapter"), use(userMail,"mail"));
        st.execute(true);
        return st.get_affected_rows();
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        return 0;
    }
}
int DAOUsersAdapters::addConAccount(string adapterId, string userMail, string newRole){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"change acc (adapter="<<adapterId<<" acc:"<<userMail<<" role:"<<newRole<<")\n";
    
    try{
        soci::session sql(*_pool);
        
        //sql << "LOCK TABLE users IN SHARE ROW EXCLUSIVE MODE";
        
        sql << "insert into users( mail ) SELECT :mail "
	"WHERE NOT EXISTS ("
		"SELECT * FROM users WHERE mail=:mail "
	")",
                        use(userMail, "mail");   
        
         statement st = (sql.prepare << "insert into users_adapters(fk_adapter_id, fk_user_id, role) values(:a_id, (select user_id from users where mail=:mail), :role)",
                use(adapterId, "a_id"), use(userMail, "mail"), use(newRole, "role") );
         st.execute(true);
         return st.get_affected_rows();
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        return 0;
    }
}