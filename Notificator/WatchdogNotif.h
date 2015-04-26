/* 
 * File:   WatchdogNotif.h
 * Author: Martin
 *
 */

#ifndef LIMITEXCEEDEDNOTIFICATION_H
#define	LIMITEXCEEDEDNOTIFICATION_H

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

#endif	/* LIMITEXCEEDEDNOTIFICATION_H */

