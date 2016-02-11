#include "DAODevicesParameters.h"
#include "DAO.h"

using namespace std;
using namespace soci;

const string DAODevicesParameters::tableDevicesParameter = "device_parameter";
const DeviceParameterColumns DAODevicesParameters::col;

namespace soci
{
    template<>
    struct type_conversion<deviceParameter>
    {
        typedef values base_type;

        static void from_base(values const & v, indicator /* ind */, deviceParameter & deviceParam)
        {  
//            string devId = (v.get<std::string>(DAODevicesParameters::col.euid, ""));
//            deviceParam.euid = stoull (devId); 
//            string gwId = (v.get<std::string>(DAODevicesParameters::col.gateway_id, ""));
//            deviceParam.gatewayId = stoull (gwId); 
            deviceParam.euid = (deviceId_64b) (v.get<double>(DAODevicesParameters::col.euid));
            deviceParam.gatewayId = (gatewayId_64b) (v.get<double>(DAODevicesParameters::col.gateway_id));
            deviceParam.key = (v.get<std::string>(DAODevicesParameters::col.key, ""));
            deviceParam.value = (v.get<std::string>(DAODevicesParameters::col.value, ""));
        }
    
        static void to_base(const deviceParameter & deviceParam, values & v, indicator & ind)
        {           
            v.set(DAODevicesParameters::col.euid, (double)deviceParam.euid);
            v.set(DAODevicesParameters::col.gateway_id, (double)deviceParam.gatewayId);
            v.set(DAODevicesParameters::col.key, deviceParam.key);
            v.set(DAODevicesParameters::col.value, deviceParam.value);
            ind = i_ok; 
        }
    };
}

DAODevicesParameters::DAODevicesParameters() {
}

DAODevicesParameters::~DAODevicesParameters() {
}

bool DAODevicesParameters::create(deviceParameter deviceParam) {
    Logger::db() << "create device parameter" << endl;
    
    try
    {
        soci::session sql(*_pool);
        
        sql << "INSERT INTO " << tableDevicesParameter << " (" << col.gateway_id << ", " << col.euid << ", " << col.key << ", " << col.value << ") " <<
                "VALUES( :" << col.gateway_id << ", :" << col.euid << ", :" << col.key << ", :" << col.value << ") ",
                use(deviceParam);
    
        Logger::db() << "create device parameter : done" << endl;
        
        return true;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::error() << "Error: " << e.what() << endl;
        return false;
    } 
}

deviceParameter DAODevicesParameters::get(deviceParameter deviceParam) {
    Logger::db() << "get device parameter" << endl;
    
    try
    {
        soci::session sql(*_pool);
     
        deviceParameter resultDeviceParam;
        sql << "SELECT " << col.gateway_id << ", " << col.euid << ", " << col.key << ", " << col.value << " FROM " << tableDevicesParameter << " " << 
                "WHERE " << col.gateway_id << "= :" << col.gateway_id << " AND " << col.euid << "= :" << col.euid << " AND " << col.key << "= :" << col.key,
                use(deviceParam), into(resultDeviceParam);
    
        Logger::db() << "get device parameter : done" << endl;
        return resultDeviceParam;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::error() << "Error: " << e.what() << endl;
        throw;
    } 
}


int DAODevicesParameters::remove(deviceParameter deviceParam) {
    Logger::db() << "delete device parameter" << endl;
    
    try
    {
        soci::session sql(*_pool);
     
        statement st = (sql.prepare << 
                "DELETE FROM " << tableDevicesParameter << " " <<
                "WHERE " << col.gateway_id << "= :" << col.gateway_id << " AND " << col.euid << "= :" << col.euid << " AND " << col.key << "= :" << col.key,
                use(deviceParam));
        
        st.execute(true);
        Logger::db() << "delete device parameter : done" << endl;
        return st.get_affected_rows();
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::error() << "Error: " << e.what() << endl;
        return 0;
    } 
}

int DAODevicesParameters::update(deviceParameter deviceParam) {
    Logger::db() << "update device parameter" << endl;
    
    try
    {
        soci::session sql(*_pool);
     
        statement st = (sql.prepare << 
                "UPDATE " << tableDevicesParameter << " " <<
                "SET " << col.value << "= :" << col.value << " " <<
                "WHERE " << col.gateway_id << "= :" << col.gateway_id << " AND " << col.euid << "= :" << col.euid << " AND " << col.key << "= :" << col.key,
                use(deviceParam));
        
        st.execute(true);
        Logger::db() << "update device parameter : done" << endl;
        return st.get_affected_rows();
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::error() << "Error: " << e.what() << endl;
        return 0;
    } 
}