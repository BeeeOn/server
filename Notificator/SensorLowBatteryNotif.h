/* 
 * File:   SensorLowBatteryNotif.h
 * Author: Martin
 *
 */

#ifndef SENSORLOWBATTERYNOTIFICATION_H
#define	SENSORLOWBATTERYNOTIFICATION_H

#include "AlertNotification.h"
#include <string>
#include "Utils.h"
#include <sstream>

class SensorLowBatteryNotif : public AlertNotification {
public:
    SensorLowBatteryNotif(int userId, int notificationId,
            long time, int adapterId, string deviceId, int battery);
    ~SensorLowBatteryNotif();
protected:
    void addGcmData(JsonNotificationBuilder *builder);
    void addDbXmlData(stringstream *ss);
    
private:
    int mAdapterId;
    string mDeviceId;
    int mBattery;

};

#endif	/* SENSORLOWBATTERYNOTIFICATION_H */

