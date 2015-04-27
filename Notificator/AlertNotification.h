/* 
 * File:   AlertNotification.h
 * Author: Martin
 *
 * Created on 1. prosinec 2014, 23:56
 */

#ifndef ALERTNOTIFICATION_H
#define	ALERTNOTIFICATION_H

#include <string>
#include <vector>
#include "BaseNotification.h"
#include "JsonNotificationBuilder.h"

class AlertNotification : BaseNotification {
public:
    virtual bool saveToDb();
    virtual string getDbXml() = 0;
    virtual ~AlertNotification() = 0;
    int getLevel();
protected:
    AlertNotification(string name, int userId, int notificationId,
        long time);
    virtual void addGcmData(JsonNotificationBuilder *builder) = 0;
    string getType();

private:
};

#endif	/* ALERTNOTIFICATION_H */

