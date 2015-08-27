/* 
 * File:   DAOPushNotificationService.h
 * Author: pavel
 *
 * Created on 27. srpen 2015, 13:52
 */

#ifndef DAOPUSHNOTIFICATIONSERVICE_H
#define	DAOPUSHNOTIFICATIONSERVICE_H

#include "DAO.h"
#include "../ui_logger/Logger.h"

struct pushNotificationServiceColumns{
    std::string id;
    std::string userId;
    std::string serviceReference;
    std::string socket;
    std::string timezone;
    std::string logging;
    
    pushNotificationServiceColumns():
            id("push_notification_service_id"), 
            userId("user_id"), 
            serviceReference("service_reference_id")
            { }
};

class DAOPushNotificationService : public DAO {
private:
    DAOPushNotificationService();
    DAOPushNotificationService(DAOPushNotificationService const&);// Don't Implement
    void operator=(DAOPushNotificationService const&); // Don't implement
public:
    static DAOPushNotificationService& getInstance();
    ~DAOPushNotificationService(void);

    int insertPushNotification(int userId, std::string push_notif);
    int deletepushNotification(int oldUserId, std::string notification);
    
    static const std::string tablePushNotifService;
    static const pushNotificationServiceColumns col;
};


#endif	/* DAOPUSHNOTIFICATIONSERVICE_H */

