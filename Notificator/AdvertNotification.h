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
#include "BaseNotification.h"
#include "JsonNotificationBuilder.h"

class AdvertNotification : BaseNotification{
public:
    virtual string getDbXml() = 0;
    virtual ~AdvertNotification();
    virtual bool saveToDb();

    int getLevel();

protected:
    AdvertNotification(string name, int userId, int notificationId,
            long time);
    virtual void addGcmData(JsonNotificationBuilder *builder) = 0;
    string getType();
private:
};

#endif	/* ADVERTNOTIFICATION_H */

