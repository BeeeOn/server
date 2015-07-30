/* 
 * File:   DAOAdapters.cpp
 * Author: pavel
 * 
 * Created on 17. březen 2015, 17:15
 */

#include "DAOAdapters.h"
#include "ServerException.h"
#include "DAODevices.h"
#include "DAOUsersAdapters.h"

using namespace std;
using namespace soci;

const GatewayColumns DAOAdapters::col;

namespace soci
{
    template<>
    struct type_conversion<Gate>
    {
        typedef values base_type;

        static void from_base(values const & v, indicator /* ind */, Gate & gate)
        {
//            string gateId = v.get<std::string>("adapter_id","-1");
//            
//            try
//            {
//                gate.id = stoll(gateId); 
//            }
//            catch(...)
//            {
//                gate.id = -1;
//            }
            gate.id = (long long int) (v.get<double>(DAOAdapters::col.id) + 0.5);
            
            gate.name = v.get<std::string>(DAOAdapters::col.name,"");
            gate.timezone = v.get<int>(DAOAdapters::col.timezone);
        }
    
        static void to_base(const Gate & gate, values & v, indicator & ind)
        {           
            v.set(DAOAdapters::col.id, gate.id);
            v.set(DAOAdapters::col.name, gate.name);
            v.set(DAOAdapters::col.timezone, gate.timezone);
            ind = i_ok; 
        }
    };
}

const std::string DAOAdapters::tableGateway = "gateway";
const std::string DAOAdapters::tableUsersGateway = "user_gateway";


DAOAdapters::DAOAdapters(){
}

DAOAdapters::~DAOAdapters() {
}

DAOAdapters& DAOAdapters::getInstance(){
        static DAOAdapters instance;
        return instance;
}


Gate DAOAdapters::getAdapter(long long adapterId) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"get adapter : "<<adapterId<<"\n";
    try{
        soci::session sql(*_pool);
        
        Gate gate;
        sql <<"select " << col.id << "," << col.name << "," << col.timezone << " from " + tableGateway + " where " +col.id+ " = :gw"
                , use(adapterId,"gw"),into(gate); 
        return gate;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

int DAOAdapters::updateAdapter(long long int adapterId, std::string newName, std::string newTimeZone) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"update adapter : "<<adapterId<<"\n";
    try{
        soci::session sql(*_pool);
        
        string partialUpdateString;
        if(newName != "")
            partialUpdateString += col.name +" = :aname";
        if(newName != "" && newTimeZone != "")
            partialUpdateString += ", ";
        if(newTimeZone != "")
            partialUpdateString += col.timezone +" = :timezone";
        
        
        //Logger::debug3()<<"DB:"<<"update with: "<<partialUpdateString<<"\n";
       // Logger::debug3()<<"DB:"<<"update with: "<<newName << " " <<newTimeZone<<"\n";
        
        string xml;
        sql <<"update " + tableGateway + " set " << partialUpdateString << " where "+col.id+" = :adapter"
                ,use(newName, "aname"), use(newTimeZone, "timezone"), use(adapterId,"adapter"); 
        
        return 1;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}


int DAOAdapters::deleteAdapter(long long adapterId){
    Logger::db()<< "delete adapter " << adapterId << endl;
    try
    {
        soci::session sql(*_pool);
       
        statement st = (sql.prepare <<  
                "delete from " + tableGateway + " where "+col.id+" = :a_id",
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



GateInfo DAOAdapters::getGateInfo(long long int gateId) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"get gate info : "<<gateId<<"\n";
        
    try{
        soci::session sql(*_pool);
        
        GateInfo gateInfo;
        sql <<"select " << col.id << "," << "count(distinct " << DAODevices::col.mac << "),count(distinct " << DAOUsersAdapters::col.user_id << ")" << "," << col.name << "," << col.socket << "," << col.version << "," << col.timezone <<
                " from " + tableGateway + " left join " + DAODevices::tableDevices + " using("+col.id+") left join " + tableUsersGateway+ " using("+col.id+") "
                "where "+col.id+" = :adapter group by "+col.id
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



int DAOAdapters::isAdapterInDB(long long int adapterId) {
    Logger::db() << "isAdapterInDB" << endl;
    int c;
    try {
        soci::session sql(*_pool);
        sql << "select count(*) from " + tableGateway + " where " << col.id << "=:a_id",
                use(adapterId, "a_id"), into(c);

    }catch (soci::postgresql_soci_error& e) {
        Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() << endl;
        return 0;
    }

    return c;
}

       
//string DAOAdapters::getTimeZone(string adapterId){
//    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"get time zone (adapter="<<adapterId<<")\n";
//    try{
//        soci::session sql(*_pool);
//        
//        string timezone;
//        indicator ind;
//        sql << "select timezone from " + tableGateway + " where adapter_id=:adapter"
//                ,use(adapterId,"adapter"), into(timezone, ind);
//        
//            if(ind != i_ok)
//                throw ServerException(ServerException::ADAPTER_ID);
//        return timezone;
//    }
//    catch (soci::postgresql_soci_error& e)
//    {
//        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
//        throw;
//    }
//}

void DAOAdapters::updateAdaptersTimezone(string adapterId,  string newTimeZone){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"update time zone (adapter="<<adapterId<<")\n";
    try{
        soci::session sql(*_pool);
        
        string xml;
        sql <<"update " + tableGateway + " set " << col.timezone << " = :timezone where " << col.id << " = :adapter"
                ,use(newTimeZone, "timezone"),use(adapterId,"adapter");             
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

int DAOAdapters::delUsersAdapter(long long adapterId, int userId) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"delUsersAdapter" << endl;
    try{
        soci::session sql(*_pool);
        
        string xml;
        statement st = (sql.prepare <<  
        "delete from " + tableUsersGateway + " where " << DAOUsersAdapters::col.gateway_id << " = :adapter and " << DAOUsersAdapters::col.user_id << " = :user_id and " << DAOUsersAdapters::col.permission << " != 'superuser' "
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
