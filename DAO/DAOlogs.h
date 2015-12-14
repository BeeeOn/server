#ifndef DAOLOGS_H
#define	DAOLOGS_H

#include "DAO.h"
#include "DAODevices.h"

struct LogsColumns{
    std::string device_euid;
    std::string module_id;
    std::string value;
    std::string time;
    std::string gateway_id;
    
    LogsColumns():
            device_euid("device_euid"), 
            module_id("module_id"), 
            value("measured_value"), 
            time("measured_at"),
            gateway_id("gateway_id")
            { }
};

class DAOlogs 
    :   public DAO
{
    public:
        DAOlogs();
        ~DAOlogs(void);

        static const std::string tableLogs;
        static const LogsColumns col;

        std::string getXMLDeviceLog(gatewayId_64b gateId, device device, long long logFrom, long long logTo, std::string dataType, long long interval);

};

#endif	/* DAOLOGS_H */

