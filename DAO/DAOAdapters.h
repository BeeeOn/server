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
    std::string id;
    std::string name;
    int timezone;
};

struct GateInfo {
    std::string id;
    std::string name;
    int socket;
    int version; 
    int nUsers;
    int nFacilities;
    int timezone;
};

class DAOAdapters : public DAO {
private:
    DAOAdapters();
    DAOAdapters(DAOAdapters const&);// Don't Implement
    void operator=(DAOAdapters const&); // Don't implement
public:
    static DAOAdapters& getInstance();
    ~DAOAdapters(void);
    
    Gate getAdapter(std::string adapterId);
    int updateAdapter(std::string adapterId, std::string newName, std::string newTimeZone);
    int deleteAdapter(std::string adapterId);
    
    GateInfo getGateInfo(std::string gateId);
    int parAdapterWithUserIfPossible(long long int adapterId, std::string adapterName, int userId);
    int isAdapterInDB(long long int adapterId);
    std::string getTimeZone(std::string adapterId);
    
    void updateAdaptersTimezone(std::string adapterId,  std::string newTimeZone);
    int delUsersAdapter(std::string adapterId, int userId);
};

#endif	/* DAOADAPTERS_H */

