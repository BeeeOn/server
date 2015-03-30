/* 
 * File:   DAOMobileDevices.h
 * Author: pavel
 *
 * Created on 29. březen 2015, 21:47
 */

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


class DAOMobileDevices : public DAO {
private:
    DAOMobileDevices();
    DAOMobileDevices(DAOMobileDevices const&);// Don't Implement
    void operator=(DAOMobileDevices const&); // Don't implement
public:
    static DAOMobileDevices& getInstance();
    ~DAOMobileDevices(void);

    
int delGCMId(std::string oldUserId, std::string gcmid);
    int setGCMId(std::string IHAtoken, std::string phoneid,int userId, std::string gcmid) ;
    
int upsertMobileDevice(MobileDevice mobile, int userId);
};

#endif	/* DAOMOBILEDEVICES_H */

