#include "DAOUsersGateways.h"
#include "DAOGateways.h"

using namespace std;
using namespace soci;
const UserGatewaysColumns DAOUsersGateways::col;
const string DAOUsersGateways::tableUsersGateways = "user_gateway";
DAOUsersGateways::DAOUsersGateways(){
}

DAOUsersGateways::~DAOUsersGateways() {
}


string DAOUsersGateways::getXMLconAccounts(gatewayId_64b gatewayId){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"conAccountList (gate="<<gatewayId<<")\n";
    try{
        soci::session sql(*_pool);
        
        string xml;
        indicator ind;
        sql << "select xmlagg ("
                              "xmlelement(name user, xmlattributes(user_id as id, mail as email, " << DAOUsersGateways::col.permission << " as permission, given_name as name, family_name as surname , gender as gender, picture as imgurl))"
                              ")"
                "from users left join " + tableUsersGateways + " using(" << DAOUsersGateways::col.user_id << ") where " << DAOUsersGateways::col.gateway_id << " = :gate"
                ,use(gatewayId,"gate"), into(xml, ind);
        
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

int DAOUsersGateways::delConAccount(gatewayId_64b gatewayId, string userMail){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"del acc (gate="<<gatewayId<<" acc:"<<userMail<<")\n";
    try{
        soci::session sql(*_pool);
        
        statement st=(sql.prepare << "delete from " + tableUsersGateways + " where " << col.user_id << "=(select user_id from users where mail=:mail) and " << col.gateway_id << "=:gate"
                ,use(gatewayId,"gate"), use(userMail,"mail") );
        st.execute(true);
        return st.get_affected_rows();
        
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        return 0;
    }
}

int DAOUsersGateways::delConAccount(gatewayId_64b gatewayId, int userId){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"del acc (gate="<<gatewayId<<" acc:"<<userId<<")\n";
    try{
        soci::session sql(*_pool);
        
        statement st=(sql.prepare << "delete from " + tableUsersGateways + " where " << col.user_id << "=:uid and " << col.gateway_id << "=:gate"
                ,use(gatewayId,"gate"), use(userId,"uid") );
        st.execute(true);
        return st.get_affected_rows();
        
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        return 0;
    }
}

int DAOUsersGateways::changeConAccount(gatewayId_64b gatewayId, int userId, string newRole){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"change acc (gate="<<gatewayId<<" acc:"<<userId<<" role:"<<newRole<<")\n";
    try{
        soci::session sql(*_pool);
        statement st = (sql.prepare << " update " + tableUsersGateways + " set " << col.permission << "=:role  where " << col.user_id << "=:uid and " << col.gateway_id << "=:gate "
                ,use(newRole, "role"),use(gatewayId,"gate"), use(userId,"uid"));
        st.execute(true);
        return st.get_affected_rows();
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        return 0;
    }
}

int DAOUsersGateways::addConAccount(gatewayId_64b gatewayId, string userMail, string newRole){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"add acc (gate="<<gatewayId<<" acc:"<<userMail<<" role:"<<newRole<<")\n";
    
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

int DAOUsersGateways::addConAccount(gatewayId_64b gatewayId, int userId, string newRole){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"add acc (gate="<<gatewayId<<" acc:"<<userId<<" role:"<<newRole<<")\n";
    
    try{
        soci::session sql(*_pool);
                
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

int DAOUsersGateways::parGateWithUserIfPossible(gatewayId_64b gateId, int userId) {
    
    Logger::db()<< "par user - gate (new user?)" << endl;
    try
    {
        soci::session sql(*_pool);
       
        
        string role = "owner";
        statement st = (sql.prepare <<  "insert into " << tableUsersGateways << "(" + col.gateway_id + ", " + col.user_id + ", " + col.permission + ") select :a_id , :gId, :role where not exists (select 1 from " << tableUsersGateways << " where "+col.gateway_id+"=:a_id)",
                use(gateId, "a_id"), use(role, "role"),  use(userId, "gId"));
        st.execute(true);
        
        int updateCount = st.get_affected_rows();
        if(updateCount == 0)
            return 0;
               
        Logger::db()<< "parred"<< endl;
        return 1;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() <<endl;
        return 0;
    }
}