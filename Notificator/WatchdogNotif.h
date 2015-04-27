/* 
 * File:   WatchdogNotif.h
 * Author: Martin
 *
 */

#ifndef WATCHDOGNOTIFICATION_H
#define	WATCHDOGNOTIFICATION_H

#include "InfoNotification.h"
#include <string>
#include "Utils.h"

class WatchdogNotif : public InfoNotification {
public:
    WatchdogNotif(int userId, int notificationId,
            long time, string message, int adapterId, string deviceId, int type);
    ~WatchdogNotif();
    string getDbXml();
protected:
    void addGcmData(JsonNotificationBuilder *builder);
private:
    int mAdapterId, mDeviceType;
    string mDeviceId, mMsg;

};

#endif	/* WATCHDOGNOTIFICATION_H */

