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

class DAODevices : public DAO {
private:
    DAODevices();
    DAODevices(DAODevices const&);// Don't Implement
    void operator=(DAODevices const&); // Don't implement
public:
    static DAODevices& getInstance();
    ~DAODevices(void);
    
    /**
     * 
     * @param adapterId
     * @return XML string with all devices on adapter
     */
    std::string getXMLAllDevs(std::string adapterId);
    std::string getXMLdevices(int userId, std::vector<std::string> adaptersVec, std::vector<device> devicesVec);
    /**
     * 
     * @param adapterId
     * @return XML string with devices on adapter which is not initialized 
     */
    std::string getXMLNewDevices(std::string adapterId);
    /**
     * 
     * @param adapterId
     * @param id
     * @param type
     * @param name
     * @param visibility
     * @return number of updated rows in DB (0 or 1)
     */
    int updateDevice(std::string adapterId, std::string id, std::string type, std::string name, std::string visibility);
    std::string getXMLDeviceLog(std::string adapterId, device device, std::string logFrom, std::string logTo, std::string dataType, std::string interval);
    int updateFacility(std::string adapterId, std::string id, std::string init, std::string locationId, std::string refresh) ;
    
private: 
    std::string escapeString(std::string str);

    std::string getXMLDevicesQueryString(std::string facilitiesCond="");
};

#endif	/* DAODEVICES_H */

