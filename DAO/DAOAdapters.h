/* 
 * File:   DAOAdapters.h
 * Author: pavel
 *
 * Created on 17. březen 2015, 17:15
 */

#ifndef DAOADAPTERS_H
#define	DAOADAPTERS_H

#include "../ui_logger/Logger.h"

#include "DAO.h"

struct Gate {
    long long int  id;
    std::string name;
    int timezone;
};

struct GateInfo {
    long long int id;
    std::string name;
    int ip;
    int version; 
    int nUsers;
    int nFacilities;
    int timezone;
    
    GateInfo():id(-1) { }
};

struct GatewayColumns{
    std::string id;
    std::string name;
    std::string version;
    std::string socket;
    std::string timezone;
    std::string logging;
    
    GatewayColumns():
            id("gateway_id"), 
            name("home_name"), 
            version("version"),
            socket("socket"),
            timezone("home_timezone"),
            logging("logging_days")
            { }
};

class DAOAdapters : public DAO {
private:
    DAOAdapters();
    DAOAdapters(DAOAdapters const&);// Don't Implement
    void operator=(DAOAdapters const&); // Don't implement
public:
    static DAOAdapters& getInstance();
    ~DAOAdapters(void);
    
    Gate getAdapter(long long adapterId);
    int updateAdapter(long long adapterId, std::string newName, std::string newTimeZone);
    int deleteAdapter(long long adapterId);
    
    GateInfo getGateInfo(long long int gateId);
    int isAdapterInDB(long long int adapterId);
//    std::string getTimeZone(std::string adapterId);
    
    void updateAdaptersTimezone(std::string adapterId,  std::string newTimeZone);
    int delUsersAdapter(long long adapterId, int userId);
    
    static const std::string tableGateway;
    static const std::string tableUsersGateway;
    static const GatewayColumns col;
    
    
};

#endif	/* DAOADAPTERS_H */

