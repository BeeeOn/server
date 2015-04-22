/* 
 * File:   WatchdogNotif.h
 * Author: Martin
 *
 */

#ifndef LIMITEXCEEDEDNOTIFICATION_H
#define	LIMITEXCEEDEDNOTIFICATION_H

#include "InfoNotification.h"
#include <string>

class WatchdogNotif : public InfoNotification {
public:
    WatchdogNotif(int userId, int notificationId,
            long time, string message, int adapterId, string deviceId, int type);
    ~WatchdogNotif();
    string getDbXml();

protected:
    string getGcmMsg(string ids);
private:
    int mAdapterId, mSensorType;
    string mDeviceId;

};

#endif	/* LIMITEXCEEDEDNOTIFICATION_H */

