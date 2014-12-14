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
    LimitExceededNotification(string email, int notificationId,
            vector<string> gcmIds, long time, string message, int adapterId, string deviceId, int type, int offset);
    ~LimitExceededNotification();
    string getJson();
private:
    int mAdapterId, mType, mOffset;
    string mDeviceId;

};

#endif	/* LIMITEXCEEDEDNOTIFICATION_H */

