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
    virtual ~AdvertNotification();
protected:
    AdvertNotification(string email, int notificationId,
            vector<string> gcmIds, long time, string action, string message);
    string getType() = 0;
private:
    const string TYPE_CONTROL;
};

#endif	/* ADVERTNOTIFICATION_H */

