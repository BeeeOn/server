/* 
 * File:   DAOlogs.h
 * Author: pavel
 *
 * Created on 29. červenec 2015, 11:07
 */

#ifndef DAOLOGS_H
#define	DAOLOGS_H

#include "DAO.h"
#include "DAODevices.h"

struct LogsColumns{
    std::string device_mac;
    std::string module_id;
    std::string value;
    std::string time;
    
    LogsColumns():
            device_mac("device_mac"), 
            module_id("module_id"), 
            value("measured_at"), 
            time("measured_value")
            { }
};

class DAOlogs : public DAO{
    private:
    DAOlogs();
    DAOlogs(DAOlogs const&);// Don't Implement
    void operator=(DAOlogs const&); // Don't implement
public:
    static DAOlogs& getInstance();
    ~DAOlogs(void);
    
    static const std::string tableLogs;
    static const LogsColumns col;
    
    std::string getXMLDeviceLog(long long adapterId, device device, std::string logFrom, std::string logTo, std::string dataType, std::string interval);

};

#endif	/* DAOLOGS_H */

