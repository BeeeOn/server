/* 
 * File:   DAODevices.h
 * Author: pavel
 *
 * Created on 27. březen 2015, 13:33
 */

#ifndef DAODEVICES_H
#define	DAODEVICES_H

#include "DAO.h"
#include "../ui_logger/Logger.h"

struct device {
    std::string id;
    std::string type;
};

struct DeviceColumns{
    std::string mac;
    std::string id;
    std::string name;
    std::string refresh;
    std::string init;
    std::string involved;
    std::string measured_at;
    std::string location_id;
    std::string gateway_id;
    
    DeviceColumns():
            mac("device_mac"), 
            id("device_type"), 
            name("device_name"),
            refresh("refresh"),
            init("init"),
            involved("involved"),
            measured_at("measured_at"),
            location_id("location_id"),
            gateway_id("gateway_id")
            { }
};

struct ModuleColumns{
    std::string mac;
    std::string id;
    std::string value;
    
    ModuleColumns():
            mac("device_mac"), 
            id("module_id"), 
            value("measured_value")
            { }
};

class DAODevices : public DAO {
private:
    DAODevices();
    DAODevices(DAODevices const&);// Don't Implement
    void operator=(DAODevices const&); // Don't implement
public:
    static DAODevices& getInstance();
    ~DAODevices(void);
    static const std::string tableModule;
    static const std::string tableDevices;
    static const DeviceColumns col;
    static const ModuleColumns moduleCol;
    /**
     * 
     * @param adapterId
     * @return XML string with all devices on adapter
     */
    std::string getXMLAllDevs(long long adapterId);
    std::string getXMLdevices(int userId, std::vector<std::string> adaptersVec, std::vector<device> devicesVec);
    /**
     * 
     * @param adapterId
     * @return XML string with devices on adapter which is not initialized 
     */
    std::string getXMLNewDevices(long long adapterId);
    /**
     * 
     * @param adapterId
     * @param id
     * @param type
     * @param name
     * @param visibility
     * @return number of updated rows in DB (0 or 1)
     */
//    int updateDevice(long long adapterId, std::string id, std::string type, std::string name, std::string visibility);
    std::string getXMLDeviceLog(long long adapterId, device device, std::string logFrom, std::string logTo, std::string dataType, std::string interval);
    int updateFacility(long long adapterId, std::string id, std::string init, std::string locationId, std::string refresh, std::string name) ;
    
private: 

    std::string getXMLDevicesQueryString(std::string facilitiesCond="");
};

#endif	/* DAODEVICES_H */

