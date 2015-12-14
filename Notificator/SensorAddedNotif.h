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
#include <sstream>

class SensorAddedNotif : public InfoNotification {
public:
    SensorAddedNotif(int userId, int notificationId,
            long time, int adapterId, string deviceId);
    ~SensorAddedNotif();
protected:
    void addGcmData(JsonNotificationBuilder *builder);
    void addDbXmlData(stringstream *ss);
    
private:
    int mAdapterId;
    string mDeviceId;

};

#endif	/* SENSORADDEDNOTIFICATION_H */

