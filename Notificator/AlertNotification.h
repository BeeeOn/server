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
#include "VisibleNotification.h"

class AlertNotification : VisibleNotification {
public:
    virtual string getJson() = 0;
    virtual string getDbXml() = 0;
    virtual ~AlertNotification() = 0;
    string getType();
    int getLevel();
protected:
    AlertNotification(string name, int userId, int notificationId,
        vector<string> gcmIds, long time, string message);

private:
    const string TYPE_ALERT;
    const int LEVEL_ALERT;
};

#endif	/* ALERTNOTIFICATION_H */

