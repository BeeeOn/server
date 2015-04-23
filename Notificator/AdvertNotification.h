/* 
 * File:   AdvertNotification.h
 * Author: Martin
 *
 * Created on 2. prosinec 2014, 0:17
 */

#ifndef ADVERTNOTIFICATION_H
#define	ADVERTNOTIFICATION_H

#include <string>
#include <vector>
#include "VisibleNotification.h"

class AdvertNotification : VisibleNotification{
public:
    virtual string getDbXml() = 0;
    virtual ~AdvertNotification();
    
    int getLevel();

protected:
    AdvertNotification(string name, int userId, int notificationId,
            long time,  string message);
    virtual string getGcmMsg(string ids) = 0;
    string getType();
private:
};

#endif	/* ADVERTNOTIFICATION_H */

