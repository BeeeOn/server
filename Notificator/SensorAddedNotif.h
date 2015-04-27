/* 
 * File:   SensorAddedNotif.h
 * Author: Martin
 *
 */

#ifndef SENSORADDEDNOTIFICATION_H
#define	SENSORADDEDNOTIFICATION_H

#include "InfoNotification.h"
#include <string>
#include "Utils.h"

class SensorAddedNotif : public InfoNotification {
public:
    SensorAddedNotif(int userId, int notificationId,
            long time, int adapterId, string deviceId);
    ~SensorAddedNotif();
    string getDbXml();
protected:
    void addGcmData(JsonNotificationBuilder *builder);
private:
    int mAdapterId;
    string mDeviceId;

};

#endif	/* SENSORADDEDNOTIFICATION_H */

