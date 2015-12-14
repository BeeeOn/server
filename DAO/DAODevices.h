#ifndef DAODEVICES_H
#define	DAODEVICES_H

#include "DAO.h"
#include "DAOGateways.h"
#include "../ui_logger/Logger.h"

struct device {
    deviceId_64b id;
    int type;
};

struct DeviceColumns{
    std::string euid;
    std::string id;
    std::string name;
    std::string refresh;
    std::string init;
    std::string involved;
    std::string measured_at;
    std::string location_id;
    std::string status;
    std::string gateway_id;
    
    DeviceColumns():
            euid("device_euid"), 
            id("device_type"), 
            name("device_name"),
            refresh("refresh"),
            init("init"),
            involved("involved"),
            measured_at("measured_at"),
            location_id("location_id"),
            status("status"),
            gateway_id("gateway_id")
            { }
};

struct ModuleColumns{
    std::string euid;
    std::string id;
    std::string value;
    std::string gateway_id;
    
    ModuleColumns():
            euid("device_euid"), 
            id("module_id"), 
            value("measured_value"),
            gateway_id("gateway_id")
            { }
};

class DAODevices 
    :   public DAO 
{
    public:
        DAODevices();
        ~DAODevices(void);
        static const std::string tableModule;
        static const std::string tableDevices;
        static const DeviceColumns col;
        static const ModuleColumns moduleCol;
        /**
         * @return XML string with all devices on gate
         */
        std::string getXMLAllDevs(gatewayId_64b gateId);
        std::string getXMLdevices(int userId, std::vector<gatewayId_64b> gateVector, std::vector<deviceId_64b> devicesVec);
        /**
         * @return XML string with devices on gate which is not initialized 
         */
        std::string getXMLNewDevices(gatewayId_64b gateId);
        std::string getXMLDeviceLog(gatewayId_64b gateId, device device, std::string logFrom, std::string logTo, std::string dataType, std::string interval);
        int updateFacility(gatewayId_64b gateId, std::string id, std::string init, std::string locationId, std::string name) ;

        std::string getXMLDevicesQueryString();
};

#endif	/* DAODEVICES_H */

