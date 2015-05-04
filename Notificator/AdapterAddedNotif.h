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
#include <sstream>

class AdapterAddedNotif : public InfoNotification {
public:
    AdapterAddedNotif(int userId, int notificationId,
            long time, int adapterId);
    ~AdapterAddedNotif();
protected:
    void addGcmData(JsonNotificationBuilder *builder);
    void addDbXmlData(stringstream *ss);
private:
    int mAdapterId;
};

#endif	/* ADAPTERADDEDNOTIFICATION_H */

