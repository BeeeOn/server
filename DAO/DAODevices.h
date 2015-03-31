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
    
    
    std::string getXMLAllDevs(std::string adapter);
    std::string getXMLdevices(int userId, std::vector<std::string> adaptersVec, std::vector<device> devicesVec);
    std::string getXMLNewDevices(std::string adapterId);

    int updateDevice(std::string adapterId, std::string id, std::string type, std::string name, std::string visibility);
    std::string getXMLDeviceLog(std::string adapterId, device device, std::string logFrom, std::string logTo, std::string dataType, std::string interval);
    int updateFacility(std::string adapterId, std::string id, std::string init, std::string locationId, std::string refresh) ;
    
private: 
    std::string escapeString(std::string str);

    std::string getXMLDevicesQueryString(std::string facilitiesCond="");
};

#endif	/* DAODEVICES_H */

