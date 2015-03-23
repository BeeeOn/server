/* 
 * File:   DAOAdapters.cpp
 * Author: pavel
 * 
 * Created on 17. březen 2015, 17:15
 */

#include "DAOAdapters.h"

using namespace std;
using namespace soci;

DAOAdapters::DAOAdapters(){
    _connectionString = Config::getInstance().getDBConnectionString();
    _poolSize = 2;
    _pool = new connection_pool(_poolSize);
    setConnectionStringAndOpenSessions(_connectionString, _poolSize);
}


DAOAdapters::~DAOAdapters() {
}

DAOAdapters& DAOAdapters::getInstance(){
        static DAOAdapters instance;
        return instance;
}

int DAOAdapters::parAdapterWithUserIfPossible(long long int adapterId, std::string adapterName, std::string gId) {
    
    Logger::db()<< "par user - adapter (new user?)" << endl;
    try
    {
        soci::session sql(*_pool);
       
        
        string role = "superuser";
         statement st = (sql.prepare <<  "insert into users_adapters(fk_adapter_id, fk_user_id, role) select :a_id , :gId, :role where not exists (select 1 from users_Adapters where fk_adapter_id=:a_id)",// where fk_adapter_id=:a_id)
                use(adapterId, "a_id"), use(role, "role"),  use(gId, "gId"));
        st.execute(true);
        
        int updateCount = st.get_affected_rows();
        if(updateCount == 0)
            return 0;
       
        sql << "update adapters set name=:a_name where adapter_id=:a_id",
                 use(adapterId, "a_id"), use(adapterName, "a_name");
        //TODO if adapter is registrable, 2x sql
        Logger::db()<< "parred"<< endl;
        return 1;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() <<endl;
        return 0;
    }
    
}

int DAOAdapters::isAdapterInDB(long long int adapterId) {
    Logger::db() << "isAdapterInDB" << endl;
    int c;
    try {
        soci::session sql(*_pool);
        sql << "select count(*) from adapters where adapter_id=:a_id",
                use(adapterId, "a_id"), into(c);

    }catch (soci::postgresql_soci_error& e) {
        Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() << endl;
        return 0;
    }

    return c;
}

       
