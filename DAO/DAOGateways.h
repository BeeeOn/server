#ifndef DAOGATES_H
#define	DAOGATES_H

#include "../ui_logger/Logger.h"

#include "DAO.h"

#include <math.h>
struct Gate 
{
    gatewayId_64b id;
    std::string name;
    double timezone;
    double altitude;
    double longitude;
    double latitude;
    std::string status;
    
    Gate()
        :   id(0),
            timezone(NAN),
            altitude(NAN),
            longitude(NAN),
            latitude(NAN)
    { }
};

struct GateInfo : public Gate
{
    int ip;
    int version; 
    std::string owner;
    int nUsers;
    int nFacilities;
    
    GateInfo()
        :   ip(0),
            version(0),
            nUsers(0),
            nFacilities(0)
    { }
};

struct GatewayColumns
{
    std::string id;
    std::string name;
    std::string version;
    std::string socket;
    std::string timezone;
    std::string logging;
    std::string altitude;
    std::string longitude;
    std::string latitude;
    std::string status;
    
    GatewayColumns():
            id("gateway_id"), 
            name("home_name"), 
            version("version"),
            socket("socket"),
            timezone("home_timezone"),
            logging("logging_days"),
            altitude("altitude"),
            longitude("longitude"),
            latitude("latitude"),
            status("status")
            { }
};

class DAOGateways 
    :   public DAO 
{
    public:
        DAOGateways();
        ~DAOGateways(void);

        Gate get(gatewayId_64b gateId);

        int update(Gate gate);
        int remove(gatewayId_64b gateId);

        GateInfo getGateInfo(gatewayId_64b gateId);
        int exists(gatewayId_64b gateId);

        static const std::string tableGateway;
        static const std::string tableUsersGateway;
        static const GatewayColumns col;
    
    
};

#endif	/* DAOGATES_H */

