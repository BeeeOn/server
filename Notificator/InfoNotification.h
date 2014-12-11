/* 
 * File:   InfoNotification.h
 * Author: Martin
 *
 * Created on 2. prosinec 2014, 0:13
 */

#ifndef INFONOTIFICATION_H
#define	INFONOTIFICATION_H

#include <string>
#include <vector>
#include "VisibleNotification.h"

class InfoNotification : public VisibleNotification {
public:
    virtual string getJson() = 0;
    virtual ~InfoNotification();
    string getMessage();
    string getEmail();
    string getNotificationId();
    string getGcmIds();
    string getTime();
    string getAction();
protected:
    InfoNotification(string email, int notificationId,
            vector<string> gcmIds, long time, string action, string message);
    string getType();
private:
    const string TYPE_CONTROL;
};

#endif	/* INFONOTIFICATION_H */

