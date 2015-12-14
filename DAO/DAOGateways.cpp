#include "DAOGateways.h"
#include "ServerException.h"
#include "DAODevices.h"
#include "DAOUsersGateways.h"
#include <math.h>
#include <cmath>
using namespace std;
using namespace soci;

const GatewayColumns DAOGateways::col;
namespace soci
{
    template<>
    struct type_conversion<Gate>
    {
        typedef values base_type;

        static void from_base(values const & v, indicator /* ind */, Gate & gate)
        {  
            gate.id = (gatewayId_64b) (v.get<gatewayId_64b>(DAOGateways::col.id));
            gate.name = v.get<std::string>(DAOGateways::col.name, "");
            gate.timezone = v.get<double>(DAOGateways::col.timezone, NAN);
            gate.altitude = v.get<double>(DAOGateways::col.altitude, NAN);
            gate.longitude = v.get<double>(DAOGateways::col.longitude, NAN);
            gate.latitude = v.get<double>(DAOGateways::col.latitude, NAN);
        }
    
        static void to_base(const Gate & gate, values & v, indicator & ind)
        {           
            v.set(DAOGateways::col.id, gate.id);
            v.set(DAOGateways::col.name, gate.name);
            v.set(DAOGateways::col.timezone, gate.timezone);
            v.set(DAOGateways::col.altitude, gate.altitude);
            v.set(DAOGateways::col.longitude, gate.longitude);
            v.set(DAOGateways::col.latitude, gate.latitude);
            ind = i_ok; 
        }
    };
}

const std::string DAOGateways::tableGateway = "gateway";
const std::string DAOGateways::tableUsersGateway = "user_gateway";


DAOGateways::DAOGateways()
{
}

DAOGateways::~DAOGateways() 
{
}

Gate DAOGateways::get(gatewayId_64b gateId) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"get gate : "<<gateId<<"\n";
    try{
        soci::session sql(*_pool);
        
        Gate gate;
        sql <<"select " << col.id << "," << col.name << "," << col.timezone << 
                " from " << tableGateway << 
                " where " << col.id << " = :gw"
                , use(gateId,"gw"),into(gate); 
        return gate;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

int DAOGateways::update(Gate gate) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"update gate : "<<gate.id<<"\n";
    try{
        soci::session sql(*_pool);
        
        string columnsToSet;
        
        cout <<gate.altitude << endl;

        
        if(gate.name != "")columnsToSet.append(" "+col.name+"=:home_name, ");
        if(!std::isnan(gate.timezone))columnsToSet.append(" "+col.timezone+"=:home_timezone, ");
        if(!std::isnan(gate.altitude))columnsToSet.append(" "+col.altitude+"=:altitude, ");
        if(!std::isnan(gate.latitude))columnsToSet.append(" "+col.latitude+"=:latitude, ");
        if(!std::isnan(gate.longitude))columnsToSet.append(" "+col.longitude+"=:longitude, ");
        columnsToSet = columnsToSet.substr(0,columnsToSet.size()-2);//erase last ', '
        
        
        string xml;
        sql <<"update " << tableGateway << 
                " set " << columnsToSet << 
                " where " << col.id << " = :gateway_id"
                , use(gate); 
        
        return 1;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}


int DAOGateways::remove(gatewayId_64b gateId){
    Logger::db()<< "delete gate " << gateId << endl;
    try
    {
        soci::session sql(*_pool);
       
        statement st = (sql.prepare <<  
                "delete from " + tableGateway + 
                    " where "+col.id+" = :a_id",
                use(gateId, "a_id"));
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

GateInfo DAOGateways::getGateInfo(gatewayId_64b gateId) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"get gate info : "<<gateId<<"\n";
        
        soci::session sql(*_pool);
    try{
        indicator ind;
        GateInfo gateInfo;
        gateInfo.nFacilities = 0;
        sql <<"select " << col.id << "," << "count(distinct " << DAODevices::col.euid << "),count(distinct " << DAOUsersGateways::col.user_id << ")" << "," 
                << col.name << "," << col.socket << "," << col.version << "," << col.timezone << "," << col.altitude << "," << tableGateway << "." <<col.status << "," 
                << " coalesce((select concat(given_name, ' ', family_name) from user_gateway left join users using(user_id) where gateway_id=:gate and permission='owner' limit 1),'')" 
                " from " << tableGateway << 
                    " left join " << DAODevices::tableDevices << " using("+col.id+") " << 
                    "left join " << tableUsersGateway << " using(" << col.id << ") "
                "where " << col.id << " = :gate group by " << col.id
                , use(gateId,"gate"), into(gateInfo.id), into(gateInfo.nFacilities, ind), into(gateInfo.nUsers), into(gateInfo.name)
                , into(gateInfo.ip), into(gateInfo.version), into(gateInfo.timezone), into(gateInfo.altitude),into(gateInfo.status),into(gateInfo.owner); 
        
        
        return gateInfo;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

int DAOGateways::exists(gatewayId_64b gateId) {
    Logger::db() << "isGateInDB" << endl;
    int c;
    try {
        soci::session sql(*_pool);
        sql << "select count(*) from " << tableGateway << 
                " where " << col.id << "=:a_id",
                use(gateId, "a_id"), into(c);

    }catch (soci::postgresql_soci_error& e) {
        Logger::db() << "Error: " << e.what() << " sqlState: " << e.sqlstate() << endl;
        return 0;
    }

    return c;
}

