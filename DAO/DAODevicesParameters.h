#ifndef DAODEVICESPARAMETERS_H
#define DAODEVICESPARAMETERS_H

#include "DAO.h"

struct deviceParameterPk 
{
    deviceId_64b euid;
    gatewayId_64b gatewayId;
    std::string key;
};

struct deviceParameter
    :   public deviceParameterPk
{
    std::string value;
};

struct DeviceParameterColumns{
    std::string euid;
    std::string key;
    std::string value;    
    std::string gateway_id;
    
    DeviceParameterColumns():
            euid("device_euid"), 
            key("parameter_key"),
            value("parameter_value"),
            gateway_id("gateway_id")
            { }
};

class DAODevicesParameters :
    public DAO
{
    public:
        DAODevicesParameters();
        virtual ~DAODevicesParameters();
        static const std::string tableDevicesParameter;
        static const DeviceParameterColumns col;
        
        bool create(deviceParameter deviceParam);
        deviceParameter get(deviceParameter deviceParam);
        int update(deviceParameter deviceParam);
        int remove(deviceParameter deviceParam);
        
    private:

};

#endif /* DAODEVICESPARAMETERS_H */

