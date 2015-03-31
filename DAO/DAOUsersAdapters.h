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
class DAOUsersAdapters : public DAO {
private:
    DAOUsersAdapters();
    DAOUsersAdapters(DAOUsersAdapters const&);// Don't Implement
    void operator=(DAOUsersAdapters const&); // Don't implement
public:
    static DAOUsersAdapters& getInstance();
    ~DAOUsersAdapters(void);

    std::string getXMLconAccounts(std::string adapterId);
    int delConAccount(std::string adapterId, std::string userMail);
    int changeConAccount(std::string adapterId, std::string userMail, std::string newRole);
    int addConAccount(std::string adapterId, std::string userMail, std::string newRole);
};

#endif	/* DAOUSERSADAPTERS_H */

