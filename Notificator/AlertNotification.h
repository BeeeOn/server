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

    virtual ~AlertNotification();
protected:
    string getType() = 0;
    AlertNotification(string email, int notificationId,
        vector<string> gcmIds, long time, string action, string message);

private:
    const string TYPE_CONTROL;
};

#endif	/* ALERTNOTIFICATION_H */

