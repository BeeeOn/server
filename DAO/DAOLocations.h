#ifndef DAOROOMS_H
#define	DAOROOMS_H

#include "DAO.h"
#include "DAOGateways.h"


struct LocationsColumns{
    std::string id;
    std::string gateway_id;
    std::string kind;
    std::string name;
    
    LocationsColumns():
            id("location_id"), 
            gateway_id("gateway_id"), 
            kind("kind"), 
            name("location_name")
            { }
};

class DAOLocations 
    :   public DAO 
{
    public:
        DAOLocations();
        static DAOLocations& getInstance();
        ~DAOLocations(void);

        static const std::string tableLocations;
        static const LocationsColumns col;

        int insertNewRoom(gatewayId_64b gateId, int roomType, std::string roomName);
        int update(gatewayId_64b gateId, int roomId, int type, std::string name);
        void remove(gatewayId_64b gateId, std::string roomId);
        std::string getXMLrooms(gatewayId_64b gateId);
};

#endif	/* DAOROOMS_H */

