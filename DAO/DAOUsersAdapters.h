/* 
 * File:   DAOUsersAdapters.h
 * Author: pavel
 *
 * Created on 29. březen 2015, 21:51
 */

#ifndef DAOUSERSADAPTERS_H
#define	DAOUSERSADAPTERS_H
#include "../ui_logger/Logger.h"
#include "DAO.h"

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

class DAOUsersAdapters : public DAO {
private:
    DAOUsersAdapters();
    DAOUsersAdapters(DAOUsersAdapters const&);// Don't Implement
    void operator=(DAOUsersAdapters const&); // Don't implement
public:
    static DAOUsersAdapters& getInstance();
    ~DAOUsersAdapters(void);
    
    static const std::string tableUsersGateways;
    static const UserGatewaysColumns col;
    std::string getXMLconAccounts(long long gatewayId);
    int delConAccount(long long gatewayId, std::string userMail);
    int delConAccount(long long gatewayId, int userId);
    int changeConAccount(long long gatewayId, int userId, std::string newRole);
    int changeConAccount(long long gatewayId, std::string userMail, std::string newRole);
    int addConAccount(long long gatewayId, std::string userMail, std::string newRole);
    int addConAccount(long long gatewayId, int userId, std::string newRole);
    
    int parAdapterWithUserIfPossible(long long int adapterId, std::string adapterName, int userId);
    
};

#endif	/* DAOUSERSADAPTERS_H */

