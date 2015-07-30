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
    /**
     * 
     * @param adapterId 
     * @return XML in string with users which have access to specified adapter
     */
    std::string getXMLconAccounts(long long adapterId);
    /**
    * 
    * @param adapterId
    * @param userMail
    * @return number of deleted rows in DB
    */
    int delConAccount(long long adapterId, std::string userMail);
    /**
    * Changing user's role on adapter 
    * @param adapterId
    * @param userMail
    * @param newRole
    * @return number of changed rows in DB
    */
    int changeConAccount(long long adapterId, std::string userMail, std::string newRole);
    /**
    * Grant access to user on adapter, degree of access is given by role
    * @param adapterId
    * @param userMail
    * @param newRole
    * @return number of added rows to DB (0 or 1)
    */
    int addConAccount(long long adapterId, std::string userMail, std::string newRole);
    
    int parAdapterWithUserIfPossible(long long int adapterId, std::string adapterName, int userId);
    
};

#endif	/* DAOUSERSADAPTERS_H */

