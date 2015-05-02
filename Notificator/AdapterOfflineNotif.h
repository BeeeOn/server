/* 
 * File:   AdapterOfflineNotif.h
 * Author: Martin
 *
 */

#ifndef ADAPTEROFFLINENOTIFICATION_H
#define	ADAPTEROFFLINENOTIFICATION_H

#include "AlertNotification.h"
#include <string>
#include "Utils.h"
#include <sstream>

class AdapterOfflineNotif : public AlertNotification {
public:
    AdapterOfflineNotif(int userId, int notificationId,
            long time, int adapterId);
    ~AdapterOfflineNotif();
protected:
    void addGcmData(JsonNotificationBuilder *builder);
    void addDbXmlData(stringstream *ss);
private:
    int mAdapterId;
};

#endif	/* ADAPTEROFFLINENOTIFICATION_H */

