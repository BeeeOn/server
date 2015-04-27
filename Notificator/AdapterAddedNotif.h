/* 
 * File:   AdapterAddedNotif.h
 * Author: Martin
 *
 */

#ifndef ADAPTERADDEDNOTIFICATION_H
#define	ADAPTERADDEDNOTIFICATION_H

#include "InfoNotification.h"
#include <string>
#include "Utils.h"

class AdapterAddedNotif : public InfoNotification {
public:
    AdapterAddedNotif(int userId, int notificationId,
            long time, int adapterId);
    ~AdapterAddedNotif();
    string getDbXml();
protected:
    void addGcmData(JsonNotificationBuilder *builder);
private:
    int mAdapterId;
};

#endif	/* ADAPTERADDEDNOTIFICATION_H */

