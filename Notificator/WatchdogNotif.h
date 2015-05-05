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
#include <sstream>

class WatchdogNotif : public InfoNotification {
public:
    WatchdogNotif(int userId, int notificationId,
            long time, string message, int adapterId, string deviceId, int type, int algId);
    ~WatchdogNotif();
protected:
    void addGcmData(JsonNotificationBuilder *builder);
    void addDbXmlData(stringstream *ss);

private:
    int mAdapterId, mDeviceType, mAlgId;
    string mDeviceId, mMsg;

};

#endif	/* WATCHDOGNOTIFICATION_H */

