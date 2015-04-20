/* 
 * File:   LimitExceededNotification.h
 * Author: Martin
 *
 */

#ifndef LIMITEXCEEDEDNOTIFICATION_H
#define	LIMITEXCEEDEDNOTIFICATION_H

#include "InfoNotification.h"
#include <string>

class LimitExceededNotification : public InfoNotification {
public:
    LimitExceededNotification(int userId, int notificationId,
            vector<string> gcmIds, long time, string message, int adapterId, string deviceId, int type);
    ~LimitExceededNotification();
    string getJson();
    string getDbXml();
private:
    int mAdapterId, mType;
    string mDeviceId;

};

#endif	/* LIMITEXCEEDEDNOTIFICATION_H */

