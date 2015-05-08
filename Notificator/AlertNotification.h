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
#include <sstream>

class AlertNotification : public BaseNotification {
public:
    virtual ~AlertNotification() = 0;
    int getLevel();
    bool saveToDb();
protected:
    AlertNotification(string name, int userId, int notificationId,
        long time);
    virtual void addGcmData(JsonNotificationBuilder *builder) = 0;
    virtual void addDbXmlData(stringstream *ss) = 0;
    string getType();

private:
};

#endif	/* ALERTNOTIFICATION_H */

