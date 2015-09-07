/* 
 * File:   DAOUsersAdapters.cpp
 * Author: pavel
 * 
 * Created on 29. březen 2015, 21:51
 */

#include "DAOUsersAdapters.h"
#include "DAOAdapters.h"

using namespace std;
using namespace soci;
const UserGatewaysColumns DAOUsersAdapters::col;
const string DAOUsersAdapters::tableUsersGateways = "user_gateway";
DAOUsersAdapters::DAOUsersAdapters(){
}


DAOUsersAdapters::~DAOUsersAdapters() {
}

DAOUsersAdapters& DAOUsersAdapters::getInstance(){
        static DAOUsersAdapters instance;
        return instance;
}


string DAOUsersAdapters::getXMLconAccounts(long long gatewayId){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"conAccountList (adapter="<<gatewayId<<")\n";
    try{
        soci::session sql(*_pool);
        
        string xml;
        indicator ind;
        sql << "select xmlagg ("
                              "xmlelement(name user, xmlattributes(user_id as id, mail as email, " << DAOUsersAdapters::col.permission << " as role, given_name as name, family_name as surname , gender as gender))"
                              ")"
                "from users left join " + tableUsersGateways + " using(" << DAOUsersAdapters::col.user_id << ") where " << DAOUsersAdapters::col.gateway_id << " = :adapter"
                ,use(gatewayId,"adapter"), into(xml, ind);
        
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

int DAOUsersAdapters::delConAccount(long long gatewayId, string userMail){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"del acc (adapter="<<gatewayId<<" acc:"<<userMail<<")\n";
    try{
        soci::session sql(*_pool);
        
        statement st=(sql.prepare << "delete from " + tableUsersGateways + " where " << col.user_id << "=(select user_id from users where mail=:mail) and " << col.gateway_id << "=:adapter"
                ,use(gatewayId,"adapter"), use(userMail,"mail") );
        st.execute(true);
        return st.get_affected_rows();
        
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        return 0;
    }
}
int DAOUsersAdapters::delConAccount(long long gatewayId, int userId){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"del acc (adapter="<<gatewayId<<" acc:"<<userId<<")\n";
    try{
        soci::session sql(*_pool);
        
        statement st=(sql.prepare << "delete from " + tableUsersGateways + " where " << col.user_id << "=:uid and " << col.gateway_id << "=:adapter"
                ,use(gatewayId,"adapter"), use(userId,"uid") );
        st.execute(true);
        return st.get_affected_rows();
        
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        return 0;
    }
}

int DAOUsersAdapters::changeConAccount(long long gatewayId, int userId, string newRole){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"change acc (adapter="<<gatewayId<<" acc:"<<userId<<" role:"<<newRole<<")\n";
    try{
        soci::session sql(*_pool);
        statement st = (sql.prepare << " update " + tableUsersGateways + " set " << col.permission << "=:role  where " << col.user_id << "=:uid and " << col.gateway_id << "=:adapter "
                ,use(newRole, "role"),use(gatewayId,"adapter"), use(userId,"uid"));
        st.execute(true);
        return st.get_affected_rows();
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        return 0;
    }
}

int DAOUsersAdapters::addConAccount(long long gatewayId, string userMail, string newRole){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"change acc (adapter="<<gatewayId<<" acc:"<<userMail<<" role:"<<newRole<<")\n";
    
    try{
        soci::session sql(*_pool);
        
        sql << "insert into users( mail ) SELECT :mail "
	"WHERE NOT EXISTS ("
		"SELECT * FROM users WHERE mail=:mail "
	")",
                        use(userMail, "mail");   
        
         statement st = (sql.prepare << "insert into " << tableUsersGateways << "(" << col.gateway_id << ", " << col.user_id << ", " << col.permission << ") values(:a_id, (select user_id from users where mail=:mail), :role)",
                use(gatewayId, "a_id"), use(userMail, "mail"), use(newRole, "role") );
         st.execute(true);
         return st.get_affected_rows();
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        return 0;
    }
}

int DAOUsersAdapters::addConAccount(long long gatewayId, int userId, string newRole){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"change acc (adapter="<<gatewayId<<" acc:"<<userId<<" role:"<<newRole<<")\n";
    
    try{
        soci::session sql(*_pool);
        
        sql << "insert into users( user_id ) SELECT :mail "
	"WHERE NOT EXISTS ("
		"SELECT * FROM users WHERE user_id=:id "
	")",
                        use(userId, "id");   
        
         statement st = (sql.prepare << "insert into " << tableUsersGateways << "(" << col.gateway_id << ", " << col.user_id << ", " << col.permission << ") values(:a_id, :id, :role)",
                use(gatewayId, "a_id"), use(userId, "id"), use(newRole, "role") );
         st.execute(true);
         return st.get_affected_rows();
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        return 0;
    }
}


int DAOUsersAdapters::parAdapterWithUserIfPossible(long long int adapterId, std::string adapterName, int userId) {
    
    Logger::db()<< "par user - adapter (new user?)" << endl;
    try
    {
        soci::session sql(*_pool);
       
        
        string role = "superuser";
        statement st = (sql.prepare <<  "insert into " << tableUsersGateways << "(" + col.gateway_id + ", " + col.user_id + ", " + col.permission + ") select :a_id , :gId, :role where not exists (select 1 from " << tableUsersGateways << " where "+col.gateway_id+"=:a_id)",// where fk_adapter_id=:a_id)
                use(adapterId, "a_id"), use(role, "role"),  use(userId, "gId"));
        st.execute(true);
        
        int updateCount = st.get_affected_rows();
        if(updateCount == 0)
            return 0;
       
        sql << "update " + DAOAdapters::tableGateway + " set " << DAOAdapters::col.name << "=:a_name where " << DAOAdapters::col.id << "=:a_id",
                 use(adapterId, "a_id"), use(adapterName, "a_name");
        
        Logger::db()<< "parred"<< endl;
        return 1;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() <<endl;
        return 0;
    }
    
}