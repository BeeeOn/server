/* 
 * File:   DAOAdapters.cpp
 * Author: pavel
 * 
 * Created on 17. březen 2015, 17:15
 */

#include "DAOAdapters.h"
#include "ServerException.h"

using namespace std;
using namespace soci;

DAOAdapters::DAOAdapters(){
}


DAOAdapters::~DAOAdapters() {
}

DAOAdapters& DAOAdapters::getInstance(){
        static DAOAdapters instance;
        return instance;
}

int DAOAdapters::parAdapterWithUserIfPossible(long long int adapterId, std::string adapterName, int userId) {
    
    Logger::db()<< "par user - adapter (new user?)" << endl;
    try
    {
        soci::session sql(*_pool);
       
        
        string role = "superuser";
         statement st = (sql.prepare <<  "insert into users_adapters(fk_adapter_id, fk_user_id, role) select :a_id , :gId, :role where not exists (select 1 from users_Adapters where fk_adapter_id=:a_id)",// where fk_adapter_id=:a_id)
                use(adapterId, "a_id"), use(role, "role"),  use(userId, "gId"));
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

       
string DAOAdapters::getTimeZone(string adapterId){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"get time zone (adapter="<<adapterId<<")\n";
    try{
        soci::session sql(*_pool);
        
        string timezone;
        indicator ind;
        sql << "select timezone from adapters where adapter_id=:adapter"
                ,use(adapterId,"adapter"), into(timezone, ind);
        
            if(ind != i_ok)
                throw ServerException(ServerException::ADAPTER_ID);
        return timezone;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

void DAOAdapters::updateAdaptersTimezone(string adapterId,  string newTimeZone){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"update time zone (adapter="<<adapterId<<")\n";
    try{
        soci::session sql(*_pool);
        
        string xml;
        sql <<"update adapters set timezone = :timezone where adapter_id = :adapter"
                ,use(newTimeZone, "timezone"),use(adapterId,"adapter");             
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}