#ifndef DAOMOBILEDEVICES_H
#define	DAOMOBILEDEVICES_H

#include "DAO.h"
#include "../ui_logger/Logger.h"

struct MobileDevice
{
    std::string token;
    std::string mobile_id;
    std::string type;
    std::string locale;
    std::string push_notification;
    long long int user_id;    
};

class DAOMobileDevices 
    :   public DAO 
{
    public:
        DAOMobileDevices();
        ~DAOMobileDevices(void);

        int delGCMId(int oldUserId, std::string gcmid);
        int setGCMId(std::string token, std::string gcmid) ;

        int upsertMobileDevice(MobileDevice mobile, int userId);

        int insertPushNotification(int userId, std::string push_notif);
        int deletepushNotification(int oldUserId, std::string notification);
};

#endif	/* DAOMOBILEDEVICES_H */

