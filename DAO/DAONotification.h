#ifndef DAONOTIFICATION_H
#define	DAONOTIFICATION_H

#include "DAO.h"

class DAONotification 
    :   public DAO 
    {
    public:
        DAONotification();
        ~DAONotification(void);

        std::string getXMLNotifications(int userId);
        int setReaded(int notif);
};

#endif	/* DAONOTIFICATION_H */

