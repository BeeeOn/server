#ifndef DAOUSERSADAPTERS_H
#define	DAOUSERSADAPTERS_H
#include "../ui_logger/Logger.h"
#include "DAO.h"
#include "DAOGateways.h"

struct UserGatewaysColumns{
    std::string user_id;
    std::string gateway_id;
    std::string permission;
    
    UserGatewaysColumns():
            user_id("user_id"), 
            gateway_id("gateway_id"), 
            permission("permission")
            { }
};

class DAOUsersGateways 
    :   public DAO 
{
public:
    DAOUsersGateways();
    ~DAOUsersGateways(void);
    
    static const std::string tableUsersGateways;
    static const UserGatewaysColumns col;
    std::string getXMLconAccounts(gatewayId_64b gatewayId);
    int delConAccount(gatewayId_64b gatewayId, std::string userMail);
    int delConAccount(gatewayId_64b gatewayId, int userId);
    int changeConAccount(gatewayId_64b gatewayId, int userId, std::string newRole);
    int changeConAccount(gatewayId_64b gatewayId, std::string userMail, std::string newRole);
    int addConAccount(gatewayId_64b gatewayId, std::string userMail, std::string newRole);
    int addConAccount(gatewayId_64b gatewayId, int userId, std::string newRole);
    
    int parGateWithUserIfPossible(gatewayId_64b gateId, int userId);
    
};

#endif	/* DAOUSERSADAPTERS_H */

