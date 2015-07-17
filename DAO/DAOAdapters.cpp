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

namespace soci
{
    template<>
    struct type_conversion<Gate>
    {
        typedef values base_type;

        static void from_base(values const & v, indicator /* ind */, Gate & gate)
        {
            gate.id = v.get<string>("adapter_id");
            gate.name = v.get<std::string>("name","");
            gate.timezone = v.get<int>("timezone");
        }
    
        static void to_base(const Gate & gate, values & v, indicator & ind)
        {           
            v.set("adapter_id", gate.id);
            v.set("name", gate.name);
            v.set("timezone", gate.timezone);
            ind = i_ok;
        }
    };
}

DAOAdapters::DAOAdapters(){
}

DAOAdapters::~DAOAdapters() {
}

DAOAdapters& DAOAdapters::getInstance(){
        static DAOAdapters instance;
        return instance;
}

Gate DAOAdapters::getAdapter(std::string adapterId) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"get adapter : "<<adapterId<<"\n";
    try{
        soci::session sql(*_pool);
        
        Gate gate;
        sql <<"select adapter_id::text, name, timezone from adapters where adapter_id = :adapter"
                , use(adapterId,"adapter"), into(gate); 
        return gate;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}


int DAOAdapters::updateAdapter(std::string adapterId, std::string newName, std::string newTimeZone) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"update adapter : "<<adapterId<<"\n";
    try{
        soci::session sql(*_pool);
        
        string partialUpdateString;
        if(newName != "")
            partialUpdateString += "name = :aname";
        if(newName != "" && newTimeZone != "")
            partialUpdateString += ", ";
        if(newTimeZone != "")
            partialUpdateString += "timezone = :timezone";
        
        
        //Logger::debug3()<<"DB:"<<"update with: "<<partialUpdateString<<"\n";
       // Logger::debug3()<<"DB:"<<"update with: "<<newName << " " <<newTimeZone<<"\n";
        
        string xml;
        sql <<"update adapters set " << partialUpdateString << " where adapter_id = :adapter"
                ,use(newName, "aname"), use(newTimeZone, "timezone"), use(adapterId,"adapter"); 
        
        return 1;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}


int DAOAdapters::deleteAdapter(string adapterId){
    Logger::db()<< "delete adapter " << adapterId << endl;
    try
    {
        soci::session sql(*_pool);
       
        statement st = (sql.prepare <<  
                "delete from adapters where adapter_id = :a_id",
                use(adapterId, "a_id"));
        st.execute(true);
        
        int updateCount = st.get_affected_rows();
        if(updateCount == 0)
            return 0;
        return 1;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() <<endl;
        return 0;
    }
}

GateInfo DAOAdapters::getGateInfo(std::string gateId) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"get gate info : "<<gateId<<"\n";
    try{
        soci::session sql(*_pool);
        
        GateInfo gateInfo;
        sql <<"select adapter_id::text, count(distinct mac), count(distinct fk_user_id), name, socket, version, timezone "
                "from adapters left join facilities as f on f.fk_adapter_id = adapter_id left join users_adapters as ua on ua.fk_adapter_id = adapter_id "
                "where adapter_id = :adapter group by adapter_id"
                , use(gateId,"adapter"), into(gateInfo.id), into(gateInfo.nFacilities), into(gateInfo.nUsers), into(gateInfo.name)
                , into(gateInfo.ip), into(gateInfo.version), into(gateInfo.timezone); 
        return gateInfo;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
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

int DAOAdapters::delUsersAdapter(std::string adapterId, int userId) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"delUsersAdapter" << endl;
    try{
        soci::session sql(*_pool);
        
        string xml;
        statement st = (sql.prepare <<  
        "delete from users_adapters where fk_adapter_id = :adapter and fk_user_id = :user_id and role != 'superuser' "
                ,use(userId, "user_id"),use(adapterId,"adapter"));         
        st.execute(true);
        
        int deleteCount = st.get_affected_rows();
        return deleteCount;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}
