/* 
 * File:   DAONotification.h
 * Author: Pavel
 *
 * Created on 23. dubna 2015, 13:29
 */

#ifndef DAONOTIFICATION_H
#define	DAONOTIFICATION_H

#include "DAO.h"


class DAONotification : public DAO {
private:
    DAONotification();
    DAONotification(DAONotification const&);// Don't Implement
    void operator=(DAONotification const&); // Don't implement
public:
    static DAONotification& getInstance();
    ~DAONotification(void);

    std::string getXMLNotifications(int userId);

};

#endif	/* DAONOTIFICATION_H */

