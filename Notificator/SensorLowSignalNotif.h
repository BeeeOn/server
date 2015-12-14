/* 
 * File:   SensorLowSignalNotif.h
 * Author: Martin
 *
 */

#ifndef SENSORLOWSIGNALNOTIFICATION_H
#define	SENSORLOWSIGNALNOTIFICATION_H

#include "AlertNotification.h"
#include <string>
#include "Utils.h"
#include <sstream>

class SensorLowSignalNotif : public AlertNotification {
public:
    SensorLowSignalNotif(int userId, int notificationId,
            long time, int adapterId, std::string deviceId, int signal);
            
    SensorLowSignalNotif(int userId, int notificationId,
            long time, long long int adapterId, std::string deviceId, int signal);
    ~SensorLowSignalNotif();
protected:
    void addGcmData(JsonNotificationBuilder *builder);
    void addDbXmlData(stringstream *ss);
    
private:
    long long int mAdapterId;
    string mDeviceId;
    int mSignal;

};

#endif	/* SENSORLOWSIGNALNOTIFICATION_H */

